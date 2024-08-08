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
#define MAX_WAIT 160
#define ATTACK_COOLDOWN 160

const UINT8 bat_anim_idle[] = {6, 6,6,6,7,6,7}; //The first number indicates the number of frames
const UINT8 bat_anim_fly[] = {9, 1, 2, 3, 4, 5, 5, 4, 3, 2}; //The first number indicates the number of frames
const UINT8 bat_anim_falling[] = {1, 2}; //The first number indicates the number of frames

extern Sprite* s_motherpl;
extern MOTHERPL_STATE motherpl_state;
extern Sprite* s_blocking;
extern UINT8 motherpl_blocked;

extern void motherpl_hitted(Sprite* s_enemy) BANKED;

void bat_change_state(Sprite* s_bat, ENEMY_STATE e_state) BANKED;


void START(){	
    THIS->lim_x = 80u;
    THIS->lim_y = 80u;
    SetSpriteAnim(THIS, bat_anim_idle, 12u);
    struct EnemyData* bat_data = (struct EnemyData*)THIS->custom_data;
    bat_data->configured = 0u;
    bat_data->wait = MAX_WAIT;
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
    struct EnemyData* bat_data = (struct EnemyData*)THIS->custom_data;
    if(bat_data->hp <= 0){
        SpriteManagerRemoveSprite(THIS);
        return;
    }
    bat_data->wait--;
    if(bat_data->x_frameskip == 0){bat_data->x_frameskip=1; return;}
    else{bat_data->x_frameskip = 0;}
    switch(bat_data->e_state){
        case ENEMY_RUN:
        case ENEMY_WALK:
            {
            UINT8 vy = vy = (-GRAVITY * 2);
            if(bat_data->configured == 0){//se sto salendo, devo salire indip dal wait fino a collision
                if(bat_data->wait > (MAX_WAIT - MAX_WAIT/4)){vy = GRAVITY * 3;}
                else if(bat_data->wait > (MAX_WAIT/2 + MAX_WAIT/8)){vy = GRAVITY;
                    SetSpriteAnim(THIS, bat_anim_fly, 32u);}
                else if(bat_data->wait > (MAX_WAIT/2)){vy = 0;}
                else if(bat_data->wait > (MAX_WAIT/2 - MAX_WAIT/8)){vy = 0;}
                else if(bat_data->wait > MAX_WAIT/4){vy = -GRAVITY;
                    THIS->anim_speed = 90u;}
                else if(bat_data->wait > 0){
                    bat_data->configured = 1;}
            }
            
            bat_data->et_collision = TranslateSprite(THIS,bat_data->vx, vy);
            if(bat_data->et_collision == 34u || bat_data->et_collision == 35u ||
                bat_data->et_collision == 36u || bat_data->et_collision == 37u){
                bat_change_state(THIS, ENEMY_IDLE);
            }else if(bat_data->et_collision){
                SetSpriteAnim(THIS, bat_anim_fly, 32u);
            }
            }
        break;
        case ENEMY_IDLE:
            if(bat_data->wait < 5 || bat_data->wait > MAX_WAIT){
                bat_change_state(THIS, ENEMY_WALK);
            }
        break;
        case ENEMY_ATTACK:
            THIS->x = s_motherpl->x + 2u;
            THIS->y = s_motherpl->y + 4u;
            if(bat_data->wait < 10u){
                bat_data->wait = ATTACK_COOLDOWN;
                motherpl_hitted(THIS);
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
                        if(motherpl_blocked == 0 && bat_data->e_state != ENEMY_ATTACK
                            && bat_data->e_state != ENEMY_RUN){
                            bat_change_state(THIS, ENEMY_ATTACK);
                        }
                    break;
                }
            }
        };
}

void bat_change_state(Sprite* s_bat, ENEMY_STATE e_state) BANKED{
    struct EnemyData* s_bat_data = (struct EnemyData*)s_bat->custom_data;
    switch(e_state){
        case ENEMY_HIT_2:
            if(s_bat_data->e_state != ENEMY_IDLE){
                s_bat_data->hp-=2;
            }
            return;
        break;
        case ENEMY_HIT_1:
            if(s_bat_data->e_state != ENEMY_IDLE){
                s_bat_data->hp--;
            }
            return;
        break;
        case ENEMY_ATTACK:
            s_bat_data->wait = ATTACK_COOLDOWN;
            s_blocking = s_bat;
            motherpl_blocked = 1u;
            s_bat->anim_speed = 90; 
            SetSpriteAnim(s_bat, bat_anim_fly, 64);
        break;
        case ENEMY_RUN:
        case ENEMY_WALK:
            s_bat_data->wait = MAX_WAIT;
            SetSpriteAnim(s_bat, bat_anim_falling, 32u);
            s_bat_data->vx = 1;
            s_bat->mirror = NO_MIRROR;
            if(s_bat->x > s_motherpl->x){
                s_bat_data->vx = -1;
                s_bat->mirror = V_MIRROR;
            }
            s_bat->y += 2;
        break;
        case ENEMY_IDLE:
            SetSpriteAnim(s_bat, bat_anim_idle, 12u);
            s_bat_data->configured = 0;
            s_bat_data->wait = MAX_WAIT;
            s_bat->y -= 3;
        break;
    }
    s_bat_data->e_state = e_state;
}

void DESTROY(){
}