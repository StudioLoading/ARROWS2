#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"
#include "Dialogs.h"

const UINT8 a_owpusha[] = {2,0,1}; //The first number indicates the number of frames
extern INT8 ow_pusha_hp;
extern struct PushASignData d_push_sign;

void START(){
	THIS->lim_x = 10u;
	THIS->lim_y = 10u;
    SetSpriteAnim(THIS, a_owpusha, 12u);
}

void UPDATE(){
    if(ow_pusha_hp > 0){
        ow_pusha_hp--;
    }else{
        ow_pusha_hp = 0;
        d_push_sign.collided_tile = 0;
        SpriteManagerRemoveSprite(THIS);
    }
}

void DESTROY(){
    d_push_sign.collided_tile = 0;
    ow_pusha_hp = 0u;
}