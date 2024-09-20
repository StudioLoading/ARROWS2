#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Scroll.h"

#include "custom_datas.h"

const UINT8 rat_anim_idle[] = {1, 1}; //The first number indicates the number of frames
const UINT8 rat_anim_walk[] = {3, 2, 1, 3}; //The first number indicates the number of frames
const UINT8 rat_anim_hit[] = {2, 1, 0}; //The first number indicates the number of frames

void EsimpleRatAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;

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
    struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
    //configuration
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
    //HIDE IN HOLES
        if(eu_info->vx > 0){
            if(GetScrollTile((THIS->x >> 3) + 1u, (THIS->y >> 3) + 1u) == 48u){
                changeEstate(THIS, ENEMY_DEAD);
            }
        }else{
            
            if(GetScrollTile((THIS->x >> 3), (THIS->y >> 3) + 1u) == 48u){
                changeEstate(THIS, ENEMY_DEAD);
            }
        }
}

void EsimpleRatAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED{
    switch(estate){
        case ENEMY_HIT_1:
        case ENEMY_HIT_2: SetSpriteAnim(s_enemy, rat_anim_hit, 24u); break;
        case ENEMY_WALK: SetSpriteAnim(s_enemy, rat_anim_walk, 12u); break;
        case ENEMY_WAIT: SetSpriteAnim(s_enemy, rat_anim_idle, 16u); break;
        case ENEMY_IDLE: SetSpriteAnim(s_enemy, rat_anim_idle, 12u); break;
        case ENEMY_DEAD: SetSpriteAnim(s_enemy, rat_anim_hit, 24u); break;
    }
}

void DESTROY(){
    Edestroy(THIS);
}