#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "TilesAnimations0.h"
#include "custom_datas.h"
#include "sgb_palette.h"

#define MAX_WAIT_CHAR 4

IMPORT_TILES(font);
IMPORT_TILES(dialogtilesbase);
IMPORT_MAP(dialogmapbase);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern Sprite* s_motherpl;
extern Sprite* s_motherow;
extern UINT8 previous_state;
extern unsigned char EMPTY_STRING_21[];
extern unsigned char d0[];
extern WHOSTALKING whostalking;
extern struct MISSION enable_hospital;
extern struct MISSION engage_smith;
extern struct MISSION find_blackie;
extern struct MISSION outwalker_smith;

extern UINT8 dialog_ready;
extern UINT8 counter_char;
extern UINT8 wait_char;
extern UINT8 writing_line;
extern UINT8 n_lines;
extern Sprite* dialog_cursor;
extern UINT8 next_page;
extern UINT8 previous_state;

extern void move_on() BANKED;
extern void GetLocalizedDialog_EN(UINT8* n_lines) BANKED;
extern void dialog_map() BANKED;
extern UINT8 get_quantity(INVITEMTYPE itemtype) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern void shift_text_one_line_up() BANKED;
extern void show_next_character() BANKED;

void START() {
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume 0x77
    SpriteManagerLoad(SpriteInvcursor);
    //SGB PALETTE
        if(sgb_check()){
            set_sgb_palette01_WOLF();
        }
	//PlayMusic(bgm_credits, 0);
    HIDE_WIN;
    InitScroll(BANK(dialogmapbase), &dialogmapbase, 0, 0);
    dialog_map();
    INIT_FONT(font, PRINT_BKG);
    SHOW_BKG;
    wait_char = MAX_WAIT_CHAR;
    writing_line = 1u;
    counter_char = 0u;
    n_lines = 0u;
    dialog_ready = 0u;
	next_page = 0u;
}

void UPDATE() {
    if(KEY_RELEASED(J_UP)){
        counter_char = 0u;
        dialog_ready = 0u; 
    }
    if(dialog_ready == 0u){
        PRINT(0, 7, EMPTY_STRING_21);
        PRINT(0, 8, EMPTY_STRING_21);
        PRINT(0, 9, EMPTY_STRING_21);
        PRINT(0, 10, EMPTY_STRING_21);
        PRINT(0, 11, EMPTY_STRING_21);
        PRINT(0, 12, EMPTY_STRING_21);
        PRINT(0, 13, EMPTY_STRING_21);
        PRINT(0, 14, EMPTY_STRING_21);
        SpriteManagerRemoveSprite(dialog_cursor);
        n_lines = 0u;
		switch(engage_smith.mission_state){
			case MISSION_STATE_ENABLED:
                switch(engage_smith.current_step){
                    case 0: 
				        whostalking = SMITH_DISABLED;
                    break;
                    default:
                        //check for mission completed
                        whostalking = SMITH_CHECKING_WOODANDMETAL; 
                        if(get_quantity(INVITEM_METAL) >= 10u 
                            && get_quantity(INVITEM_WOOD) >= 10u){//se nell' inventario non ha ancora il materiale
                            if(engage_smith.mission_state < MISSION_STATE_ACCOMPLISHED){
                                engage_smith.mission_state = MISSION_STATE_REWARDED;
                            }
                            if(find_blackie.mission_state < MISSION_STATE_ENABLED){
                                find_blackie.mission_state = MISSION_STATE_ENABLED;
                            }
                            next_page = 1u;
                        }else{
                            whostalking = SMITH_CHECKING_NOWOODANDMETAL;
                        }
                    break;
                }
			break;
			case MISSION_STATE_ACCOMPLISHED:
			case MISSION_STATE_REWARDED:
                //normal behave
                if(outwalker_smith.mission_state == MISSION_STATE_DISABLED || 
                    outwalker_smith.mission_state == MISSION_STATE_REWARDED){   
                    whostalking = SMITH_CHECKING_NOWOODANDMETAL;
                    if(get_quantity(INVITEM_METAL) >= 10u 
                        && get_quantity(INVITEM_WOOD) >= 10u){
                        change_quantity(INVITEM_METAL, -10);
                        change_quantity(INVITEM_WOOD, -10);
                        change_quantity(INVITEM_ARROW_NORMAL, 30);
                        whostalking = SMITH_GIVING_ARROWS;
                    }
                    /*
                    if(get_quantity(INVITEM_POWDER) > 0u){
                        change_quantity(INVITEM_POWDER, -1);
                        change_quantity(INVITEM_BOMB, 1);
                        whostalking = SMITH_GIVING_BOMB;
                        next_page = 1u;
                    }*/
                }
                //special behave for outwalker_smith mission
                else{
                    if(outwalker_smith.mission_state == MISSION_STATE_ENABLED){
                        whostalking = SMITH_FLOWERS_ASKED;
                        outwalker_smith.mission_state = MISSION_STATE_STARTED;
                    }else if(outwalker_smith.mission_state == MISSION_STATE_STARTED){
                        UINT8 flowers = get_quantity(INVITEM_FLOWER);
                        if(flowers < 4){
                            whostalking = SMITH_FLOWERS_MISSING;
                        }else{
                            whostalking = SMITH_FLOWERS_THANKYOU;
                            outwalker_smith.mission_state = MISSION_STATE_REWARDED;
                            change_quantity(INVITEM_FLOWER, -4);
                            change_quantity(INVITEM_BOX, 1);
                        }
                    }
                }
			break;
		}
        GetLocalizedDialog_EN(&n_lines);
        wait_char = MAX_WAIT_CHAR;
        writing_line = 1u;
        dialog_ready = 1u;
        PRINT(0, 7, d0);
    }
    if(dialog_ready == 1u){
        if(KEY_PRESSED(J_A) || KEY_PRESSED(J_B) || KEY_PRESSED(J_DOWN)){
            wait_char = 1u;
        }
        wait_char--;
        if(wait_char == 0u){//mostra lettera successiva
            show_next_character();
        }
    }
    if(dialog_ready == 2u){
        dialog_cursor = SpriteManagerAdd(SpriteInvcursor,(UINT16)144u, (UINT16)120u);
        struct ItemSpawned* dialogcursor_data = (struct ItemSpawned*)dialog_cursor->custom_data;
        dialogcursor_data->configured = 2;
        dialog_ready = 3u;
    }
    if(dialog_ready == 3u && (KEY_RELEASED(J_A) || KEY_RELEASED(J_B))){
        if(next_page){
            next_page = 0u;
            dialog_ready = 0u;
            return;
        }
        dialog_ready = 0;
        move_on();
    }
}

