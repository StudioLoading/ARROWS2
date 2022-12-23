#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "BankManager.h"
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

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "DiaryDialogs.h"

IMPORT_TILES(fontbw);
DECLARE_MUSIC(bgm_credits);
IMPORT_MAP(border);
IMPORT_MAP(inventorymap);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

extern UINT8 border_set_diary;
extern UINT8 border_set_ow;
extern UINT8 current_map;//0=south-west, 1=south-east, 2=north-west, 3=north-east
extern unsigned char dd1[];
extern unsigned char dd2[];
extern unsigned char dd3[];
extern unsigned char dd4[];
extern unsigned char dd5[];
extern unsigned char dd6[];
extern unsigned char dd7[];
extern unsigned char dd8[];

const UINT8 collision_tiles_inv[] = {1, 2, 0};
Sprite* inv_cursor = 0;

void START(){
	/*if(border_set_diary == 0u){
        border_set_diary = 1u;
        LOAD_SGB_BORDER(border);        
	}*/
    LOAD_SGB_BORDER(border);        
    HIDE_WIN;

	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume
	//PlayMusic(bgm_credits, 0);
    
    //SGB palette
    if(sgb_check()){
        set_sgb_palette_2();
    }
    //scroll_target = 
    inv_cursor = SpriteManagerAdd(SpriteInvcursor, 8u, 24u);
	InitScroll(BANK(inventorymap), &inventorymap, collision_tiles_inv, 0);
    scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 10u << 3, (UINT16) 9u << 3);
	INIT_FONT(fontbw, PRINT_BKG);
    SHOW_BKG;
}


void UPDATE(){
    if(KEY_PRESSED(J_START)){
        SetState(StateExzoo);
    }
}