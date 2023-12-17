#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "SpriteManager.h"
#include "Scroll.h"

#include "custom_datas.h"


const UINT8 a_puntawater[] = {6, 0, 1, 2, 3, 3, 3}; //The first number indicates the number of frames

void START() {
	SetSpriteAnim(THIS, a_puntawater, 12u);
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
	if(THIS->anim_frame == 5){
		SpriteManagerRemoveSprite(THIS);
	}	
}

void DESTROY(){
	
}