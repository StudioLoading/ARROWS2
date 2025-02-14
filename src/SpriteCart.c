#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Sound.h"

#include "custom_datas.h"

#define CART_FRMSKIP_X_MIN 1
#define CART_VX 1
#define CART_GRAVITY 1

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern MOTHERPL_STATE motherpl_state;
extern UINT8 motherpl_hit_cooldown;
extern INT8 motherpl_hp;
extern UINT8 camera_ok;
extern INT8 sfx_cooldown;

extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;

const UINT8 a_cart_idle[] = {1, 1};
const UINT8 a_cart_h[] = {3, 0,6,1}; //The first number indicates the number of frames
const UINT8 a_cart_h_hit[] = {6, 0,2,6,2,1,2}; //The first number indicates the number of frames
const UINT8 a_cart_v_d[] = {3, 3,7,8}; //The first number indicates the number of frames
const UINT8 a_cart_v_d_hit[] = {2, 3,2}; //The first number indicates the number of frames
const UINT8 a_cart_v2_d[] = {1, 4}; //The first number indicates the number of frames
const UINT8 a_cart_v2_d_hit[] = {2, 4,2}; //The first number indicates the number of frames
const UINT8 a_cart_v_u[] = {1, 5}; //The first number indicates the number of frames
const UINT8 a_cart_v_u_hit[] = {2, 5,2}; //The first number indicates the number of frames
const UINT8 a_cart_v2_u[] = {1, 5}; //The first number indicates the number of frames
const UINT8 a_cart_v2_u_hit[] = {2, 5,2}; //The first number indicates the number of frames

INT8 cart_vx = 0;
INT8 cart_vy = 0;
INT8 cart_gravity = CART_GRAVITY;
//UINT8 last_v_coll_tile = 0;
UINT8 cart_frmskip_x = 0;
UINT8 a_cart_freq = 20u;
UINT8 cart_can_jump;
UINT8 cart_frmskip_x_max = 0;
UINT8 cooldown_augment_gravity = 40u;
UINT8 cart_h_collision = 0u;
Sprite* elevator;
struct EnemyData* cart_data = 0;

void cart_behave() BANKED;
void change_cart_state(ENEMY_STATE new_cart_state) BANKED;

extern void pickup(struct ItemSpawned* pickedup_data) BANKED;

