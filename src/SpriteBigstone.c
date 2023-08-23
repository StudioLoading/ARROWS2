#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern UINT8 enemy_counter;
extern UINT8 motherpl_hit;
extern MOTHERPL_STATE motherpl_state;

//const UINT8 a_bigstone[] = {4 ,0,1,2,3};
const UINT8 a_bigstone[] = {1 ,0};

void START(){
    THIS->lim_x = 60;
    THIS->lim_y = 200;
    SetSpriteAnim(THIS, a_bigstone, 16u);
    struct EnemyData* bigstone_data = (struct EnemyData*) THIS->custom_data;
    bigstone_data->hp = 1;
    bigstone_data->x_frameskip = 10;
    bigstone_data->wait = 0;
    bigstone_data->e_state = ENEMY_ATTACK;
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
                SpriteManagerRemoveSprite(THIS);
                /*bigstone_data->hp = -7;
                bigstone_data->wait++;
                THIS->y++;*/
            }else{
                bigstone_data->hp++;
            }
            bigstone_data->x_frameskip = 3;
        }
    }
     //SPRITE COLLISION
        UINT8 bigs_a_tile;
        Sprite* bigs_spr;
        SPRITEMANAGER_ITERATE(bigs_a_tile, bigs_spr) {
            if(CheckCollision(THIS, bigs_spr)) {
                switch(bigs_spr->type){
                    case SpriteMotherpl:
                        if(motherpl_state != MOTHERPL_DASH && motherpl_state != MOTHERPL_HIT
                            && motherpl_hit == 0){
                            motherpl_hit = 1;
                        }
                    case SpriteArrow:
                        SpriteManagerRemoveSprite(THIS);
                    break;
                }
            }
        };
}

void DESTROY(){
    if(enemy_counter > 0){
        enemy_counter--;
    }
    Sprite* s_stone_1 = SpriteManagerAdd(SpriteStone, THIS->x + 10u, THIS->y);
    struct EnemyData* stone1_data = (struct EnemyData*) s_stone_1->custom_data;
    stone1_data->hp = -5;
    stone1_data->vx = -2;
    Sprite* s_stone_2 = SpriteManagerAdd(SpriteStone, THIS->x + 10u, THIS->y);
    struct EnemyData* stone2_data = (struct EnemyData*) s_stone_2->custom_data;
    stone2_data->hp = -5;
    stone2_data->vx = 2;
    
}