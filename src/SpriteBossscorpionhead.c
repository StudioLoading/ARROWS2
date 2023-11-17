#include "Banks/SetAutoBank.h"

#include "Palette.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"


const UINT8 a_scorpiohead_click[] = {4, 0,0,0,1};
const UINT8 a_scorpiohead_idle[] = {1, 0};
const UINT8 a_scorpiohead_hit[] = {2, 0,4};
const UINT8 a_scorpiohead_walk[] = {4, 0,0,0,1};
const UINT8 a_scorpiohead_attack[] = {7, 2,3,2,3,2,3,2};

extern MOTHERPL_STATE motherpl_state;
extern Sprite* s_motherpl;
extern Sprite* scorpio_body;
extern UINT8 walk_timeout;
extern struct EnemyData* scorpiobody_data;
struct EnemyData* scorpiohead_data = 0;

void START(){
    THIS->lim_x = 25u;
    THIS->lim_y = 25u;
    scorpiohead_data = (struct EnemyData*)THIS->custom_data;
    scorpiohead_data->configured = 2;
    scorpiohead_data->e_state = ENEMY_WALK;
    //LOAD OBP1 with 0,0,2,3
    OBP1_REG = PAL_DEF(0, 0, 1, 3);
    SPRITE_SET_PALETTE(THIS,1);
}

void UPDATE(){
    if(scorpiohead_data->e_state != scorpiobody_data->e_state){
    scorpiohead_data->e_state = scorpiobody_data->e_state;
        switch(scorpiohead_data->e_state){
            case ENEMY_HIT_1:
            case ENEMY_HIT_2:SetSpriteAnim(THIS,a_scorpiohead_hit, 24u);break;
            case ENEMY_IDLE:SetSpriteAnim(THIS,a_scorpiohead_idle, 16u);break;
            case ENEMY_DEAD:SetSpriteAnim(THIS, a_scorpiohead_hit, 24u);break;
            case ENEMY_WALK:SetSpriteAnim(THIS,a_scorpiohead_walk, 8u);break;
            case ENEMY_ATTACK:SetSpriteAnim(THIS,a_scorpiohead_attack, 4u);break;
        }
    }
    THIS->mirror = scorpio_body->mirror;
    switch(THIS->mirror){
        case NO_MIRROR:
            THIS->x = scorpio_body->x - 12;
            THIS->y = scorpio_body->y;
        break;
        case V_MIRROR:
            THIS->x = scorpio_body->x + 32;
            THIS->y = scorpio_body->y;
        break;
    }
}

void DESTROY(){
}