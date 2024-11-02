#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "SpriteManager.h"
#include "Scroll.h"

#include "custom_datas.h"

const UINT8 a_minosbuff[] = {3, 0,1,2}; //The first number indicates the number of frames


void START() {
    SetSpriteAnim(THIS, a_minosbuff, 12u);
    struct EnemyData* sbuff_data = (struct EnemyData*) THIS->custom_data;
    sbuff_data->hp = 4;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
    struct EnemyData* sbuff_data = (struct EnemyData*) THIS->custom_data;
    if(THIS->anim_frame == 1){
        if(sbuff_data->hp == 4){
            sbuff_data->hp = 3;
            THIS->y--;
        }
    }
    if(THIS->anim_frame == 2){
        if(sbuff_data->hp == 3){
            sbuff_data->hp = 2;
            THIS->y--;
        }
        SpriteManagerRemoveSprite(THIS);
    }
}

void DESTROY(){
	
}