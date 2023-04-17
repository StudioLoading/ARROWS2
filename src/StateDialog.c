#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "Palette.h"
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
extern unsigned char d1[];
extern unsigned char d2[];
extern unsigned char d3[];
extern unsigned char d4[];
extern unsigned char d5[];
extern unsigned char d6[];
extern unsigned char d7[];
extern unsigned char d8[];
extern unsigned char d9[];
extern WHOSTALKING whostalking;
extern UINT8 choice;
extern UINT8 choice_left;
extern UINT8 choice_right;
extern struct MISSION missions[4];

UINT8 dialog_ready = 0u;
UINT8 counter_char = 0u;
UINT8 wait_char = MAX_WAIT_CHAR;
UINT8 writing_line = 1u;
UINT8 n_lines = 0u;
Sprite* dialog_cursor = 0;
UINT8 next_page = 0u;

void move_on() BANKED;
void shift_text_one_line_up() BANKED;

extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
extern void GetLocalizedDialog_EN(UINT8* n_lines) BANKED;
extern void dialog_map();
extern void manage_bgm(UINT8 new_state, UINT8 previous_state) BANKED;


void START() {
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume 0x77
    SpriteManagerLoad(SpriteInvcursor);
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
	SHOW_SPRITES;
}

void UPDATE() {
    if(KEY_RELEASED(J_UP)){
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
        SpriteManagerRemoveSprite(dialog_cursor);
        n_lines = 0u;
        GetLocalizedDialog_EN(&n_lines);
        wait_char = MAX_WAIT_CHAR;
        writing_line = 1u;
        dialog_ready = 1u;
    }
    if(dialog_ready == 1u){
        if(KEY_PRESSED(J_A) || KEY_PRESSED(J_B) || KEY_PRESSED(J_DOWN)){
            wait_char = 1u;
        }
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
                case 8u:to_print[0] = d8[counter_char];break;
                case 9u:to_print[0] = d9[counter_char];break;
            }
            UINT8 writing_line_on_video = writing_line;
            if(writing_line > 7){writing_line_on_video = 7;}
            PRINT(counter_char, 7+writing_line_on_video, to_print);
            wait_char = MAX_WAIT_CHAR;
            counter_char++;
            if(counter_char == 21u){
                counter_char = 0u;
                writing_line++;
                if(writing_line > 7 && writing_line < n_lines){
                    shift_text_one_line_up();
                }
                if(writing_line > n_lines){
                    dialog_ready = 2u;
                }
            }
        }
    }
    if(dialog_ready == 2u){
        dialog_cursor = SpriteManagerAdd(SpriteInvcursor,(UINT16)144u, (UINT16)120u);
        if(choice == 1u){
            dialog_cursor->x = 8u;
            dialog_cursor->y = 96u;
        }
        dialog_ready = 3u;
    }
    if(dialog_ready == 3u){
        if(choice == 0u){//NO CHOICE TO DO
            if(KEY_RELEASED(J_JUMP) || KEY_RELEASED(J_FIRE)){
                move_on();
            }
        }else{//CHOICE TO DO
            if(KEY_RELEASED(J_SELECT)){
                if(dialog_cursor->x == 8u){dialog_cursor->x = 104u;}
                else{dialog_cursor->x = 8u;}
            }
            if(KEY_RELEASED(J_START)){
                if(dialog_cursor->x == 8u){choice_left = 1u;}
                else{choice_right = 1u;}
                move_on();
            }            
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

void shift_text_one_line_up() BANKED{
    switch(writing_line){
        case 8u:
            PRINT(0, 7, d2);
            PRINT(0, 8, d3);
            PRINT(0, 9, d4);
            PRINT(0, 10, d5);
            PRINT(0, 11, d6);
            PRINT(0, 12, d7);
            PRINT(0, 13, d8);
        break;
        case 9u:
            PRINT(0, 7, d3);
            PRINT(0, 8, d4);
            PRINT(0, 9, d5);
            PRINT(0, 10, d6);
            PRINT(0, 11, d7);
            PRINT(0, 12, d8);
            PRINT(0, 13, d9);
        break;
    }
}

void move_on() BANKED{
    manage_bgm(previous_state, StateDialog);
    SpriteManagerRemoveSprite(dialog_cursor);
    if(previous_state == StateBlackiecave && whostalking == WOLF01){
        //choice_left == NO
        //choice_right == YES
        if(choice_right == 1u){
            previous_state = StateBlackieroom;
        }
        //return;//DEMO ENDS HERE
    }
	if(next_page){
		next_page = 0u;
		dialog_ready = 0u;
		return;
	}
    if(previous_state == StateOverworld){
        ChangeState(previous_state, s_motherow);
    }else{
        ChangeState(previous_state, s_motherpl);
    }
}