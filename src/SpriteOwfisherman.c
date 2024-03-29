#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "Sprite.h"
#include "SpriteManager.h"

const UINT8 a_owfisherman_h[] = {7,0,0,0,0,1,0,0,1}; //The first number indicates the number of frames
const UINT8 a_owfisherman_v[] = {7,2,2,2,2,3,2,2,3}; //The first number indicates the number of frames

void START(){
	THIS->lim_x = 6500u;
	THIS->lim_y = 6500u;
    SetSpriteAnim(THIS, a_owfisherman_h, 12u);
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE(){
}

void DESTROY(){
}