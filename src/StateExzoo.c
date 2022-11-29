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
#include "Dialogs.h"

IMPORT_TILES(fontbw);
IMPORT_TILES(exzootiles);
IMPORT_MAP(border2);
IMPORT_MAP(exzoomap0);

extern UINT8 border_set_exzoo;
extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 coll_tiles_exzoo[] = {1, 0};
const UINT8 coll_surface_exzoo[] = {1, 0};
Sprite* s_motherpl = 0;

void START(){
	if(border_set_exzoo == 0u){
        border_set_exzoo = 1u;
        LOAD_SGB_BORDER(border2);
	}
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume
	//SCROLL LIMITS
	scroll_top_movement_limit = 56u;
	scroll_bottom_movement_limit = 80u;
    //SGB PALETTE
    if(sgb_check()){
        set_sgb_palette_2();
    }
    //INIT GRAPHICS    
    s_motherpl = SpriteManagerAdd(SpriteMotherpl, 10u, 1u);
    scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x, s_motherpl->y);
    InitScroll(BANK(exzoomap0), &exzoomap0, 0, coll_surface_exzoo);
}

void UPDATE(){

}
