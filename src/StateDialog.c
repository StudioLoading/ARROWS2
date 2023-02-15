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
#include "Fade.h"
#include "Music.h"

#include "TilesAnimations0.h"
#include "custom_datas.h"
#include "sgb_palette.h"

IMPORT_TILES(font);
IMPORT_TILES(dialogtilesbase);
IMPORT_TILES(dialogtiles00);
IMPORT_MAP(dialogmapbase);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern Sprite* s_motherpl;
extern Sprite* s_motherow;
extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;

WHOSTALKING whostalking;

extern UINT8 previous_state;
UINT8 dialog_ready = 0u;

void START() {
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume 0x77
	//PlayMusic(bgm_credits, 0);
    InitScroll(BANK(dialogmapbase), &dialogmapbase, 0, 0);
    dialog_map();
    INIT_FONT(font, PRINT_BKG);
    SHOW_BKG;
}

void UPDATE() {
    if(dialog_ready == 0u){
        PRINT(1, 7, "ABC DEFGHIJ");
        PRINT(1, 8, "KLM NOPQRST");
        dialog_ready = 1u;
    }
    if(KEY_TICKED(J_START)){
        if(previous_state == StateOverworld){
            ChangeState(previous_state, s_motherow);
        }else{
            ChangeState(previous_state, s_motherpl);
        }
    }		
}
