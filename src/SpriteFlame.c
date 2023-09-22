#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Sound.h"

#include "custom_datas.h"

const UINT8 animflame[] = {8, 1, 2, 3, 4, 5, 6, 7, 8}; //The first number indicates the number of frames
const UINT8 animflame2[] = {6, 2, 3, 4, 5, 7, 8}; //The first number indicates the number of frames

void START() {
	THIS->lim_x = 255u;
	THIS->lim_y = 60u;
    SetSpriteAnim(THIS, animflame2, 24u);
	if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
	
}

void DESTROY() {
}
