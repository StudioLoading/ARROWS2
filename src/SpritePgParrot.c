#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "SpriteManager.h"
#include "Scroll.h"

#include "custom_datas.h"


const UINT8 a_parrot[] = {1, 0}; //The first number indicates the number of frames

void START() {
    THIS->lim_x = 1000;
    THIS->lim_y = 1000;
	SetSpriteAnim(THIS, a_parrot, 12u);
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
}

void DESTROY(){
	
}