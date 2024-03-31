#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "SpriteManager.h"
#include "Scroll.h"

#include "custom_datas.h"

const UINT8 a_minoskull_skull[] = {1, 1}; //The first number indicates the number of frames
const UINT8 a_minoskull_v[] = {1, 0}; //The first number indicates the number of frames
const UINT8 a_minoskull_h[] = {1, 0};// 2}; //The first number indicates the number of frames
const UINT8 a_minoskull_h_hit[] = {1, 0}; //The first number indicates the number of frames
const UINT8 a_minoskull_hit[] = {1,0}; //The first number indicates the number of frames

extern struct EnemyData* mino_skull_data;

void START() {
    THIS->lim_x = 6000u;
    THIS->lim_y = 6000u;
	SetSpriteAnim(THIS, a_minoskull_v, 4u);
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
    switch(mino_skull_data->e_state){
        case ENEMY_TREMBLING:
            SetSpriteAnim(THIS, a_minoskull_h_hit, 16u);
        break;
        case ENEMY_PREATTACK:
            SetSpriteAnim(THIS, a_minoskull_skull, 1u);
        break;
        case ENEMY_ATTACK:
            SetSpriteAnim(THIS, a_minoskull_h, 4u);
        break;
        default:
            SetSpriteAnim(THIS, a_minoskull_v, 4u);
        break;
    }
}

void DESTROY(){
	
}