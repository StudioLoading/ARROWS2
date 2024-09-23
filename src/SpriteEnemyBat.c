#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Sound.h"

#include "custom_datas.h"
#include "CircleMath.h"

#define GRAVITY 1
#define MAX_WAIT 140
#define FALLING_WAIT 80
#define ATTACK_COOLDOWN 200

const UINT8 bat_anim_idle[] = {6, 6,6,6,7,6,7}; //The first number indicates the number of frames
const UINT8 bat_anim_fly[] = {9, 1, 2, 3, 4, 5, 5, 4, 3, 2}; //The first number indicates the number of frames
const UINT8 bat_anim_falling[] = {1, 2}; //The first number indicates the number of frames
const UINT8 bat_anim_hit[] = {2, 0,3}; //The first number indicates the number of frames

extern Sprite* s_motherpl;
extern MOTHERPL_STATE motherpl_state;
extern Sprite* s_blocking;
extern UINT8 motherpl_blocked;
extern UINT8 enemy_random_30_100;
extern UINT8 enemy_counter;

extern void EspawnItem(Sprite* s_enemy) BANKED;


void bat_change_state(Sprite* s_bat_arg, ENEMY_STATE e_state) BANKED;


void START(){	
    THIS->lim_x = 120u;
    THIS->lim_y = 120u;
    SetSpriteAnim(THIS, bat_anim_idle, 12u);
    struct EnemyData* bat_data = (struct EnemyData*)THIS->custom_data;
    bat_data->configured = 0u;
    bat_data->wait = 20u;
    bat_data->e_state = ENEMY_IDLE;
    bat_data->et_collision = 0u;
    bat_data->hp = 2u;
    bat_data->vx = -1;
    bat_data->x_frameskip = 0;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE(){
    enemy_random_30_100++;
    if(enemy_random_30_100 >= 100u){enemy_random_30_100 = 30u;}
    struct EnemyData* bat_data = (struct EnemyData*)THIS->custom_data;
    if(bat_data->hp <= 0){
        EspawnItem(THIS);
        SpriteManagerRemoveSprite(THIS);
        return;
    }
    //UPDATE ANIMATION WHEN RELOADED
        if(bat_data->configured == 4){
            bat_change_state(THIS, bat_data->e_state);
            bat_data->configured = 2;
        }
    bat_data->wait--;
    if(bat_data->x_frameskip == 0){bat_data->x_frameskip=1; return;}
    else{bat_data->x_frameskip = 0;}
    switch(bat_data->e_state){
        case ENEMY_SLIDE_DOWN:
            bat_data->et_collision = TranslateSprite(THIS,bat_data->vx, GRAVITY << 2);
            if(bat_data->wait == 0 || bat_data->et_collision){
                bat_change_state(THIS, ENEMY_WALK);
            }
        break;
        case ENEMY_WALK:
            bat_data->et_collision = TranslateSprite(THIS,bat_data->vx, 0);
            if(bat_data->wait == 0){
                bat_change_state(THIS, ENEMY_SLIDE_UP);
            }
        break;
        case ENEMY_SLIDE_UP:
            bat_data->et_collision = TranslateSprite(THIS,bat_data->vx, -GRAVITY << 1);
            if(bat_data->et_collision == 34u || bat_data->et_collision == 35u ||
                bat_data->et_collision == 36u || bat_data->et_collision == 37u){
                bat_change_state(THIS, ENEMY_IDLE);
            }
        break;
        case ENEMY_IDLE:
            if(bat_data->wait < 5 || bat_data->wait > MAX_WAIT){
                bat_change_state(THIS, ENEMY_SLIDE_DOWN);
            }
        break;
        case ENEMY_ATTACK:
            THIS->x = s_motherpl->x + 2u;
            THIS->y = s_motherpl->y + 4u;
            if(bat_data->wait < 10u || bat_data->wait > ATTACK_COOLDOWN){
                bat_data->wait = ATTACK_COOLDOWN;
                THIS->y -= 24u;
                bat_change_state(THIS, ENEMY_SLIDE_UP);
            }
        break;
    }
    
    //EspriteCollision(eu_info); 
        UINT8 scroll_b_tile;
        Sprite* bespr;
        SPRITEMANAGER_ITERATE(scroll_b_tile, bespr) {
            if(CheckCollision(THIS, bespr)) {
                switch(bespr->type){
                    case SpriteMotherpl:
                    case SpriteMotherplarmor://io enemy ho colpito motherpl
                        if(motherpl_blocked == 0 && bat_data->e_state != ENEMY_ATTACK){
                            bat_change_state(THIS, ENEMY_ATTACK);
                        }
                    break;
                }
            }
        };
}

void bat_change_state(Sprite* s_bat_arg, ENEMY_STATE e_state) BANKED{
    struct EnemyData* s_bat_data = (struct EnemyData*)s_bat_arg->custom_data;
    switch(e_state){
        case ENEMY_HIT_2:
            if(s_bat_data->e_state != ENEMY_IDLE){
                s_bat_data->hp-=2;
                SetSpriteAnim(s_bat_arg, bat_anim_hit, 64);
            }
        break;
        case ENEMY_HIT_1:
            if(s_bat_data->e_state != ENEMY_IDLE){
                s_bat_data->hp--;
                SetSpriteAnim(s_bat_arg, bat_anim_hit, 64);
            }
        break;
        case ENEMY_ATTACK:
            s_bat_data->wait = ATTACK_COOLDOWN;
            s_blocking = s_bat_arg;
            motherpl_blocked = 1u;
            s_bat_arg->anim_speed = 90; 
            SetSpriteAnim(s_bat_arg, bat_anim_fly, 64);
        break;
        case ENEMY_WALK:
            s_bat_data->wait = MAX_WAIT;
            SetSpriteAnim(s_bat_arg, bat_anim_fly, 16u);
            s_bat_arg->y -=1 ;
        break;
        case ENEMY_SLIDE_DOWN:
            s_bat_data->wait = FALLING_WAIT;
            SetSpriteAnim(s_bat_arg, bat_anim_falling, 1u);
            s_bat_data->vx = 1;
            s_bat_arg->mirror = NO_MIRROR;
            if(s_bat_arg->x > s_motherpl->x){
                s_bat_data->vx = -1;
                s_bat_arg->mirror = V_MIRROR;
            }
        break;
        case ENEMY_SLIDE_UP:
            s_bat_data->wait = MAX_WAIT;
            SetSpriteAnim(s_bat_arg, bat_anim_fly, 64u);
            s_bat_arg->anim_speed = 56;
        break;
        case ENEMY_IDLE:
            SetSpriteAnim(s_bat_arg, bat_anim_idle, 12u);
            s_bat_data->configured = 0;
            s_bat_data->wait = MAX_WAIT;
            s_bat_arg->y -= 3;
        break;
    }
    s_bat_data->e_state = e_state;
}

void DESTROY(){
    enemy_counter--;
}