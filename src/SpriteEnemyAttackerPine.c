#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 pine_anim_idle[] = {1, 1}; //The first number indicates the number of frames
const UINT8 pine_anim_walk[] = {4, 1, 2, 3, 2}; //The first number indicates the number of frames
const UINT8 pine_anim_hit[] = {2, 1, 0}; //The first number indicates the number of frames
const UINT8 pine_anim_attack[] = {2, 4, 5}; //The first number indicates the number of frames

void EattackerPineAnim(ENEMY_STATE estate) BANKED;

extern void Estart(Sprite* s_enemy) BANKED;
extern void Edestroy(Sprite* s_enemy) BANKED;
extern void configure(Sprite* s_enemy) BANKED;
extern void changeEstate(Sprite* s_enemy, ENEMY_STATE new_e_state) BANKED;
extern void Econfiguration(Sprite* s_enemy) BANKED;
extern void Emanagement(Sprite* s_enemy) BANKED;

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
        }
    //CHECK DEATH
        if(eu_info->hp <= 0){changeEstate(THIS, ENEMY_DEAD);}
    //MANAGEMENT
        Emanagement(THIS);
}

void EattackerPineAnim(ENEMY_STATE estate) BANKED{
    struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
    switch(estate){
        case ENEMY_HIT_1:
        case ENEMY_HIT_2: SetSpriteAnim(THIS, pine_anim_hit, 24u); break;
        case ENEMY_WALK: SetSpriteAnim(THIS, pine_anim_walk, 12u); break;
        case ENEMY_WAIT: SetSpriteAnim(THIS, pine_anim_idle, 16u); break;
        case ENEMY_IDLE: SetSpriteAnim(THIS, pine_anim_idle, 12u); break;
        case ENEMY_DEAD: SetSpriteAnim(THIS, pine_anim_hit, 32u); break;
        case ENEMY_PREATTACK: SetSpriteAnim(THIS, pine_anim_idle, 24u); break;
        case ENEMY_ATTACK: SetSpriteAnim(THIS, pine_anim_attack, 16u); break;
    }
}

void DESTROY(){
    Edestroy(THIS);
}