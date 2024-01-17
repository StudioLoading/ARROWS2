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

const UINT8 a_mino_idle[] = {3, 1,2,1};
const UINT8 a_mino_hit[] = {2, 0,1};
const UINT8 a_mino_walk[] = {5, 3,1,3,4,2};

extern MOTHERPL_STATE motherpl_state;
extern Sprite* s_motherpl;

extern UINT8 walk_timeout;
struct EnemyData* minotaur_data = 0;
UINT8 mino_jump_power = 0u;
UINT8 mino_v_coll = 0u;


void mino_behave() BANKED;
void mino_change_state(ENEMY_STATE crab_new_state) BANKED;
void mino_turn() BANKED;
void mino_update_wait() BANKED;

extern void motherpl_hitted(Sprite* s_enemy) BANKED;

void START(){
    THIS->lim_x = 255u;
    THIS->lim_y = 255u;
        minotaur_data = (struct EnemyData*)THIS->custom_data;
        minotaur_data->configured = 2;
        minotaur_data->vx = 1;
        minotaur_data->hp = 6;//6;
        minotaur_data->x_frameskip = 2;
        mino_jump_power = 0u;
        mino_change_state(ENEMY_IDLE);
    mino_v_coll = 0;
    //LOAD OBP1 with 0,0,2,3
    OBP1_REG = PAL_DEF(0, 0, 1, 3);
    SPRITE_SET_PALETTE(THIS,1);
}

void UPDATE(){
    mino_behave();
    //horizontal
        if(minotaur_data->x_frameskip > 4){
            minotaur_data->x_frameskip = 3;
        }
        minotaur_data->x_frameskip--;
        if(minotaur_data->x_frameskip == 0 && (
            minotaur_data->e_state == ENEMY_WALK || minotaur_data->e_state == ENEMY_JUMP)
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
                        if(minotaur_data->e_state == ENEMY_WALK || 
                        minotaur_data->e_state == ENEMY_JUMP && mino_jump_power == 0){
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
        case NO_MIRROR:
            THIS->x--;
            THIS->mirror = V_MIRROR;
        break;
        case V_MIRROR:
            THIS->x++;
            THIS->mirror = NO_MIRROR;
        break;
    }
}

void mino_behave() BANKED{
    if(THIS->x < (UINT16)8u){THIS->x = 10u; mino_turn();}
    if(THIS->x > (UINT16)130u){THIS->x = 126u; mino_turn();}
    minotaur_data->wait--;
    if(minotaur_data->wait > 200u){
        minotaur_data->wait = 0u;
    }
    if((THIS->x < 8u && minotaur_data->vx < 0) 
        || (THIS->x > (UINT16) 136u && minotaur_data->vx > 0)){
        mino_turn();
    }
    switch(minotaur_data->e_state){
        case ENEMY_IDLE:
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
            if(mino_jump_power == 0 && mino_v_coll != 0){
                mino_change_state(ENEMY_IDLE);
            }
        break;
        case ENEMY_WALK:
            if(minotaur_data->wait == 0){
                mino_change_state(ENEMY_JUMP);
            }
        break;
        case ENEMY_HIT_1:
        case ENEMY_HIT_2:
            if(minotaur_data->wait == 0){
                mino_update_wait();
                if(s_motherpl->x < THIS->x){minotaur_data->vx = -2;}
                else{minotaur_data->vx = 2;}
                mino_change_state(ENEMY_JUMP);
            }
        break;
    }
}

void mino_change_state(ENEMY_STATE crab_new_state) BANKED{
    if(minotaur_data->e_state == crab_new_state){
        return;
    }
    switch(crab_new_state){
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
            minotaur_data->e_state = crab_new_state;
            THIS->mirror = H_MIRROR;
            THIS->y += 4u;
            SetSpriteAnim(THIS, a_mino_idle, 10u);
            minotaur_data->vx = 0;
            minotaur_data->wait = 199u;
            SpriteManagerAdd(SpriteGlasses, 9u << 3, 6u << 3);
        break;
        case ENEMY_JUMP:
            //SetSpriteAnim(THIS,a_crab_click, 16u);
            mino_jump_power = 12u;
        break;
        case ENEMY_WALK:
            SetSpriteAnim(THIS,a_mino_walk, 16u);
            mino_update_wait();
        break;
    }
    if(minotaur_data->hp <= 0 && minotaur_data->e_state != ENEMY_DEAD){
        mino_change_state(ENEMY_DEAD);
    }else{
        minotaur_data->e_state = crab_new_state;
    }
}

void mino_update_wait() BANKED{
    switch(minotaur_data->hp){
        case 8:case 7:minotaur_data->wait = WALK_TIMEOUT_HPMAX;minotaur_data->vx = 1;minotaur_data->x_frameskip = 3;break;
        case 6:case 5:case 4:minotaur_data->wait = WALK_TIMEOUT_HPMED;minotaur_data->vx = 2;minotaur_data->x_frameskip = 2;break;
        case 3:case 2:case 1:minotaur_data->wait = WALK_TIMEOUT_HPMIN;minotaur_data->vx = 3;minotaur_data->x_frameskip = 1;break;
    }
    if((THIS->mirror == V_MIRROR && minotaur_data->vx > 0) 
        || (THIS->mirror == NO_MIRROR && minotaur_data->vx < 0)){
        minotaur_data->vx = -minotaur_data->vx;
    }
}

void DESTROY(){
}