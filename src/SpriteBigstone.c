#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern UINT8 enemy_counter;

const UINT8 a_bigstone[] = {4 ,0,1,2,3};

void START(){
    THIS->lim_x = 60;
    THIS->lim_y = 200;
    SetSpriteAnim(THIS, a_bigstone, 16u);
    struct EnemyData* bigstone_data = (struct EnemyData*) THIS->custom_data;
    bigstone_data->hp = 1;
    bigstone_data->x_frameskip = 10;
    bigstone_data->wait = 0;
    bigstone_data->vx = -2;
    enemy_counter++;
    /*    
	INT8 hp;
	INT8 vx;
	UINT8 wait;
	UINT8 configured;
	ENEMY_TYPE type;
	UINT8 et_collision;
	ENEMY_STATE e_state;
	UINT8 x_frameskip;
    */
}

void UPDATE(){
    struct EnemyData* bigstone_data = (struct EnemyData*) THIS->custom_data;
    bigstone_data->x_frameskip--;
    if(bigstone_data->x_frameskip == 0){ 
        if(bigstone_data->wait == 2){
            if(bigstone_data->vx > 0){bigstone_data->vx=1;}
            else{bigstone_data->vx =-1;}
            THIS->x = THIS->x + bigstone_data->vx;
            THIS->y = THIS->y +2;
            bigstone_data->x_frameskip = 1;
        }else{
            UINT8 bigstone_v_coll= TranslateSprite(THIS, bigstone_data->vx << delta_time, bigstone_data->hp << delta_time);
            if(bigstone_v_coll > 0){
                if(bigstone_v_coll == 64u){
                    bigstone_data->vx = -bigstone_data->vx;
                }
                bigstone_data->hp = -7;
                bigstone_data->wait++;
            }else{
                bigstone_data->hp++;
            }
            bigstone_data->x_frameskip = 3;
        }
    }
}

void DESTROY(){
    if(enemy_counter > 0){
        enemy_counter--;
    }
}