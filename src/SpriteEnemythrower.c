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

void EthrowerAnim(ENEMY_STATE estate) BANKED;
void Ethrow(ENEMY_STATE estate) BANKED;

extern void Estart() BANKED;
extern void configure() BANKED;
extern void changeEstate(ENEMY_STATE new_e_state) BANKED;
extern void Econfiguration() BANKED;
extern void Emanagement() BANKED;

void START(){
    Estart();
}

void UPDATE(){
    //configuration
        struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
        if(eu_info->configured == 0){
            return;
        }
        if(eu_info->configured == 1){
            Econfiguration();
        }
    //CHECK DEATH
        if(eu_info->hp <= 0){changeEstate(ENEMY_DEAD);}
    //MANAGEMENT
        Emanagement();
}

void EthrowerAnim( ENEMY_STATE estate) BANKED{
    struct EnemyData* eu_info = (struct EnemyData*)THIS->custom_data;
    switch(estate){
        case ENEMY_HIT:
            SetSpriteAnim(THIS, spider_anim_hit, 16u);
        break;
        case ENEMY_WALK:
            SetSpriteAnim(THIS, spider_anim_walk, 16u);
        break;
        case ENEMY_WAIT:
            SetSpriteAnim(THIS, spider_anim_wait, 2u);
        break;
        case ENEMY_IDLE:
            SetSpriteAnim(THIS, spider_anim_idle, 20u);
        break;
        case ENEMY_DEAD:
            SetSpriteAnim(THIS, spider_anim_hit, 32u);
        break;
        case ENEMY_PREATTACK:
            SetSpriteAnim(THIS, spider_anim_preattack, 16u);
        break;
        case ENEMY_THROW:
            SetSpriteAnim(THIS, spider_anim_idle, 16u);
        break;
    }
}

void Ethrow(ENEMY_STATE estate) BANKED{
    struct EnemyData* eu_info = (struct EnemyData*)THIS->custom_data;
    eu_info->wait = enemy_random_30_100;
    EthrowerAnim(estate);
    Sprite* s_web = SpriteManagerAdd(SpriteEnemythrowable, THIS->x +8u, THIS->y - 8u);
    struct ThrowableData* throwable_data = (struct ThrowableData*) s_web->custom_data;
    if(eu_info->type == SPIDER){throwable_data->type = WEB;}
    if(eu_info->type == TARANTULA){throwable_data->type = ACID;}
    throwable_data->configured = 1u;
}

void DESTROY(){
}