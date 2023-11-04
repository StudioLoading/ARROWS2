#include "Banks/SetAutoBank.h"

#include "Palette.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define CRAB_GRAVITY 2
#define WALK_TIMEOUT_HPMAX 120
#define WALK_TIMEOUT_HPMED 70
#define WALK_TIMEOUT_HPMIN 60

const UINT8 a_crab_click[] = {8, 0,1,0,0,2,0,0,3};
const UINT8 a_crab_idle[] = {8, 4,5,5,4,5,5,5,5};
const UINT8 a_crab_hit[] = {2, 4,5};
const UINT8 a_crab_walk[] = {3, 7,8,6};

extern MOTHERPL_STATE motherpl_state;
extern Sprite* s_motherpl;
extern UINT8 motherpl_hit;

UINT8 walk_timeout = 0u;
struct EnemyData* crab_data = 0;
UINT8 crab_jump_power = 0u;
UINT8 crab_v_coll = 0u;


void crab_behave() BANKED;
void crab_change_state(ENEMY_STATE crab_new_state) BANKED;
void crab_turn() BANKED;
void crab_update_wait() BANKED;

void START(){
    THIS->lim_x = 255u;
    THIS->lim_y = 255u;
        crab_data = (struct EnemyData*)THIS->custom_data;
        crab_data->configured = 2;
        crab_data->vx = 1;
        crab_data->hp = 6;//6;
        crab_data->x_frameskip = 2;
        crab_jump_power = 0u;
        crab_change_state(ENEMY_IDLE);
    crab_v_coll = 0;
    //LOAD OBP1 with 0,0,2,3
    OBP1_REG = PAL_DEF(0, 0, 1, 3);
    SPRITE_SET_PALETTE(THIS,1);
}

void UPDATE(){
    crab_behave();
    //horizontal
        if(crab_data->x_frameskip > 4){
            crab_data->x_frameskip = 3;
        }
        crab_data->x_frameskip--;
        if(crab_data->x_frameskip == 0 && (
            crab_data->e_state == ENEMY_WALK || crab_data->e_state == ENEMY_JUMP)
        ){
            crab_data->et_collision = TranslateSprite(THIS, crab_data->vx << delta_time, 0);
        }
    //vertical: gravity & jump
        if(crab_jump_power > 0){
            crab_v_coll = TranslateSprite(THIS, 0, (CRAB_GRAVITY - crab_jump_power) << delta_time);
            crab_jump_power--;
        }else{//normal gravity
            crab_v_coll = TranslateSprite(THIS, 0, CRAB_GRAVITY << delta_time);
        }
    //sprite collision
        UINT8 scroll_crab_tile;
        Sprite* icrabspr;
        SPRITEMANAGER_ITERATE(scroll_crab_tile, icrabspr) {
            if(CheckCollision(THIS, icrabspr)) {
                switch(icrabspr->type){
                    case SpriteMotherpl:
                        if((crab_data->e_state == ENEMY_WALK || 
                        crab_data->e_state == ENEMY_JUMP && crab_jump_power == 0)
                        && motherpl_hit == 0){
                            motherpl_hit = 1;
                        }
                    break;
                }
            }
        };
}

void crab_turn() BANKED{
    crab_data->vx = -crab_data->vx;
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

void crab_behave() BANKED{
    if(THIS->x < (UINT16)8u){THIS->x = 10u; crab_turn();}
    if(THIS->x > (UINT16)130u){THIS->x = 126u; crab_turn();}
    crab_data->wait--;
    if(crab_data->wait > 200u){
        crab_data->wait = 0u;
    }
    if((THIS->x < 8u && crab_data->vx < 0) 
        || (THIS->x > (UINT16) 136u && crab_data->vx > 0)){
        crab_turn();
    }
    switch(crab_data->e_state){
        case ENEMY_IDLE:
            if(crab_data->wait == 0){
                crab_change_state(ENEMY_WALK);
            }
        break;
        case ENEMY_DEAD:
            THIS->mirror = H_MIRROR;
            SetSpriteAnim(THIS, a_crab_idle, 12u);
            if(crab_data->wait == 0u){
                //dare gli occhiali
                //dialogo finale
                //torno sull' ow e lo vedo prender il largo
            }
        break;
        case ENEMY_JUMP:
            if(crab_jump_power == 0 && crab_v_coll != 0){
                crab_change_state(ENEMY_IDLE);
            }
        break;
        case ENEMY_WALK:
            if(crab_data->wait == 0){
                crab_change_state(ENEMY_JUMP);
            }
        break;
        case ENEMY_HIT_1:
        case ENEMY_HIT_2:
            if(crab_data->wait == 0){
                crab_update_wait();
                if(s_motherpl->x < THIS->x){crab_data->vx = -2;}
                else{crab_data->vx = 2;}
                crab_change_state(ENEMY_JUMP);
            }
        break;
    }
}

void crab_change_state(ENEMY_STATE crab_new_state) BANKED{
    if(crab_data->e_state == crab_new_state){
        return;
    }
    switch(crab_new_state){
        case ENEMY_HIT_1:
        case ENEMY_HIT_2:
            crab_data->hp--;
            crab_jump_power = 8;
            SetSpriteAnim(THIS,a_crab_hit, 24u);
            crab_data->vx = 0;
            crab_data->wait = 80u;
        break;
        case ENEMY_IDLE:
            SetSpriteAnim(THIS,a_crab_idle, 16u);
            crab_data->vx = 0;
            crab_jump_power = 0;
            crab_data->wait = 30u;
        break;
        case ENEMY_DEAD:
            crab_data->e_state = crab_new_state;
            THIS->mirror = H_MIRROR;
            THIS->y += 4u;
            SetSpriteAnim(THIS, a_crab_idle, 10u);
            crab_data->vx = 0;
            crab_data->wait = 199u;
            SpriteManagerAdd(SpriteGlasses, 9u << 3, 6u << 3);
        break;
        case ENEMY_JUMP:
            SetSpriteAnim(THIS,a_crab_click, 16u);
            crab_jump_power = 12u;
        break;
        case ENEMY_WALK:
            SetSpriteAnim(THIS,a_crab_walk, 16u);
            crab_update_wait();
        break;
    }
    if(crab_data->hp <= 0 && crab_data->e_state != ENEMY_DEAD){
        crab_change_state(ENEMY_DEAD);
    }else{
        crab_data->e_state = crab_new_state;
    }
}

void crab_update_wait() BANKED{
    switch(crab_data->hp){
        case 8:case 7:crab_data->wait = WALK_TIMEOUT_HPMAX;crab_data->vx = 1;crab_data->x_frameskip = 3;break;
        case 6:case 5:case 4:crab_data->wait = WALK_TIMEOUT_HPMED;crab_data->vx = 2;crab_data->x_frameskip = 2;break;
        case 3:case 2:case 1:crab_data->wait = WALK_TIMEOUT_HPMIN;crab_data->vx = 3;crab_data->x_frameskip = 1;break;
    }
    if((THIS->mirror == V_MIRROR && crab_data->vx > 0) 
        || (THIS->mirror == NO_MIRROR && crab_data->vx < 0)){
        crab_data->vx = -crab_data->vx;
    }
}

void DESTROY(){
}