#include "Banks/SetAutoBank.h"

#include "Palette.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern MOTHERPL_STATE motherpl_state;

const UINT8 a_gullh[] = {6, 4, 1, 1, 1, 2, 3}; //The first number indicates the number of frames
const UINT8 a_gullh_dead[] = {6, 0, 1, 0, 2,0 , 3}; //The first number indicates the number of frames

extern UINT8 enemy_counter;

void START() {
    THIS->lim_x = 100u;
    THIS->lim_y = 100u;
	SetSpriteAnim(THIS, a_gullh, 8u);
    struct EnemyData* gull_data = (struct EnemyData*) THIS->custom_data;
    gull_data->vx = 0;
    gull_data->wait = 2;
    gull_data->hp = 1;
    gull_data->type = GULL;
    gull_data->e_state = ENEMY_WALK;
    enemy_counter++;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
    struct EnemyData* gull_data = (struct EnemyData*) THIS->custom_data;
    switch(gull_data->configured){
        case 1:
            if(gull_data->vx < 0){
                THIS->mirror = V_MIRROR;
            }else{THIS->mirror = NO_MIRROR;}
            gull_data->configured = 2;
        break;
        case 2:
            if(gull_data->hp <= 0 && gull_data->e_state != ENEMY_DEAD){
                gull_data->e_state = ENEMY_DEAD;
                return;
            }
            if(gull_data->wait > 4){
                if(THIS->y > 64u){
                    THIS->x--;
                    THIS->y--;
                }else{
                    gull_data->wait--;
                    if(gull_data->wait > 34){
                        if(gull_data->wait & 0b100){
                            gull_data->et_collision = TranslateSprite(THIS, gull_data->vx << delta_time, 1);
                        }else{
                            gull_data->et_collision = TranslateSprite(THIS, gull_data->vx << delta_time, 0);
                        }
                    }else{
                        if(gull_data->wait & 0b100){
                            gull_data->et_collision = TranslateSprite(THIS, gull_data->vx << delta_time, -1);
                        }else{
                            gull_data->et_collision = TranslateSprite(THIS, gull_data->vx << delta_time, 0);
                        }
                    }
                }
            }else{
                gull_data->wait = 64;
            }
        break;
        case 3:
            if(gull_data->e_state == ENEMY_DEAD){
                SetSpriteAnim(THIS, a_gullh_dead, 24u);
                THIS->mirror = V_MIRROR;
                THIS->y-=2;
            }
        break;
    }    
}

void DESTROY(){
	enemy_counter--;
}
