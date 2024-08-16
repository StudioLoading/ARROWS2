#include "Banks/SetAutoBank.h"

#include "Palette.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define SCORPIO_GRAVITY 2
#define WALK_TIMEOUT_HPMAX 120
#define WALK_TIMEOUT_HPMED 70
#define WALK_TIMEOUT_HPMIN 60


const UINT8 a_scorpiobody_click[] = {1, 4};
const UINT8 a_scorpiobody_idle[] = {4, 4,7,4,7};
const UINT8 a_scorpiobody_hit[] = {2, 0,4};
const UINT8 a_scorpiobody_dead[] = {1, 0};
const UINT8 a_scorpiobody_walk[] = {5, 7,3,2,1,4};
const UINT8 a_scorpiobody_attack[] = {7, 5,6,5,6,5,6,5};

extern MOTHERPL_STATE motherpl_state;
extern Sprite* s_motherpl;
extern UINT8 walk_timeout;
Sprite* scorpio_head = 0;
Sprite* scorpio_body = 0;
struct EnemyData* scorpiobody_data = 0;
extern struct EnemyData* scorpiohead_data;
UINT8 scorpio_jump_power = 0u;
UINT8 scorpio_v_coll = 0u;
UINT8 old_attacking_frame = 0u;
INT8 scorpio_hp;

void scorpio_behave() BANKED;
void scorpio_change_state(ENEMY_STATE scorpio_new_state) BANKED;
void scorpio_turn() BANKED;
void scorpio_update_wait() BANKED;

extern void motherpl_hitted(Sprite* s_enemy) BANKED;


void START(){
    THIS->lim_x = 25u;
    THIS->lim_y = 25u;
    //LOAD OBP1 with 0,0,2,3
    scorpiobody_data = (struct EnemyData*)THIS->custom_data;
    OBP1_REG = PAL_DEF(0, 0, 1, 3);
    SPRITE_SET_PALETTE(THIS,1);
}

void UPDATE(){
    if(scorpiobody_data->configured == 1){
        scorpio_v_coll = 0;
        scorpio_jump_power = 0u;
        scorpio_hp = 1;
        scorpiobody_data->configured = 2;
        scorpio_change_state(ENEMY_WALK);
        return;
    }
    //body behavior
    scorpio_behave();
    //horizontal
        if(scorpiobody_data->x_frameskip > 4){
            scorpiobody_data->x_frameskip = 3;
        }
        scorpiobody_data->x_frameskip--;
        if(scorpiobody_data->x_frameskip == 0 && (
            scorpiobody_data->e_state == ENEMY_WALK)
        ){
            scorpiobody_data->et_collision = TranslateSprite(scorpio_body, scorpiobody_data->vx << delta_time, 0);
            if(scorpiobody_data->et_collision != 0){
                scorpio_turn();
            }
        }
    //vertical: gravity & jump
        scorpio_v_coll = TranslateSprite(scorpio_body, 0, SCORPIO_GRAVITY << delta_time);
    //sprite collision
        UINT8 scroll_scorpio_tile;
        Sprite* iscorpiospr;
        SPRITEMANAGER_ITERATE(scroll_scorpio_tile, iscorpiospr) {
            if(CheckCollision(THIS, iscorpiospr)) {
                switch(iscorpiospr->type){
                    case SpriteMotherpl:
                    case SpriteMotherplarmor:
                        motherpl_hitted(THIS);
                    break;
                }
            }
        };
}

void scorpio_turn() BANKED{
    scorpiobody_data->vx = -scorpiobody_data->vx;
    switch(scorpio_body->mirror){
        case NO_MIRROR:
            scorpio_body->x--;
            scorpio_body->mirror = V_MIRROR;
        break;
        case V_MIRROR:
            scorpio_body->x++;
            scorpio_body->mirror = NO_MIRROR;
        break;
    }
}

