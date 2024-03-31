#include "Banks/SetAutoBank.h"

#include "Palette.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define MINOTAUR_GRAVITY 2
#define WALK_TIMEOUT_HPMAX 120
#define WALK_TIMEOUT_HPMED 70
#define WALK_TIMEOUT_HPMIN 60
#define RUN_VXMAX 4
#define RUN_VXMIN 2

const UINT8 a_mino_idle[] = {3, 4,8,4};
const UINT8 a_mino_hit[] = {2, 0,4};
const UINT8 a_mino_walk[] = {4, 3,4,5,6};
const UINT8 a_mino_run[] = {8, 3,4,5,6,3,8,5,6};
const UINT8 a_mino_preattack[] = {4,1,2,7,7};

struct EnemyData* minotaur_data = 0;
struct EnemyData* mino_skull_data = 0;
UINT8 mino_jump_power = 0u;
UINT8 mino_v_coll = 0u;
Sprite* mino_skull = 0;

extern MOTHERPL_STATE motherpl_state;
extern Sprite* s_motherpl;
extern UINT8 walk_timeout;

void mino_behave() BANKED;
void mino_change_state(ENEMY_STATE minotaur_new_state) BANKED;
void mino_turn() BANKED;
void mino_update_wait() BANKED;

extern void motherpl_hitted(Sprite* s_enemy) BANKED;


void START(){
    THIS->lim_x = 255u;
    THIS->lim_y = 255u;
        minotaur_data = (struct EnemyData*)THIS->custom_data;
        minotaur_data->configured = 2;
        minotaur_data->vx = 1;
        minotaur_data->hp = 6;
        minotaur_data->x_frameskip = 2;
        minotaur_data->et_collision = 0;
        mino_jump_power = 0u;
        mino_change_state(ENEMY_WALK);
    mino_v_coll = 0;
    mino_skull = SpriteManagerAdd(SpriteBossminotaurskull, THIS->x, THIS->y);
    mino_skull_data = (struct EnemyData*) mino_skull->custom_data;
    //LOAD OBP1 with 0,0,2,3
    OBP1_REG = PAL_DEF(0, 0, 1, 3);
    SPRITE_SET_PALETTE(THIS,1);
}

void UPDATE(){
    mino_behave();
    //horizontal
        if(minotaur_data->x_frameskip > 4){
            minotaur_data->x_frameskip = 2;
        }
        minotaur_data->x_frameskip--;
        if(minotaur_data->x_frameskip == 0 && (
            minotaur_data->e_state == ENEMY_WALK || minotaur_data->e_state == ENEMY_JUMP
            || minotaur_data->e_state == ENEMY_ATTACK)
        ){
            minotaur_data->et_collision = TranslateSprite(THIS, minotaur_data->vx << delta_time, 0);
        }
    //vertical: gravity & jump
        if(mino_jump_power > 0){
            mino_v_coll = TranslateSprite(THIS, 0, (MINOTAUR_GRAVITY - mino_jump_power) << delta_time);
            mino_jump_power--;
        }else{//normal gravity
            mino_v_coll = TranslateSprite(THIS, 0, MINOTAUR_GRAVITY << delta_time);
        }
    //sprite collision
        UINT8 scroll_mino_tile;
        Sprite* iminospr;
        SPRITEMANAGER_ITERATE(scroll_mino_tile, iminospr) {
            if(CheckCollision(THIS, iminospr)) {
                switch(iminospr->type){
                    case SpriteMotherpl:
                        if(minotaur_data->e_state != ENEMY_PREATTACK && 
                        minotaur_data->e_state != ENEMY_JUMP && minotaur_data->e_state != ENEMY_HIT_1 &&
                        minotaur_data->e_state != ENEMY_HIT_2){
                            motherpl_hitted(THIS);
                        }
                    break;
                }
            }
        };
}

void mino_turn() BANKED{
    minotaur_data->vx = -minotaur_data->vx;
    switch(THIS->mirror){
        case NO_MIRROR://from right to left
            THIS->x--;
            THIS->mirror = V_MIRROR;
        break;
        case V_MIRROR://from left to right
            THIS->x++;
            THIS->mirror = NO_MIRROR;
        break;
    }
    mino_skull->mirror = THIS->mirror;
}

