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

#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "custom_datas.h"

IMPORT_TILES(tiles00);

IMPORT_MAP(mapflipper);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 collision_tiles_flipper[] = {1,2,3,4,5,6,7,9,0};//ends with zero

void START() {
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	//NR50_REG = 0x44; //Max volume 0x77
	//PlayMusic(bgm_credits, 0);

    if(sgb_check()){
        set_sgb_palette01_WOLF();
	}

    InitScroll(BANK(mapflipper), &mapflipper, collision_tiles_flipper, 0);
	scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 12u << 3, (UINT16) 15u << 3);
	
	SpriteManagerAdd(SpriteFlipperball, (UINT16) 12u << 3, (UINT16) 4u << 3);
	SHOW_BKG;
	
}

void UPDATE() {
	if(KEY_TICKED(J_START)){
		SetState(StateFlipper);
	}
}
