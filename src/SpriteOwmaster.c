#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"


const UINT8 a_masterow[] = {4,0,0,0,1}; //The first number indicates the number of frames

void START(){
	THIS->lim_x = 255u;
	THIS->lim_y = 255u;
	SetSpriteAnim(THIS, a_masterow, 12u);
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE(){
}

void DESTROY(){
}