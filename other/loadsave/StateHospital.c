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
#include "custom_datas.h"
#include "sgb_palette.h"

IMPORT_MAP(dialogmapbase);
IMPORT_TILES(font);

extern UINT8 previous_state;
extern Sprite* s_motherpl;
extern unsigned char EMPTY_STRING_21[];
extern UINT8 current_map;

extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
extern UINT8 save_game() BANKED;
extern UINT8 load_game() BANKED;

UINT8 loadsave_timeout = 0u;
UINT8 text_written = 0u;
UINT8 done = 0u;

void START(){
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
    text_written = 0u;
    loadsave_timeout = 0u;
    done = 0u;
}

void UPDATE(){
    if(text_written == 0u){
        PRINT(0, 7, EMPTY_STRING_21);
        PRINT(0, 8, "PRESS A TO SAVE");
        PRINT(0, 9, EMPTY_STRING_21);
        PRINT(0, 10, EMPTY_STRING_21);
        PRINT(0, 11, "PRESS B TO LOAD");
        PRINT(0, 12, EMPTY_STRING_21);
        PRINT(0, 13, EMPTY_STRING_21);
        text_written = 1u;
    }
    if(text_written == 1u){
        if(KEY_PRESSED(J_A)){
            PRINT(0, 8, "SAVING...      ");
            PRINT(0, 11, EMPTY_STRING_21);
            done = save_game();
            text_written = 2u;
        } 
        if(KEY_PRESSED(J_B)){
            PRINT(0, 8, "LOADING...      ");
            PRINT(0, 11, EMPTY_STRING_21);
            done = load_game();
            text_written = 3u;
        } 
    }
    if(text_written > 1u && text_written < 4u && done){
        switch(text_written){
            case 2u:PRINT(0, 8, "DONE SAVING!!   ");break;
            case 3u:PRINT(0, 8, "DONE LOADING!!  ");break;
        }
        text_written = 4u;
    }
    if(text_written == 4u){
        loadsave_timeout++;
        if(loadsave_timeout > 120u){
            text_written = 5u;
        }
    }
    if(text_written == 5u){
        current_map = 0u;
        ChangeState(StateExzoo, s_motherpl);
    }
}