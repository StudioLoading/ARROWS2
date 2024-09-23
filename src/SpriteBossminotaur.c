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
#define RUN_VXMAX 5
#define RUN_VXMIN 2

const UINT8 a_mino_idle[] = {1, 3};
const UINT8 a_mino_hit[] = {2, 7,0};
const UINT8 a_mino_walk[] = {7, 6,8,3,5,7,2,1};
const UINT8 a_mino_run[] = {7, 6,8,3,5,7,2,1};
const UINT8 a_mino_preattack[] = {2,3,4};
const UINT8 a_mino_dead[] = {1,9};

struct EnemyData* minotaur_data = 0;
struct EnemyData* mino_skull_data = 0;
UINT8 mino_jump_power = 0u;
UINT8 mino_v_coll = 0u;
Sprite* mino_skull = 0;

extern MOTHERPL_STATE motherpl_state;
extern Sprite* s_motherpl;
extern UINT8 walk_timeout;
extern struct MISSION golden_armor;

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
    if(golden_armor.phase < 2){
        minotaur_data->vx = 1;
        minotaur_data->hp = 6;
        minotaur_data->x_frameskip = 2;
        minotaur_data->et_collision = 0;
        mino_change_state(ENEMY_WALK);
    }else{
        SetSpriteAnim(THIS, a_mino_dead, 1u);
        mino_change_state(ENEMY_DEAD);
    }
    mino_v_coll = 0;
    mino_jump_power = 0u;
    mino_skull = SpriteManagerAdd(SpriteBossminotaurskull, THIS->x, THIS->y);
    mino_skull_data = (struct EnemyData*) mino_skull->custom_data;
    //LOAD OBP1 with 0,0,2,3
        if(_cpu != CGB_TYPE){
            OBP1_REG = PAL_DEF(0, 0, 1, 3);
            SPRITE_SET_PALETTE(THIS,1);
        }
}

