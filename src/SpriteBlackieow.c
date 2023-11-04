#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 blackieow_anim[] = {4, 0, 1, 0, 0};
const UINT8 a_blackieow_walk[] = {3, 2,3,4};
struct EnemyData* blackieow_data;


void START(){
    SetSpriteAnim(THIS,blackieow_anim, 8u);
    THIS->lim_x = 255u;
    THIS->lim_y = 255u;
    blackieow_data = (struct EnemyData*)THIS->custom_data;
    blackieow_data->vx = 0;
    blackieow_data->wait = 0;
}

void UPDATE(){
    if(blackieow_data->wait > 0){blackieow_data->wait--; return;}
	if(blackieow_data->vx != 0){
        SetSpriteAnim(THIS,a_blackieow_walk, 16u);//anim running
        THIS->x+=blackieow_data->vx;
    }
}

void DESTROY(){
}