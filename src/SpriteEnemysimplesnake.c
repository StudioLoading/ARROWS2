#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 snake_anim_idle[] = {1, 1}; //The first number indicates the number of frames
const UINT8 snake_anim_walk[] = {4, 3, 4, 1, 2}; //The first number indicates the number of frames
const UINT8 snake_anim_hit[] = {2, 0, 1}; //The first number indicates the number of frames

void EsimpleSnakeAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;

extern void Estart(Sprite* s_enemy) BANKED;
extern void configure(Sprite* s_enemy) BANKED;
extern void changeEstate(Sprite* s_enemy, ENEMY_STATE new_e_state) BANKED;
extern void Econfiguration(Sprite* s_enemy) BANKED;
extern void Emanagement(Sprite* s_enemy) BANKED;
extern void Edestroy(Sprite* s_enemy) BANKED;

void START(){
    Estart(THIS);
}

void UPDATE(){
    //configuration
        struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
        if(eu_info->configured == 0){
            return;
        }
        if(eu_info->configured == 1){
            Econfiguration(THIS);
            return;
        }
    //CHECK DEATH
        if(eu_info->hp <= 0){changeEstate(THIS, ENEMY_DEAD);}
    //MANAGEMENT
        Emanagement(THIS);
}

void EsimpleSnakeAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED{
    switch(estate){
        case ENEMY_HIT_1:
        case ENEMY_HIT_2: SetSpriteAnim(s_enemy, snake_anim_hit, 64u); break;
        case ENEMY_WALK: SetSpriteAnim(s_enemy, snake_anim_walk, 12u); break;
        case ENEMY_WAIT: SetSpriteAnim(s_enemy, snake_anim_idle, 16u); break;
        case ENEMY_IDLE: SetSpriteAnim(s_enemy, snake_anim_idle, 12u); break;
        case ENEMY_DEAD: SetSpriteAnim(s_enemy, snake_anim_hit, 64u); break;
    }
}

void DESTROY(){
    Edestroy(THIS);
}