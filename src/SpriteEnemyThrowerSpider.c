#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 spider_anim_wait[] = {1, 2};
const UINT8 spider_anim_idle[] = {3, 1, 1, 2}; //The first number indicates the number of frames
const UINT8 spider_anim_walk[] = {8, 1, 2, 6, 3, 1, 2, 5, 4};//The first number indicates the number of frames
const UINT8 spider_anim_hit[] = {2, 1, 0};//{2, 1, 3}; //The first number indicates the number of frames
const UINT8 spider_anim_preattack[] = {2, 5, 6};//{2, 1, 3}; //The first number indicates the number of frames

extern UINT8 enemy_random_30_100;

void EthrowerSpiderAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;
void EthrowWeb(Sprite* s_enemy, ENEMY_STATE estate) BANKED;

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
            return;
        }
    //CHECK DEATH
        if(eu_info->hp <= 0){changeEstate(THIS, ENEMY_DEAD);}
    //MANAGEMENT
        Emanagement(THIS);
}

void EthrowerSpiderAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED{
    switch(estate){
        case ENEMY_HIT_1: 
        case ENEMY_HIT_2: SetSpriteAnim(s_enemy, spider_anim_hit, 64u); break;
        case ENEMY_WALK: SetSpriteAnim(s_enemy, spider_anim_walk, 16u); break;
        case ENEMY_WAIT: SetSpriteAnim(s_enemy, spider_anim_wait, 2u); break;
        case ENEMY_IDLE: SetSpriteAnim(s_enemy, spider_anim_idle, 20u); break;
        case ENEMY_DEAD: SetSpriteAnim(s_enemy, spider_anim_hit, 64u); break;
        case ENEMY_PREATTACK: SetSpriteAnim(s_enemy, spider_anim_preattack, 16u); break;
        case ENEMY_THROW: SetSpriteAnim(s_enemy, spider_anim_idle, 16u); break;
    }
}

void EthrowWeb(Sprite* s_enemy, ENEMY_STATE estate) BANKED{
    struct EnemyData* eu_info = (struct EnemyData*)s_enemy->custom_data;
    eu_info->wait = enemy_random_30_100;
    EthrowerSpiderAnim(s_enemy, estate);
    Sprite* s_web = SpriteManagerAdd(SpriteEnemythrowable, s_enemy->x +8u, s_enemy->y - 8u);
    struct ThrowableData* throwable_data = (struct ThrowableData*) s_web->custom_data;
    throwable_data->type = WEB;
    throwable_data->configured = 1u;
}

void DESTROY(){
    Edestroy(THIS);
}