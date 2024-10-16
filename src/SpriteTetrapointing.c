#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "SpriteManager.h"
#include "Scroll.h"

#include "custom_datas_tetra.h"


const UINT8 a_tetrapoint[] = {2, 0,1}; //The first number indicates the number of frames

UINT8 tetrapointing_hp = 100u;
UINT8 pointing_configured = 0;

void START() {
    THIS->lim_x = 300;
    THIS->lim_y = 0;
	SetSpriteAnim(THIS, a_tetrapoint, 24u);
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
    tetrapointing_hp = 100;
    pointing_configured = 0;
}

void UPDATE() {
    switch(pointing_configured){
        case 0: return; break;
        case 1: //captain!
            THIS->x = (UINT16) 17 << 3;
            THIS->y = (UINT16) 1 << 3;
            THIS->mirror = H_MIRROR;
            pointing_configured = 3;
        break;
        case 2: //captain!
            THIS->x = (UINT16) 17 << 3;
            THIS->y = (UINT16) 15 << 3;
            pointing_configured = 3;
        break;
        case 3:
            tetrapointing_hp--;
            if(tetrapointing_hp < 10){
                SpriteManagerRemoveSprite(THIS);
            }
        break;
    }
}

void DESTROY(){
	pointing_configured = 0;
}