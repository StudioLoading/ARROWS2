#include "Banks/SetAutoBank.h"

#include "Palette.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern MOTHERPL_STATE motherpl_state;

const UINT8 a_gull[] = {6, 4, 1, 1, 1, 2, 3}; //The first number indicates the number of frames
const UINT8 a_gull_dead[] = {6, 0, 1, 0, 2,0 , 3}; //The first number indicates the number of frames

extern void changeEstate(Sprite* s_enemy, ENEMY_STATE new_e_state) BANKED;
extern void motherpl_hitted(Sprite* s_enemy) BANKED;

void START() {
    THIS->lim_x = 200u;
    THIS->lim_y = 20u;
	SetSpriteAnim(THIS, a_gull, 8u);
    struct EnemyData* gull_data = (struct EnemyData*) THIS->custom_data;
    gull_data->vx = 0;
    gull_data->wait = 2;
    gull_data->hp = 1;
    gull_data->type = GULL;
    gull_data->e_state = ENEMY_WALK;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
    struct EnemyData* gull_data = (struct EnemyData*) THIS->custom_data;
    switch(gull_data->configured){
        case 1:
            THIS->mirror = V_MIRROR;
            gull_data->configured = 2;
        break;
        case 2:
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
                //gull_data->et_collision = TranslateSprite(THIS, gull_data->vx << delta_time, 0);
            }
            /*if(gull_data->et_collision != 0){
                THIS->y--;
            }*/
            if(gull_data->e_state == ENEMY_DEAD){
                SetSpriteAnim(THIS, a_gull_dead, 24u);
                gull_data->configured = 3;
            }
        break;
        case 3u:
            THIS->y--;
        break;
    }
    //EspriteCollision(eu_info); 
        UINT8 scroll_sg_tile;
        Sprite* isgspr;
        SPRITEMANAGER_ITERATE(scroll_sg_tile, isgspr) {
            if(CheckCollision(THIS, isgspr)) {
                switch(isgspr->type){
                    case SpriteMotherpl://io enemy ho colpito motherpl
                        if(motherpl_state == MOTHERPL_DASH){
                            switch(THIS->type){
                                case SpriteEnemysimplesnake:
                                case SpriteEnemysimplerat:
                                    changeEstate(THIS, ENEMY_HIT_1);
                                    return;
                                break;
                            }
                        } else if(gull_data->hp > 0 && gull_data->e_state != ENEMY_UPSIDEDOWN 
                            && gull_data->e_state != ENEMY_HIT_1 
                            && gull_data->e_state != ENEMY_HIT_2
                            && gull_data->e_state != ENEMY_DEAD){
                            motherpl_hitted(THIS);
                        }
                    break;
                }
            }
        };
}

void DESTROY(){
	
}
