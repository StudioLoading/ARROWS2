#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Palette.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Sound.h"

#include "custom_datas.h"

const UINT8 scorpion_anim_wait[] = {4, 1,1,1,6};
const UINT8 scorpion_anim_idle[] = {4, 1,1,1,6}; //The first number indicates the number of frames
const UINT8 scorpion_anim_walk[] = {6, 1,2,3,4,5,6};//The first number indicates the number of frames
const UINT8 scorpion_anim_hit[] = {2, 1, 0};//{2, 1, 3}; //The first number indicates the number of frames
const UINT8 scorpion_anim_preattack[] = {2, 5, 6};//{2, 1, 3}; //The first number indicates the number of frames
const UINT8 scorpion_anim_attack[] = {1, 7};//{2, 1, 3}; //The first number indicates the number of frames

void EthrowerScorpionAnim(ENEMY_STATE estate) BANKED;
void EthrowProjectile(Sprite* s_enemy, ENEMY_STATE estate) BANKED;

extern void Estart(Sprite* s_enemy) BANKED;
extern void Edestroy(Sprite* s_enemy) BANKED;
extern void configure(Sprite* s_enemy) BANKED;
extern void changeEstate(Sprite* s_enemy, ENEMY_STATE new_e_state) BANKED;
extern void Econfiguration(Sprite* s_enemy) BANKED;
extern void Emanagement(Sprite* s_enemy) BANKED;
extern void enemy_death() BANKED;
extern void ETurn(Sprite* s_enemy, UINT8 e_vx);

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
    //SCORPION LIMITS
        INT8 scroll_distance = scroll_target->x - THIS->x;
        if(scroll_distance > 70){
            THIS->x = scroll_target->x - 70u;
        }else if(scroll_distance < -80){
            THIS->x = scroll_target->x + 80u;
        }

}

void EthrowerScorpionAnim(ENEMY_STATE estate) BANKED{
    struct EnemyData* eu_info = (struct EnemyData*)THIS->custom_data;
    switch(estate){
        case ENEMY_HIT_1:
        case ENEMY_HIT_2: 
            SetSpriteAnim(THIS, scorpion_anim_hit, 32u);
        break;
        case ENEMY_WALK: SetSpriteAnim(THIS, scorpion_anim_walk, 24u); break;
        case ENEMY_WAIT: SetSpriteAnim(THIS, scorpion_anim_wait, 16u); break;
        case ENEMY_IDLE: SetSpriteAnim(THIS, scorpion_anim_idle, 20u); break;
        case ENEMY_DEAD: SetSpriteAnim(THIS, scorpion_anim_hit, 32u); break;
        case ENEMY_PREATTACK: SetSpriteAnim(THIS, scorpion_anim_preattack, 16u); break;
        case ENEMY_THROW: SetSpriteAnim(THIS, scorpion_anim_attack, 16u); break;
    }
}

void EthrowProjectile(Sprite* s_enemy, ENEMY_STATE estate) BANKED{
    struct EnemyData* eu_info = (struct EnemyData*)s_enemy->custom_data;
    EthrowerScorpionAnim(estate);
    Sprite* s_projectile = SpriteManagerAdd(SpriteEnemythrowable, s_enemy->x +8u, s_enemy->y - 7u);
    struct ThrowableData* throwable_data = (struct ThrowableData*) s_projectile->custom_data;
    throwable_data->type = PROJECTILE;
    throwable_data->vx = 2;
    if(s_enemy->mirror == V_MIRROR){
        throwable_data->vx = -2; 
    }
    throwable_data->configured = 1u;
}

void DESTROY(){
    struct EnemyData* eu_info = (struct EnemyData*)THIS->custom_data;
    if(eu_info->e_state == ENEMY_DEAD){
        enemy_death();
        Edestroy(THIS);
    }
}