#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Music.h"

#include "TilesAnimations0.h"
//#include "Dialogs.h"
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
extern unsigned char d1[];
extern unsigned char d2[];
extern unsigned char d3[];
extern unsigned char d4[];
extern unsigned char d5[];
extern unsigned char d6[];
extern unsigned char d7[];
extern WHOSTALKING whostalking;
extern struct MISSION missions[4];

extern UINT8 dialog_ready;
extern UINT8 counter_char;
extern UINT8 wait_char;
extern UINT8 writing_line;
extern UINT8 n_lines;
extern Sprite* dialog_cursor;
extern UINT8 next_page;

extern void move_on();

extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
extern void GetLocalizedDialog_EN(UINT8* n_lines) BANKED;
extern void dialog_map() BANKED;
extern UINT8 get_quantity(INVITEMTYPE itemtype) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;


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
    if(KEY_PRESSED(J_A) || KEY_PRESSED(J_B) || KEY_PRESSED(J_DOWN)){
        wait_char = 1u;
    }    
    if(dialog_ready > 1 && next_page == 0 && KEY_RELEASED(J_JUMP)){
        move_on();
    }
    if(dialog_ready == 0u){
        PRINT(0, 7, EMPTY_STRING_21);
        PRINT(0, 8, EMPTY_STRING_21);
        PRINT(0, 9, EMPTY_STRING_21);
        PRINT(0, 10, EMPTY_STRING_21);
        PRINT(0, 11, EMPTY_STRING_21);
        PRINT(0, 12, EMPTY_STRING_21);
        PRINT(0, 13, EMPTY_STRING_21);
        SpriteManagerRemoveSprite(dialog_cursor);
        n_lines = 0u;
		switch(missions[1].mission_state){
			case MISSION_STATE_DISABLED:
				whostalking = SMITH_DISABLED; 
			break;
			case MISSION_STATE_ENABLED:
				//check for mission completed
				whostalking = SMITH_CHECKING_WOODANDMETAL; 
				if(get_quantity(INVITEM_METAL) >= 10u 
					&& get_quantity(INVITEM_WOOD) >= 10u){//se nell' inventario non ha ancora il materiale
				    missions[1].mission_state = MISSION_STATE_ACCOMPLISHED;
				    next_page = 1u;
				}else{
					whostalking = SMITH_CHECKING_NOWOODANDMETAL;
				}
			break;
			case MISSION_STATE_ACCOMPLISHED:
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
			break;
		}
        GetLocalizedDialog_EN(&n_lines);
        wait_char = MAX_WAIT_CHAR;
        writing_line = 1u;
        dialog_ready = 1u;
    }
    if(dialog_ready == 1u){
        wait_char--;
        if(wait_char == 0u){//mostra lettera successiva
            unsigned char to_print[2] = " \0";
            switch(writing_line){
                case 1u:to_print[0] = d1[counter_char];break;
                case 2u:to_print[0] = d2[counter_char];break;
                case 3u:to_print[0] = d3[counter_char];break;
                case 4u:to_print[0] = d4[counter_char];break;
                case 5u:to_print[0] = d5[counter_char];break;
                case 6u:to_print[0] = d6[counter_char];break;
                case 7u:to_print[0] = d7[counter_char];break;
            }
            PRINT(counter_char, 7+writing_line, to_print);
            wait_char = MAX_WAIT_CHAR;
            counter_char++;
            if(counter_char == 21u){
                counter_char = 0u;
                writing_line++;
                if(writing_line > n_lines){
                    dialog_ready = 2u;
                }
            }
        }
    }
    if(dialog_ready == 2u){
        dialog_cursor = SpriteManagerAdd(SpriteInvcursor,(UINT16)144u, (UINT16)120u);
        dialog_ready = 3u;
    }
    if(dialog_ready == 3u){
        if(next_page){
            next_page = 0u;
            dialog_ready = 0u;
            return;
        }
        switch(previous_state){
            case StateCemetery:
                if(whostalking == SMITH){
                    missions[1].mission_state = MISSION_STATE_ENABLED;
                }
            break;
        }
    }
}