void UPDATE(){
    mino_behave();
    if(minotaur_data->hp <= 0 && minotaur_data->e_state != ENEMY_DEAD){
        mino_change_state(ENEMY_DEAD);
        return;
    }
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
                    case SpriteMotherplarmor:
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
    if(THIS->x < 16u){THIS->x = 16u;}
    if(THIS->x > 152u){THIS->x = 152u;}
    minotaur_data->wait--;
    if(minotaur_data->wait > 200u){
        minotaur_data->wait = 0u;
    }
    switch(minotaur_data->e_state){
        case ENEMY_IDLE:
            mino_skull->x = THIS->x + 24u;
            mino_skull->y = THIS->y - 2u;
            if(THIS->mirror == V_MIRROR){
                mino_skull->x = THIS->x - 2u;
                mino_skull->y = THIS->y - 2u;
            }
            if(minotaur_data->wait == 0){
                mino_change_state(ENEMY_WALK);
            }
        break;
        case ENEMY_DEAD:
            minotaur_data->wait--;
            if(minotaur_data->wait == 0u){
                //dare gli occhiali
                //dialogo finale
                //torno sull' ow e lo vedo prender il largo
            }
        break;
        case ENEMY_JUMP:
            mino_skull->x = THIS->x + 24u;
            mino_skull->y = THIS->y - 2u;
            if(THIS->mirror == V_MIRROR){
                mino_skull->x = THIS->x - 2u;
                mino_skull->y = THIS->y - 2u;
            }
            if(mino_jump_power == 0 && mino_v_coll != 0){
                mino_turn();
                mino_change_state(ENEMY_IDLE);
            }
        break;
        case ENEMY_WALK:
            mino_skull->x = THIS->x + 24u;
            mino_skull->y = THIS->y - 2u;
            if(THIS->mirror == V_MIRROR){
                mino_skull->x = THIS->x - 2u;
                mino_skull->y = THIS->y - 2u;
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
            mino_skull->x = THIS->x + 24;
            mino_skull->y = THIS->y - 2u;
            if(THIS->mirror == V_MIRROR){
                mino_skull->x = THIS->x - 2u;
                mino_skull->y = THIS->y - 2u;
            }
            switch(minotaur_data->wait){
                case 0: case 240: case 210:
                case 180: case 150: case 120:
                if(minotaur_data->vx < 0){
                    SpriteManagerAdd(SpritePuff, THIS->x, THIS->y + 14u);
                }else{
                    SpriteManagerAdd(SpritePuff, THIS->x, THIS->y + 14u);
                }
            }
            switch(minotaur_data->wait){
                case 90: case 60: case 30: case 5:
                if(minotaur_data->vx < 0){
                    SpriteManagerAdd(SpritePuff, THIS->x, THIS->y + 14u);
                }else{
                    SpriteManagerAdd(SpritePuff, THIS->x, THIS->y + 14u);
                }
            }         
            if(minotaur_data->et_collision){
                mino_change_state(ENEMY_JUMP);
            }
        break;
        case ENEMY_HIT_1:
        case ENEMY_HIT_2:
            mino_skull->x = THIS->x + 24u;
            mino_skull->y = THIS->y - 2u;
            if(THIS->mirror == V_MIRROR){
                mino_skull->x = THIS->x - 2u;
                mino_skull->y = THIS->y - 2u;
            }
            if(minotaur_data->wait == 0){
                mino_update_wait();
                mino_change_state(ENEMY_IDLE);
            }
        break;
        case ENEMY_PREATTACK:
            mino_skull->x = THIS->x + 16u;
            mino_skull->y = THIS->y - 2u;
            if(THIS->mirror == V_MIRROR){
                mino_skull->x = THIS->x - 4u;
                mino_skull->y = THIS->y - 2u;
            }
            if(minotaur_data->wait == 70 || minotaur_data->wait == 10){
                if(THIS->mirror == NO_MIRROR){
                    SpriteManagerAdd(SpriteBossminotaursbuff, THIS->x + 24u, THIS->y + 12u);
                }else{
                    Sprite* s_minosbuff = SpriteManagerAdd(SpriteBossminotaursbuff, THIS->x - 6u, THIS->y + 12u);
                    s_minosbuff->mirror = THIS->mirror;
                }
            } 
            if(minotaur_data->wait == 100 || minotaur_data->wait == 80 || 
            minotaur_data->wait == 60 || minotaur_data->wait == 40 || minotaur_data->wait == 20 ||
            minotaur_data->wait == 5){
                UINT16 puff_x = THIS->x + 4u;
                Sprite* s_puff = SpriteManagerAdd(SpritePuff, puff_x, THIS->y + 16u);
                s_puff->mirror = THIS->mirror;
                if(s_puff->mirror == V_MIRROR){
                    s_puff->x = THIS->x - 4u;
                }
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
            mino_jump_power = 7;
            SetSpriteAnim(THIS,a_mino_hit, 20u);
            //minotaur_data->vx = 0;
            minotaur_data->wait = 120u;
        break;
        case ENEMY_IDLE:
            SetSpriteAnim(THIS,a_mino_idle, 16u);
            minotaur_data->vx = 0;
            mino_jump_power = 0;
            minotaur_data->wait = 30u;
        break;
        case ENEMY_DEAD:
            minotaur_data->hp = 0;
            THIS->y -= 4u;
            minotaur_data->vx = 0;
            minotaur_data->wait = 199u;
            SetSpriteAnim(THIS, a_mino_dead, 1u);
            if(golden_armor.mission_state < MISSION_STATE_ACCOMPLISHED){
                SpriteManagerAdd(SpriteSkull, THIS->x, THIS->y);
            }
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
            SetSpriteAnim(THIS, a_mino_preattack, 4u);
            minotaur_data->wait = WALK_TIMEOUT_HPMAX;
            switch(minotaur_data->hp){
                case 2:
                case 1:
                    SetSpriteAnim(THIS, a_mino_preattack, 16u);
                    minotaur_data->wait = minotaur_data->wait >> 1;
                break;
            }
        break;
        case ENEMY_ATTACK:
            minotaur_data->wait = 190u;
            UINT8 run_rate = 12u;
            if(minotaur_data->hp < 3){
                run_rate = 16u;
                if(minotaur_data->hp == 1){
                    run_rate = 20u;
                }
            }
            SetSpriteAnim(THIS,a_mino_run, run_rate);
            if(minotaur_data->hp > 3){minotaur_data->vx = RUN_VXMIN ;}
            else{minotaur_data->vx = RUN_VXMAX;}
            if(THIS->mirror == V_MIRROR){minotaur_data->vx = -minotaur_data->vx;}
        break;
    }
    minotaur_data->e_state = minotaur_new_state;
    mino_skull_data->e_state = minotaur_new_state;
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