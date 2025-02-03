#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "TAnim0.h"
#include "custom_datas.h"

#define MAX_WAIT_CHAR 4
#define HP_INITIAL 3

IMPORT_TILES(font);
IMPORT_MAP(dmaphospital);

UINT16 get_chapter_cost() BANKED;
void manage_heal_or_death() BANKED;

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern UINT8 previous_state;
extern unsigned char EMPTY_STRING_21[];
extern unsigned char d0[];
extern WHOSTALKING whostalking;
extern struct MISSION find_antidote;
extern struct MISSION defeat_scorpions;
extern struct MISSION enable_hospital;
extern CHAPTERS chapter;

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
extern UINT16 get_quantity(INVITEMTYPE itemtype) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern void shift_text_one_line_up() BANKED;
extern void show_next_character() BANKED;
extern void play_music_missionaccomplished() BANKED;


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

UINT16 get_chapter_cost() BANKED{
    return (chapter+1) * 10u;
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
				if(chapter == CHAPTER_2_PLAGUE){
                    if(defeat_scorpions.mission_state < MISSION_STATE_REWARDED){
                        if(motherpl_hp < 1){motherpl_hp = 1;}
                        whostalking = HOSPITAL_HEAL_1;
                    }else if(find_antidote.mission_state < MISSION_STATE_REWARDED){
                        if(find_antidote.mission_state == MISSION_STATE_DISABLED){
                            find_antidote.mission_state = MISSION_STATE_STARTED;
                            SpriteManagerAdd(SpriteDiary, 72, 8);
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
                                    if(get_quantity(INVITEM_HERB) >= 8){
                                        UINT16 q_herbs = get_quantity(INVITEM_HERB);
                                        change_quantity(INVITEM_HERB, -q_herbs);
                                        whostalking = HOSPITAL_ANTIDOTE_BUILT;
                                        SpriteManagerAdd(SpriteDiary, 72, 8);
                                        find_antidote.mission_state = MISSION_STATE_ACCOMPLISHED;
                                        play_music_missionaccomplished();
                                    }//AND GO TALK TO BLACKIE
                                break;
                            }
                            if(motherpl_hp <= 0){
                                whostalking = HOSPITAL_HEAL_1;motherpl_hp = 2;
                            }else if(motherpl_hp < 2){motherpl_hp = 2;}
                        }
                    }
                }else{
                    manage_heal_or_death();
                }
            break;
            case MISSION_STATE_ACCOMPLISHED://ho silver
            case MISSION_STATE_ENABLED://ho bisogno di metallo speciale
			case MISSION_STATE_STARTED://ho bisogno di metallo speciale
                whostalking = HOSPITAL_DISABLED; 
                if(get_quantity(INVITEM_SILVER) > 0){//se in inventario ho il metallo specialo
                    enable_hospital.mission_state = MISSION_STATE_REWARDED;
                    SpriteManagerAdd(SpriteDiary, 72, 8);
                    whostalking = HOSPITAL_ENABLING;
                    motherpl_hp = 5;
                    change_quantity(INVITEM_SILVER, -1);
                }else{
                    if(motherpl_hp <= 0){
                        whostalking = HOSPITAL_GAMEOVER;
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
        if(KEY_PRESSED(J_FIRE) || KEY_PRESSED(J_JUMP) || KEY_PRESSED(J_DOWN)){
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
    if(dialog_ready == 3u && (KEY_RELEASED(J_FIRE) || KEY_RELEASED(J_JUMP))){
        if(next_page){
            next_page = 0u;
            dialog_ready = 0u;
            return;
        }
        dialog_ready = 0;
        move_on();
    }
}

void manage_heal_or_death() BANKED{
    if(motherpl_hp == 5){
        whostalking = HOSPITAL_FINE;
    }else{//in cura
        UINT16 chapter_cost = get_chapter_cost(); 
        if(get_quantity(INVITEM_MONEY) >= chapter_cost){
            change_quantity(INVITEM_MONEY, -chapter_cost);
            if(motherpl_hp > 0){ whostalking = HOSPITAL_CURE;}
            else{ whostalking = HOSPITAL_CURE_FROM_DEATH; }
            motherpl_hp = 5;
        }else if(motherpl_hp > 0){ whostalking = HOSPITAL_CANT_CURE;
        }else{
            motherpl_hp = 5;//sennò al giro dopo si impappina
            whostalking = HOSPITAL_GAMEOVER;
        }
    }
}