void scorpio_behave() BANKED{
    if(scorpiohead_data->e_state != ENEMY_DEAD){
        if(scorpio_body->x < (UINT16)8u){scorpio_body->x = 10u; scorpio_turn();}
        if(scorpio_body->x > (UINT16)130u){scorpio_body->x = 126u; scorpio_turn();}
    }
    scorpiobody_data->wait--;
    if(scorpiobody_data->wait > 200u){
        scorpiobody_data->wait = 0u;
    }
    if((scorpio_body->x < 8u && scorpiobody_data->vx < 0) 
        || (scorpio_body->x > (UINT16) 136u && scorpiobody_data->vx > 0)){
        scorpio_turn();
    }
    switch(scorpiobody_data->e_state){
        case ENEMY_IDLE:
            if(scorpiobody_data->wait == 0){
                scorpio_change_state(ENEMY_WALK);
            }
        break;
        case ENEMY_DEAD:
            THIS->x++;
        break;
        case ENEMY_JUMP:
            if(scorpiobody_data->wait == 60 || 
                scorpiobody_data->wait == 90){
                scorpio_body->y-=3;
            }else if(scorpiobody_data->wait < 5){
                scorpiobody_data->wait = 60u;
                scorpio_change_state(ENEMY_IDLE);
            }
        break;
        case ENEMY_WALK:
            if(scorpiobody_data->wait == 0){
                //scorpio_change_state(ENEMY_JUMP);
                //caga scorpioncini (JUMP) oppure spara (ATTACK)
                scorpio_change_state(ENEMY_ATTACK);
            }
        break;
        case ENEMY_HIT_1:
        case ENEMY_HIT_2:
            if(scorpiobody_data->wait == 0){
                if(scorpiohead_data->hp <= 0){
                    scorpio_change_state(ENEMY_DEAD);
                    return;
                }
                scorpio_update_wait();
                if(s_motherpl->x < scorpio_body->x){scorpiobody_data->vx = -2;}
                else{scorpiobody_data->vx = 2;}
                scorpio_change_state(ENEMY_JUMP);
            }
        break;
        case ENEMY_ATTACK:
            if(scorpiobody_data->wait == 0){
                scorpio_change_state(ENEMY_IDLE);
            }else{
                if((scorpio_head->anim_frame == 1 || scorpio_head->anim_frame == 3 ||
                    scorpio_head->anim_frame == 5) &&
                    (old_attacking_frame != scorpio_head->anim_frame)){
                    old_attacking_frame = scorpio_head->anim_frame;
                    Sprite* s_web = SpriteManagerAdd(SpriteEnemythrowable, scorpio_head->x +8u, scorpio_head->y);
                    struct ThrowableData* throwable_data = (struct ThrowableData*) s_web->custom_data;
                    throwable_data->type = PROJECTILE;
                    throwable_data->configured = 1u;
                    throwable_data->vx = -2;
                    if(scorpio_head->mirror == V_MIRROR){
                        throwable_data->vx = 2;
                    }
                }
            }
        break;
    }
}

void scorpio_change_state(ENEMY_STATE scorpio_new_state) BANKED{
    if(scorpiobody_data->e_state == scorpio_new_state){
        return;
    }
    switch(scorpio_new_state){
        case ENEMY_HIT_1:
        case ENEMY_HIT_2:
            scorpiohead_data->hp--;
            scorpio_jump_power = 8;
            scorpiobody_data->vx = 0;
            scorpiobody_data->wait = 40u;
            SetSpriteAnim(scorpio_body,a_scorpiobody_hit, 24u);
        break;
        case ENEMY_IDLE:
            scorpiobody_data->vx = 0;
            scorpio_jump_power = 0;
            scorpiobody_data->wait = 30u;
            SetSpriteAnim(scorpio_body,a_scorpiobody_idle, 16u);
        break;
        case ENEMY_DEAD:
            scorpiobody_data->e_state = scorpio_new_state;
            scorpiobody_data->vx = 1;
            scorpiobody_data->wait = 199u;
            SpriteManagerAdd(SpriteScorpiontail, 9u << 3, 6u << 3);
            SetSpriteAnim(scorpio_body, a_scorpiobody_dead, 12u);
        break;
        case ENEMY_WALK:
            scorpio_update_wait();
            SetSpriteAnim(scorpio_body,a_scorpiobody_walk, 24u);
        break;
        case ENEMY_ATTACK:
            scorpio_update_wait();
            SetSpriteAnim(scorpio_body,a_scorpiobody_attack, 4u);
        break;
    }
    if(scorpiobody_data->hp <= 0 && scorpiobody_data->e_state != ENEMY_DEAD){
        scorpio_change_state(ENEMY_DEAD);
    }else{
        scorpiobody_data->e_state = scorpio_new_state;
    }
}

void scorpio_update_wait() BANKED{
    switch(scorpio_hp){
        case 6:scorpiobody_data->wait = WALK_TIMEOUT_HPMAX;scorpiobody_data->vx = 1;scorpiobody_data->x_frameskip = 3;break;
        case 5:case 4:case 3:scorpiobody_data->wait = WALK_TIMEOUT_HPMED;scorpiobody_data->vx = 1;scorpiobody_data->x_frameskip = 1;break;
        case 2:case 1:scorpiobody_data->wait = WALK_TIMEOUT_HPMIN;scorpiobody_data->vx = 3;scorpiobody_data->x_frameskip = 1;break;
    }
    if((scorpio_body->mirror == V_MIRROR && scorpiobody_data->vx < 0) 
        || (scorpio_body->mirror == NO_MIRROR && scorpiobody_data->vx > 0)){
        scorpiobody_data->vx = -scorpiobody_data->vx;
    }
}

void DESTROY(){
}