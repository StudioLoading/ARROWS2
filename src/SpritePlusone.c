#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "SpriteManager.h"
#include "Scroll.h"

#include "custom_datas.h"


const UINT8 a_plusone[] = {1, 0}; //The first number indicates the number of frames

void START() {
	SetSpriteAnim(THIS, a_plusone, 1u);
    struct EnemyData* plusone_data = (struct EnemyData*)THIS->custom_data;
    plusone_data->wait = 80u;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
    struct EnemyData* plusone_data = (struct EnemyData*)THIS->custom_data;
    plusone_data->wait--;
    if((plusone_data->wait % 4) == 0){
        THIS->y--;
    }
    if(plusone_data->wait < 4){
        SpriteManagerRemoveSprite(THIS);
    }
}

void DESTROY(){
	
}