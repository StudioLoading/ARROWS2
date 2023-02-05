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

IMPORT_MAP(smithborder);
IMPORT_TILES(smithtiles);
IMPORT_MAP(smithmap);
DECLARE_MUSIC(bgm_credits);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern Sprite* s_motherow;
extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;


void START() {
    LOAD_SGB_BORDER(smithborder);
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume 0x77
	//PlayMusic(bgm_credits, 0);
	InitScroll(BANK(smithmap), &smithmap, 0, 0);

	SHOW_BKG;
}

void UPDATE() {
    if(KEY_TICKED(J_START)){
        ChangeState(StateOverworld, s_motherow);
    }		
}
