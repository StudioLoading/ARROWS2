#include "Banks/SetAutoBank.h"

#include "Palette.h"
#include "ZGBMain.h"
#include "SpriteManager.h"
#include "Scroll.h"

#include "custom_datas.h"

//PER I SCORPIONI VERI PRENDERE IL SpriteEThrowerSpider.c come template

const UINT8 a_scor_walk[] = {5, 0, 1, 2, 3, 4}; //The first number indicates the number of frames

void START() {
	SetSpriteAnim(THIS, a_scor_walk, 12u);
    struct EnemyData* scorpion_data = (struct EnemyData*) THIS->custom_data;
    scorpion_data->vx = 0;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
    struct EnemyData* scorpion_data = (struct EnemyData*) THIS->custom_data;
    UINT8 scor_v_coll = TranslateSprite(THIS, 0, 2 << delta_time);
    if(scor_v_coll > 0){
        scorpion_data->vx = -1;
    }
    UINT8 h_coll = TranslateSprite(THIS, scorpion_data->vx << delta_time, 0);
    if(h_coll > 0){
        SpriteManagerRemoveSprite(THIS);
    }
}

void DESTROY(){
	
}
