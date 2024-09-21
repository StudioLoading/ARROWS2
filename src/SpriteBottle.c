#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "SpriteManager.h"
#include "Scroll.h"

#include "custom_datas.h"


const UINT8 a_bottle[] = {2, 0,1}; //The first number indicates the number of frames

void START() {
    THIS->lim_x = 6000u;
    THIS->lim_y = 6000u;
	SetSpriteAnim(THIS, a_bottle, 24u);
    struct ItemSpawned* map_data = (struct ItemSpawned*) THIS->custom_data;
    map_data->itemtype = INVITEM_LIAM_HANDWRITTEN;
    map_data->quantity = 1;
    map_data->equippable = 0;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
}

void DESTROY(){
	
}