#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 cobra_anim_idle[] = {1, 1}; //The first number indicates the number of frames
const UINT8 cobra_anim_walk[] = {4, 1, 2, 3, 4}; //The first number indicates the number of frames
const UINT8 cobra_anim_hit[] = {2, 1, 0}; //The first number indicates the number of frames
const UINT8 cobra_anim_attack[] = {1, 5}; //The first number indicates the number of frames

void EattackerCobraAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;

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
    //GRADINI
        switch(current_state){
            case StateBatcave:
            {
                switch(eu_info->et_collision){
                    case 54u: //GRADINI IN SALITA A SINISTRA
                        if(eu_info->e_state == ENEMY_WALK && eu_info->vx < 0){
                            THIS->y -= 8;
                            TranslateSprite(THIS, -3, 0);
                        }
                    break;
                    case 55u: //GRADINI IN SALITA A DESTRA
                        if(eu_info->e_state == ENEMY_WALK && eu_info->vx > 0){
                            THIS->y -= 8;
                            TranslateSprite(THIS, 3, 0);
                        }
                    break;
                }
            }
            break;
        }
}

void EattackerCobraAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED{
    switch(estate){
        case ENEMY_HIT_1:
        case ENEMY_HIT_2: SetSpriteAnim(s_enemy, cobra_anim_hit, 64u); break;
        case ENEMY_WALK: SetSpriteAnim(s_enemy, cobra_anim_walk, 12u); break;
        case ENEMY_WAIT: SetSpriteAnim(s_enemy, cobra_anim_idle, 16u); break;
        case ENEMY_IDLE: SetSpriteAnim(s_enemy, cobra_anim_idle, 12u); break;
        case ENEMY_DEAD: SetSpriteAnim(s_enemy, cobra_anim_hit, 64u); break;
        case ENEMY_PREATTACK: SetSpriteAnim(s_enemy, cobra_anim_idle, 24u); break;
        case ENEMY_ATTACK: SetSpriteAnim(s_enemy, cobra_anim_attack, 16u); break;
    }
}

void DESTROY(){
    Edestroy(THIS);
}