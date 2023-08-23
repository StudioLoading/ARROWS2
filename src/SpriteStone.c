#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern UINT8 enemy_counter;

//const UINT8 a_bigstone[] = {4 ,0,1,2,3};
const UINT8 a_stone[] = {1 ,0};

void START(){
    THIS->lim_x = 60;
    THIS->lim_y = 200;
    SetSpriteAnim(THIS, a_stone, 16u);
    struct EnemyData* stone_data = (struct EnemyData*) THIS->custom_data;
    //stone_data->hp = 1;
    stone_data->x_frameskip = 10;
    stone_data->wait = 0;
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
    struct EnemyData* stone_data = (struct EnemyData*) THIS->custom_data;
    stone_data->x_frameskip--;
    if(stone_data->x_frameskip == 0){ 
        if(stone_data->wait == 1){//se metto a 2 rimbalza una volta in più
            SpriteManagerRemoveSprite(THIS);
        }else{
            UINT8 stone_v_coll= TranslateSprite(THIS, stone_data->vx << delta_time, stone_data->hp << delta_time);
            if(stone_v_coll > 0){
                if(stone_v_coll == 64u){
                    stone_data->vx = -stone_data->vx;
                }
                stone_data->hp = -7;
                stone_data->wait++;
                THIS->y++;
            }else{
                stone_data->hp++;
            }
            stone_data->x_frameskip = 3;
        }
    }
}

void DESTROY(){
}