void START() {
	SetSpriteAnim(THIS, a_cart_h, 8u);
    cart_data = (struct EnemyData*) THIS->custom_data;
    cart_vx = 1;
    cart_vy = 1;
    cart_can_jump = 1;
    cart_frmskip_x_max = CART_FRMSKIP_X_MIN;
    cart_data->et_collision = 0u;
    change_cart_state(MOTHERPL_IDLE);
    motherpl_state = MOTHERPL_IDLE;
    //last_v_coll_tile = 0u;
    cooldown_augment_gravity = 40u;
    cart_h_collision = 0u;
    if(_cpu != CGB_TYPE){
        //jump_max_power = GRAVITY*9;
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void cart_turn_left() BANKED{
    if(cart_vx != -2){
        camera_ok = 0;
        cart_vx = - (CART_VX << 1);
        change_cart_state(MOTHERPL_WALK);
    }
}

void cart_turn_right() BANKED{
    if(cart_vx != CART_VX || motherpl_state != MOTHERPL_WALK){
        cart_vx = CART_VX;
        THIS->mirror = NO_MIRROR;
        change_cart_state(MOTHERPL_WALK);
    }
}

void cart_behave() BANKED{
    if(motherpl_hit_cooldown > 0){
        motherpl_hit_cooldown--;
    }
    switch(motherpl_state){
        case MOTHERPL_JUMP:
            cart_data->wait--;
            if(cart_data->wait == 0){
                SetSpriteAnim(THIS, a_cart_v_u, a_cart_freq);
            }
            cart_data->et_collision = TranslateSprite(THIS, 0, cart_vy << delta_time);
            if(cart_vy < 0){//ascending
                cart_vy+=3;
            }else{//descending
                cart_vy = 1;
            }
        break;
        case ENEMY_SLIDE_DOWN:
            switch(cart_h_collision){
                case 30u:
                case 64u:
                    THIS->y -= 2;
                    change_cart_state(MOTHERPL_WALK);
                    return;
                break;
            }
            THIS->y++;
            if(_cpu != CGB_TYPE){
                if(cart_data->wait == 0){
                    THIS->y++;
                    cart_data->wait = 2;
                }else{
                    cart_data->wait--;
                }
            }else{
                if(cart_data->wait == 0){
                    THIS->y--;
                    cart_data->wait = 80;
                }else{
                    cart_data->wait--;
                }
            }
        break;
        case MOTHERPL_WALK:
            cart_data->et_collision = TranslateSprite(THIS, 0, cart_gravity << delta_time);
            if(cart_data->et_collision == 0){
                cart_can_jump = 0;
                cart_gravity = 1+ CART_GRAVITY;
            }else{
                cart_gravity = CART_GRAVITY;
                cart_can_jump = 1;
            }
        break;
        case MOTHERPL_IDLE:
            {
                cart_can_jump = 0;
                UINT8 idle_v_coll = TranslateSprite(THIS, 0, (1+cart_gravity) << delta_time);
                switch(idle_v_coll){
                    //case 0u:
                    case 3u:
                    case 7u:
                    case 99u:
                    case 100u:
                        THIS->y++;
                    break;
                    case 22u:
                    case 64u:
                        cart_turn_right();
                    break;
                    case 93u:
                        cart_turn_left();
                        return;
                    break;
                }
            }
        break;
        case MOTHERPL_ASCENDING:
            my_play_fx(CHANNEL_2, 40, 0x4a, 0xf1, 0x73, 0x82, 0x00);
            if(THIS->y < 54u){
                cart_vx = CART_VX + CART_VX;
                struct EnemyData* elevator_data = (struct EnemyData*) elevator->custom_data;
                elevator_data->e_state = ENEMY_SLIDE_DOWN;
                cart_data->wait = 8;
                TranslateSprite(THIS, 0, 8 << delta_time);
                change_cart_state(MOTHERPL_WALK);
            }
        break;
    }
    if(cart_h_collision != 0){
        switch(cart_h_collision){
            case 30u:
            case 64u:
                change_cart_state(MOTHERPL_WALK);
            break;
            case 93u:
                cart_turn_left();
                change_cart_state(MOTHERPL_WALK);
            break;
        }
        cart_h_collision = 0;
    } else {
        switch(cart_data->et_collision){
            case 3u:
            case 7u:
                change_cart_state(MOTHERPL_IDLE);
            break;
            case 22u:
                cart_turn_right();
            break;
            case 30u:
            case 64u:
                change_cart_state(MOTHERPL_WALK);
            break;
            case 66u:
            case 67u:
            case 68u:
            case 84u:
                change_cart_state(ENEMY_SLIDE_DOWN);
            break;
            case 93u:
                cart_turn_left();
            break;
            case 94u:
                change_cart_state(MOTHERPL_ASCENDING);
            break;
            case 109u:
            case 111u:
                change_cart_state(MOTHERPL_HIT);
            break;
        }
        cart_data->et_collision = 0;
    }
}

void change_cart_state(ENEMY_STATE new_cart_state) BANKED{
    cart_data->wait=0;
    if(new_cart_state != motherpl_state){
        switch(new_cart_state){
            case MOTHERPL_JUMP:
                cart_vy = -14;
                //if(_cpu != CGB_TYPE){
                //    cart_vy = -11;
                //}
                cart_can_jump = 0;
                cart_frmskip_x = 0;
                cart_data->wait = 8u;
                if(_cpu != CGB_TYPE){
                    //cart_vx = CART_VX + CART_VX;
                }
                //SetSpriteAnim(THIS, a_cart_v_u, a_cart_freq);
            break;
            case MOTHERPL_WALK:     
                cart_can_jump = 1;
                THIS->y -= 8;
                if(motherpl_state == MOTHERPL_JUMP){
                    THIS->y += 2;
                }
                if(motherpl_state != ENEMY_SLIDE_DOWN &&
                    motherpl_state != MOTHERPL_ASCENDING){
                    my_play_fx(CHANNEL_2, 40, 0x54, 0xd2, 0xeb, 0x87, 0x00);
                    SpriteManagerAdd(SpritePuff, THIS->x + 24u, THIS->y + 6u);
                }
                if(THIS->mirror == NO_MIRROR){
                    cart_vx = CART_VX;
                }else{
                    cart_vx = -CART_VX;
                }
                SetSpriteAnim(THIS, a_cart_h, a_cart_freq);
            break;
            case ENEMY_SLIDE_DOWN:
                cart_can_jump = 1;
                cart_vx = CART_VX << 1;
                THIS->x += 2;
                THIS->y += 6;
                SetSpriteAnim(THIS, a_cart_v_d, a_cart_freq);
                cart_data->wait = 1;
            break;
            case MOTHERPL_HIT:
                my_play_fx(CHANNEL_2, 60, 0xce, 0x53, 0xb2, 0x83, 0x00);//SFX_HIT
                motherpl_hit_cooldown = 64;
                motherpl_hp--;
                cart_data->wait = 120u;
                SetSpriteAnim(THIS, a_cart_h_hit, a_cart_freq);
            break;
            case MOTHERPL_IDLE:
                cart_vx = 0;
                cart_can_jump = 0;
                SetSpriteAnim(THIS, a_cart_idle, a_cart_freq);
            break;
            case MOTHERPL_ASCENDING:
                SetSpriteAnim(THIS, a_cart_idle, a_cart_freq);
                THIS->mirror = NO_MIRROR;
                cart_vx = 0;
                cart_can_jump = 0;
                elevator = SpriteManagerAdd(SpriteCartelevator, 10u, ((UINT16) 35 << 3));
            break;
        }
        motherpl_state = new_cart_state;
    }
}

void UPDATE() {
    cart_frmskip_x++;
    UINT8 cart_x_collided = TranslateSprite(THIS, cart_vx << delta_time, 0);
    if(cart_frmskip_x >= cart_frmskip_x_max && motherpl_state != MOTHERPL_IDLE){
        // cart_x_collided = TranslateSprite(THIS, cart_vx << delta_time, 0);
        cart_frmskip_x = 0;
    }
    if(cart_h_collision != cart_x_collided){
        cart_h_collision = cart_x_collided;
    }
    if(sfx_cooldown > 0){sfx_cooldown--;}
    //BEHAVE
        cart_behave();
    //SPRITE COLLISION
        UINT8 cart_a_tile;
        Sprite* cart_s_spr;
        SPRITEMANAGER_ITERATE(cart_a_tile, cart_s_spr) {
            if(CheckCollision(THIS, cart_s_spr)) {
                switch(cart_s_spr->type){
                    case SpriteCartelevator:
                        {
                            struct EnemyData* elevator_data = (struct EnemyData*) cart_s_spr->custom_data;
                            if(elevator_data->e_state == ENEMY_SLIDE_UP){
                                THIS->x = cart_s_spr->x + 2;
                                THIS->y = cart_s_spr->y - 4;
                            }
                        }
                    break;
                    case SpriteItemspawned:
                        {   
                            struct ItemSpawned* picking_up_data = (struct ItemSpawned*) cart_s_spr->custom_data;
                            pickup(picking_up_data);
                            SpriteManagerRemoveSprite(cart_s_spr);
                        }
                    break;
                }
            }
        };
    //INPUTS
        if(THIS->y < ((UINT16) 25u << 3)){//quando è sotto non può saltare, bona
            if(cart_can_jump == 1 && KEY_TICKED(J_JUMP)){
                change_cart_state(MOTHERPL_JUMP);
                return;
            }
        }
    //hit
        //a_cart_freq = 12 | 16 | 20 | 24
}

void DESTROY(){
	
}