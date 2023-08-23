#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "SpriteManager.h"
#include "Scroll.h"

#include "custom_datas.h"

#define BOLT_VX_COUNTDOWN_200 40
#define BOLT_VX_COUNTDOWN_100 30
#define BOLT_VX_COUNTDOWN_50 20
#define BOLT_VX_COUNTDOWN_20 8

const UINT8 a_boltground[] = {3, 0, 1, 2}; //The first number indicates the number of frames
const UINT8 a_boltground2[] = {8, 3,4,5,5,5,5,5,5}; //The first number indicates the number of frames
const UINT8 a_boltup[] = {8, 6,7,8,8,8,8,8,8};

void START() {
    THIS->lim_x = 255;
    THIS->lim_y = 40;
    struct EnemyData* boltground_data = (struct EnemyData*) THIS->custom_data;
    boltground_data->vx = 1;
    boltground_data->hp = 4;
    boltground_data->configured = 0;
    boltground_data->wait = BOLT_VX_COUNTDOWN_200;
	SetSpriteAnim(THIS, a_boltground, 16u);
}

void UPDATE() {
    struct EnemyData* boltground_data = (struct EnemyData*) THIS->custom_data;
    boltground_data->wait--;
    if(boltground_data->wait == 0){
        boltground_data->hp--;
        switch(boltground_data->hp){
            case 3:
                boltground_data->wait = BOLT_VX_COUNTDOWN_100;
                boltground_data->vx = -boltground_data->vx;
            break;
            case 2:
                boltground_data->wait = BOLT_VX_COUNTDOWN_50;
	            SetSpriteAnim(THIS, a_boltground2, 12u);
                boltground_data->vx = 0;
                {
                    Sprite* s_boltg_up = SpriteManagerAdd(SpriteBoltground, THIS->x, THIS->y - 54u);
                    struct EnemyData* boltg_up_data = (struct EnemyData*) s_boltg_up->custom_data;
                    boltg_up_data->vx = 0;
                    boltg_up_data->configured = 1;
                    boltg_up_data->hp = 1;
                    boltg_up_data->wait = BOLT_VX_COUNTDOWN_20;
                    SetSpriteAnim(s_boltg_up, a_boltup, 12u);
                }
            break;
            case 1:
                boltground_data->wait = BOLT_VX_COUNTDOWN_20;
            break;
            case 0:
                if(boltground_data->configured == 1){
	                SpriteManagerAdd(SpriteBolt, THIS->x + 4u, THIS->y - 16u);
                }
                SpriteManagerRemoveSprite(THIS);
            break;
        }
    }
    if(boltground_data->configured == 0){
        TranslateSprite(THIS, boltground_data->vx << delta_time, 4 << delta_time);
    }else if(boltground_data->configured == 1){
        //TranslateSprite(THIS, 0, -1 << delta_time);
    }
}

void DESTROY(){
}