#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 pine_anim_idle[] = {1, 1}; //The first number indicates the number of frames
const UINT8 pine_anim_walk[] = {4, 1, 2, 3, 2}; //The first number indicates the number of frames
const UINT8 pine_anim_hit[] = {2, 1, 0}; //The first number indicates the number of frames
const UINT8 pine_anim_attack[] = {2, 4, 5};
const UINT8 cobra_anim_idle[] = {1, 6}; //The first number indicates the number of frames
const UINT8 cobra_anim_walk[] = {4, 6, 7, 8, 9}; //The first number indicates the number of frames
const UINT8 cobra_anim_hit[] = {2, 6, 0}; //The first number indicates the number of frames
const UINT8 cobra_anim_attack[] = {1, 10}; //The first number indicates the number of frames
extern UINT8 enemy_random_30_100;

void EattackerAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;

extern void Estart(Sprite* s_enemy) BANKED;
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

void EattackerAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED{
    struct EnemyData* eu_info = (struct EnemyData*) s_enemy->custom_data;
    switch(estate){
        case ENEMY_HIT:
            if(eu_info->type == COBRA){(THIS, cobra_anim_hit, 24u);}
            if(eu_info->type == PORCUPINE){(THIS, pine_anim_hit, 24u);}
        break;
        case ENEMY_WALK:        
            if(eu_info->type == PORCUPINE){SetSpriteAnim(s_enemy, pine_anim_walk, 12u);}
            if(eu_info->type == COBRA){SetSpriteAnim(s_enemy, cobra_anim_walk, 12u);}
        break;
        case ENEMY_WAIT:
            if(eu_info->type == PORCUPINE){SetSpriteAnim(s_enemy, pine_anim_idle, 16u);}
            if(eu_info->type == COBRA){SetSpriteAnim(s_enemy, cobra_anim_idle, 16u);}
        break;
        case ENEMY_IDLE:
            if(eu_info->type == COBRA){SetSpriteAnim(s_enemy, cobra_anim_idle, 12u);}
            if(eu_info->type == PORCUPINE){SetSpriteAnim(s_enemy, pine_anim_idle, 12u);}
        break;
        case ENEMY_DEAD:
            if(eu_info->type == PORCUPINE){SetSpriteAnim(s_enemy, pine_anim_hit, 32u);}
            if(eu_info->type == COBRA){SetSpriteAnim(s_enemy, cobra_anim_hit, 32u);}
        break;
        case ENEMY_PREATTACK:
            if(eu_info->type == COBRA){SetSpriteAnim(s_enemy, cobra_anim_idle, 24u);}
            if(eu_info->type == PORCUPINE){SetSpriteAnim(s_enemy, pine_anim_idle, 24u);}
        break;
        case ENEMY_ATTACK:
            if(eu_info->type == COBRA){SetSpriteAnim(s_enemy, cobra_anim_attack, 16u);}
            if(eu_info->type == PORCUPINE){SetSpriteAnim(s_enemy, pine_anim_attack, 16u);}
        break;
    }
}

void DESTROY(){
}