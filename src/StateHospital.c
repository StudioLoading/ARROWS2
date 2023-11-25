#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "TilesAnimations0.h"
#include "custom_datas.h"

#define MAX_WAIT_CHAR 4

IMPORT_TILES(font);
IMPORT_MAP(dmaphospital);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern Sprite* s_motherpl;
extern Sprite* s_motherow;
extern UINT8 previous_state;
extern unsigned char EMPTY_STRING_21[];
extern unsigned char d0[];
extern WHOSTALKING whostalking;
extern struct MISSION find_antidote;
extern struct MISSION defeat_scorpions;
extern struct MISSION enable_hospital;
extern INT8 chapter;

extern UINT8 dialog_ready;
extern UINT8 counter_char;
extern UINT8 wait_char;
extern UINT8 writing_line;
extern UINT8 n_lines;
extern Sprite* dialog_cursor;
extern UINT8 next_page;
extern INT8 motherpl_hp;

extern void move_on() BANKED;

extern void GetLocalizedDialog_EN(UINT8* n_lines) BANKED;
extern UINT8 get_quantity(INVITEMTYPE itemtype) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern void shift_text_one_line_up() BANKED;
extern void show_next_character() BANKED;

void START() {
    HIDE_WIN;
    InitScroll(BANK(dmaphospital), &dmaphospital, 0, 0);
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
		switch(enable_hospital.mission_state){
            case MISSION_STATE_REWARDED:// la curo e la rispedisco in overworld
				if(chapter == 2){
                    if(defeat_scorpions.mission_state < MISSION_STATE_REWARDED){
                        if(motherpl_hp < 1){motherpl_hp = 1;}
                        whostalking = HOSPITAL_HEAL_1;
                    }else if(find_antidote.mission_state < MISSION_STATE_REWARDED){
                        if(find_antidote.mission_state == MISSION_STATE_DISABLED){
                            find_antidote.mission_state = MISSION_STATE_STARTED;
                            SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
                        }else{
                            switch(find_antidote.phase){
                                case 0:
                                case 1:
                                    whostalking = HOSPITAL_GET_ANTIDOTE;
                                break;
                                case 2:
                                    whostalking = HOSPITAL_GET_ANTIDOTE;
                                    if(get_quantity(INVITEM_SCORPIONTAIL) > 0){
                                        change_quantity(INVITEM_SCORPIONTAIL, -1);
                                        find_antidote.phase = 3;
                                        whostalking = HOSPITAL_THANKS_FOR_TAIL;
                                    }
                                break;
                                case 3:
                                    whostalking = HOSPITAL_THANKS_FOR_TAIL;
                                break;
                                case 4:
                                    whostalking = HOSPITAL_GO_FOR_HERBS;
                                    if(get_quantity(INVITEM_HERB) == 8){
                                        whostalking = HOSPITAL_ANTIDOTE_BUILT;
                                        SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
                                        find_antidote.mission_state = MISSION_STATE_ACCOMPLISHED;
                                    }//AND GO TALK TO BLACKIE
                                break;
                            }
                            if(motherpl_hp <= 0){
                                whostalking = HOSPITAL_HEAL_1;motherpl_hp = 2;
                            }else if(motherpl_hp < 2){motherpl_hp = 2;}
                        }
                    }
                }else{
                    if(motherpl_hp <= 0){//morta
                        whostalking = HOSPITAL_CURE_FROM_DEATH;
                    }else if(motherpl_hp == 5){
                        whostalking = HOSPITAL_FINE;
                    }else{//in cura
                        whostalking = HOSPITAL_CURE;
                    }
                    motherpl_hp = 5;
                }
            break;
            case MISSION_STATE_ACCOMPLISHED:
                enable_hospital.mission_state = MISSION_STATE_REWARDED;
			    SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
			case MISSION_STATE_ENABLED://ho bisogno di metallo speciale
                motherpl_hp = 2;
				whostalking = HOSPITAL_DISABLED; 
                if(get_quantity(INVITEM_METAL_SPECIAL) > 0){//se in inventario ho il metallo specialo
                    enable_hospital.mission_state = MISSION_STATE_REWARDED;
			        SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
                    whostalking = HOSPITAL_ENABLING;
                    motherpl_hp = 5;
                    change_quantity(INVITEM_METAL_SPECIAL, -1);
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
        struct ItemSpawned* dialog_cursor_data = (struct ItemSpawned*)dialog_cursor->custom_data;
        dialog_cursor_data->configured = 2;
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