void mino_behave() BANKED{
    minotaur_data->wait--;
    if(minotaur_data->wait > 200u){
        minotaur_data->wait = 0u;
    }
    switch(minotaur_data->e_state){
        case ENEMY_IDLE:
            mino_skull->x = THIS->x + 16u;
            mino_skull->y = THIS->y - 8u;
            if(THIS->mirror == V_MIRROR){
                mino_skull->x = THIS->x - 2u;
                mino_skull->y = THIS->y - 8u;
            }
            if(minotaur_data->wait == 0){
                mino_change_state(ENEMY_WALK);
            }
        break;
        case ENEMY_DEAD:
            THIS->mirror = H_MIRROR;
            SetSpriteAnim(THIS, a_mino_idle, 12u);
            if(minotaur_data->wait == 0u){
                //dare gli occhiali
                //dialogo finale
                //torno sull' ow e lo vedo prender il largo
            }
        break;
        case ENEMY_JUMP:
            mino_skull->x = THIS->x + 20u;
            mino_skull->y = THIS->y - 11u;
            if(THIS->mirror == V_MIRROR){
                mino_skull->x = THIS->x - 9u;
                mino_skull->y = THIS->y - 12u;
            }
            if(mino_jump_power == 0 && mino_v_coll != 0){
                mino_turn();
                mino_change_state(ENEMY_IDLE);
            }
        break;
        case ENEMY_WALK:
            mino_skull->x = THIS->x + 16u;
            mino_skull->y = THIS->y - 6u;
            if(THIS->mirror == V_MIRROR){
                mino_skull->x = THIS->x - 3u;
                mino_skull->y = THIS->y - 6u;
            }
            if(minotaur_data->wait == 0){
                if(minotaur_data->et_collision){
                       mino_turn();
                }
                if(minotaur_data->wait == 0){
                    mino_change_state(ENEMY_PREATTACK);
                }
            }
        break;
        case ENEMY_ATTACK:
            mino_skull->x = THIS->x + 16;
            mino_skull->y = THIS->y - 8u;
            if(THIS->mirror == V_MIRROR){
                mino_skull->x = THIS->x - 6u;
                mino_skull->y = THIS->y - 8u;
            }
            if(minotaur_data->et_collision){
                mino_change_state(ENEMY_JUMP);
            }
        break;
        case ENEMY_HIT_1:
        case ENEMY_HIT_2:
            if(minotaur_data->wait == 0){
                mino_update_wait();
                mino_change_state(ENEMY_IDLE);
            }
        break;
        case ENEMY_PREATTACK:
            mino_skull->x = THIS->x + 5u;
            mino_skull->y = THIS->y - 13u;
            if(THIS->mirror == V_MIRROR){
                mino_skull->x = THIS->x + 5u;
                mino_skull->y = THIS->y - 13u;
            }
            if(THIS->anim_frame == 2){
                UINT16 puff_x = THIS->x;
                Sprite* puff = SpriteManagerAdd(SpritePuff, puff_x, THIS->y + 16u);
                if(THIS->mirror == V_MIRROR){
                    puff->x = puff_x + 16u;
                }
                puff->mirror = THIS->mirror;
            }
            if(minotaur_data->wait == 0){
                mino_change_state(ENEMY_ATTACK);
            }
        break;
    }
}

void mino_change_state(ENEMY_STATE minotaur_new_state) BANKED{
    if(minotaur_data->e_state == minotaur_new_state){
        return;
    }
    switch(minotaur_new_state){
        case ENEMY_HIT_1:
        case ENEMY_HIT_2:
            minotaur_data->hp--;
            mino_jump_power = 8;
            SetSpriteAnim(THIS,a_mino_hit, 24u);
            minotaur_data->vx = 0;
            minotaur_data->wait = 80u;
        break;
        case ENEMY_IDLE:
            SetSpriteAnim(THIS,a_mino_idle, 16u);
            minotaur_data->vx = 0;
            mino_jump_power = 0;
            minotaur_data->wait = 30u;
        break;
        case ENEMY_DEAD:
            minotaur_data->e_state = minotaur_new_state;
            THIS->mirror = H_MIRROR;
            THIS->y += 4u;
            SetSpriteAnim(THIS, a_mino_idle, 10u);
            minotaur_data->vx = 0;
            minotaur_data->wait = 199u;
            SpriteManagerAdd(SpriteGlasses, 9u << 3, 6u << 3);
        break;
        case ENEMY_JUMP:
            mino_jump_power = 6u;
            minotaur_data->vx = -minotaur_data->vx;
        break;
        case ENEMY_WALK:
            SetSpriteAnim(THIS,a_mino_walk, 6u);
            mino_update_wait();
        break;
        case ENEMY_PREATTACK:
            SetSpriteAnim(THIS, a_mino_preattack, 8u);
            minotaur_data->wait = WALK_TIMEOUT_HPMAX;
        break;
        case ENEMY_ATTACK:
            SetSpriteAnim(THIS,a_mino_run, 16u);
            if(minotaur_data->hp > 3){minotaur_data->vx = RUN_VXMIN ;}
            else{minotaur_data->vx = RUN_VXMAX;}
            if(THIS->mirror == V_MIRROR){minotaur_data->vx = -minotaur_data->vx;}
        break;
    }
    if(minotaur_data->hp <= 0 && minotaur_data->e_state != ENEMY_DEAD){
        minotaur_data->hp = 0;
        mino_change_state(ENEMY_DEAD);
    }else{
        minotaur_data->e_state = minotaur_new_state;
        mino_skull_data->e_state = minotaur_new_state;
    }
}

void mino_update_wait() BANKED{
    switch(minotaur_data->hp){
        case 8:case 7:minotaur_data->wait = WALK_TIMEOUT_HPMAX;minotaur_data->vx = RUN_VXMIN - 1;minotaur_data->x_frameskip = 2;break;
        case 6:case 5:case 4:minotaur_data->wait = WALK_TIMEOUT_HPMED;minotaur_data->vx = RUN_VXMAX-RUN_VXMIN - 1;minotaur_data->x_frameskip = 1;break;
        case 3:case 2:case 1:minotaur_data->wait = WALK_TIMEOUT_HPMIN;minotaur_data->vx = RUN_VXMAX - 1;minotaur_data->x_frameskip = 1;break;
    }
    if((THIS->mirror == V_MIRROR && minotaur_data->vx > 0) 
        || (THIS->mirror == NO_MIRROR && minotaur_data->vx < 0)){
        minotaur_data->vx = -minotaur_data->vx;
    }
}

void DESTROY(){
}