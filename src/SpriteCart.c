#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "Scroll.h"
#include "Sound.h"

#include "custom_datas.h"

#define CART_FRMSKIP_X_MIN 1
//#define CART_FRMSKIP_X_MAX 2

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern MOTHERPL_STATE motherpl_state;
extern UINT8 motherpl_hit_cooldown;
extern INT8 motherpl_hp;

extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;

const UINT8 a_cart_h[] = {3, 0,6,1}; //The first number indicates the number of frames
const UINT8 a_cart_h_hit[] = {6, 0,2,6,2,1,2}; //The first number indicates the number of frames
const UINT8 a_cart_v_d[] = {1, 3}; //The first number indicates the number of frames
const UINT8 a_cart_v_d_hit[] = {2, 3,2}; //The first number indicates the number of frames
const UINT8 a_cart_v2_d[] = {1, 4}; //The first number indicates the number of frames
const UINT8 a_cart_v2_d_hit[] = {2, 4,2}; //The first number indicates the number of frames
const UINT8 a_cart_v_u[] = {1, 5}; //The first number indicates the number of frames
const UINT8 a_cart_v_u_hit[] = {2, 5,2}; //The first number indicates the number of frames
const UINT8 a_cart_v2_u[] = {1, 5}; //The first number indicates the number of frames
const UINT8 a_cart_v2_u_hit[] = {2, 5,2}; //The first number indicates the number of frames

INT8 cart_vx = 0;
INT8 cart_vy = 0;
INT8 cart_gravity = 1;
INT8 cart_more_gravity = 0;
INT8 cart_delta_y = 0;
UINT8 last_v_coll_tile = 0;
UINT8 cart_frmskip_x = 0;
UINT8 a_cart_freq = 8u;
UINT8 cart_can_jump;
UINT8 cart_frmskip_x_max = 0;
UINT8 cart_jumping = 0;
UINT8 cooldown_augment_gravity = 40u;
/*
struct EnemyData{
	INT8 hp;
	INT8 vx;
	UINT8 wait;
	UINT8 configured;
	//0 da configurare
	//1 in configurazione
	//2 non ancora spawnato item
	//3 già spawnato item
	ENEMY_TYPE type;
	UINT8 et_collision;
	ENEMY_STATE e_state;
	UINT8 x_frameskip;
};
*/
struct EnemyData* cart_data = 0;
void cart_behave() BANKED;
void change_cart_state(ENEMY_STATE new_cart_state) BANKED;

void START() {
	SetSpriteAnim(THIS, a_cart_h, 8u);
    cart_data = (struct EnemyData*) THIS->custom_data;
    cart_vx = 1;
    cart_vy = 2;
    cart_can_jump = 1;
    cart_frmskip_x_max = CART_FRMSKIP_X_MIN;
    cart_data->et_collision = 0u;
    motherpl_state = MOTHERPL_JUMP;
    last_v_coll_tile = 0u;
    cooldown_augment_gravity = 40u;
}

void cart_turn_left() BANKED{
    if(cart_vx != -2){
        cart_vx = -2;
        THIS->mirror = V_MIRROR;
        change_cart_state(MOTHERPL_WALK);
    }
}

/*
void cart_turn_right() BANKED{
    if(cart_vx != 2 && ){
        cart_vx = 2;
        THIS->mirror = NO_MIRROR;
        change_cart_state(MOTHERPL_WALK);
    }
}
*/

void cart_behave() BANKED{
    if(motherpl_hit_cooldown > 0){
        motherpl_hit_cooldown--;
    }
    if(THIS->x > ((UINT16) 122u << 3) && cart_vx != 3){
        cart_gravity+=1;
    }
    switch(motherpl_state){
        case MOTHERPL_JUMP:
            if(cart_vy < 0){//ascending
                cart_vy++;
                if(cart_vy > -8){
                    cart_vy++;
                }
                cart_frmskip_x_max = CART_FRMSKIP_X_MIN;
            }else if(cart_can_jump == 0){//descending
                cart_vy = 0;
                //cart_delta_y = 0;
                if(cart_data->et_collision == 0){
                    cart_data->et_collision = TranslateSprite(THIS, 0, cart_gravity << delta_time);
                }
            }
        break;
        case ENEMY_SLIDE_DOWN:
            if(cart_data->vx < 0){
                //cart_turn_right();
            }
            if(cart_frmskip_x == 0){
                if(cart_data->wait == 16){THIS->y += cart_delta_y;cart_data->wait=0;}
                else{cart_data->wait++;}
                cart_more_gravity = cart_gravity + 1;
                cart_data->et_collision = TranslateSprite(THIS, 0, cart_more_gravity << delta_time);
            }
        break;
        case ENEMY_SLIDE_DOWN2:
            if(cart_data->vx < 0){
                //cart_turn_right();
            }
            if(cart_frmskip_x == 0){
                cart_more_gravity = cart_gravity + 3;
                cart_data->et_collision = TranslateSprite(THIS, 0, (cart_more_gravity) << delta_time);
            }
        break;
        case ENEMY_SLIDE_UP:
            switch(cart_data->wait){
                case 3:
                    THIS->y -= cart_delta_y;
                    cart_data->wait=0;
                break;
                case 2:
                    THIS->y -= cart_delta_y;
                    cart_more_gravity = cart_gravity - 7;
                    TranslateSprite(THIS, 0, cart_more_gravity << delta_time);
                    cart_data->wait++;
                break;
                case 1:
                    THIS->y -= cart_delta_y;
                    cart_data->wait++;
                break;
                case 0:
                    {
                        UINT8 up_v_coll = TranslateSprite(THIS, 0, 1 << delta_time);
                        if(up_v_coll == 0){
                            change_cart_state(ENEMY_IDLE);
                        }else{
                            cart_data->wait++;
                        }
                    }
                break;
            }
        break;
        case MOTHERPL_HIT:
            cart_data->wait--;
            if(cart_data->wait == 0){
                change_cart_state(MOTHERPL_WALK);
            }
        break;
    }
    if(last_v_coll_tile != cart_data->et_collision){
        last_v_coll_tile = cart_data->et_collision;        
        switch(cart_data->et_collision){
            case 3u:
            case 7u:
                change_cart_state(MOTHERPL_IDLE);
            break;
            case 21u:
                cart_turn_left();
            break;
            case 22u:
                //cart_turn_right();
            break;
            case 1u:
            case 64u:
            case 69u:
                change_cart_state(MOTHERPL_WALK);
            break;
            case 66u:
            case 67u:
            case 68u:
            case 84u:
                change_cart_state(ENEMY_SLIDE_DOWN);
            break;
            case 80u:
            case 81u:
            case 82u:
            case 83u:
                change_cart_state(ENEMY_SLIDE_DOWN2);
            break;
            case 88u:
            case 89u:
            case 90u:
            case 91u:
                change_cart_state(ENEMY_SLIDE_UP);
            break;
            case 109u:
            case 111u:
                change_cart_state(MOTHERPL_HIT);
            break;
        }
    }
}

void change_cart_state(ENEMY_STATE new_cart_state) BANKED{
    cart_data->wait=0;
    if(new_cart_state != motherpl_state){
        switch(new_cart_state){
            case MOTHERPL_JUMP:
                cart_vy = -10u;
                cart_can_jump = 0;
                cart_delta_y = 0;
                cart_frmskip_x = 0;
                cart_jumping = 1;
                SetSpriteAnim(THIS, a_cart_v_u, a_cart_freq);
            break;
            case MOTHERPL_WALK:     
                if (cart_vx != 2){cart_vx = 2;}
                cart_delta_y = 0;
                cart_can_jump = 1;
                cart_frmskip_x_max = CART_FRMSKIP_X_MIN;
                if(cart_jumping == 1){
                    cart_jumping = 0;
                }
              	SetSpriteAnim(THIS, a_cart_h, 16u);
            break;
            case ENEMY_SLIDE_DOWN:
                cart_can_jump = 1;
                cart_delta_y = +1;               
                SetSpriteAnim(THIS, a_cart_v_d, a_cart_freq);
            break;
            case ENEMY_SLIDE_DOWN2:
                cart_can_jump = 1;
                cart_delta_y = +1;               
                SetSpriteAnim(THIS, a_cart_v2_d, a_cart_freq);
            break;
            case ENEMY_SLIDE_UP:
                cart_can_jump = 1;
                cart_delta_y = 2;               
                SetSpriteAnim(THIS, a_cart_v_u, a_cart_freq);
            break;
            case MOTHERPL_HIT:
                my_play_fx(CHANNEL_1, 60, 0x7d, 0x5c, 0xf1, 0x82, 0x86);//SFX_HIT
                motherpl_hit_cooldown = 64;
                motherpl_hp--;
                cart_data->wait = 120u;
                SetSpriteAnim(THIS, a_cart_h_hit, 16u);
            break;
            case MOTHERPL_IDLE:
                cart_vx = 0;
                SetSpriteAnim(THIS, a_cart_h, 16u);
            break;
        }
        motherpl_state = new_cart_state;
    }
}

void UPDATE() {
    cart_frmskip_x++;
    if(cart_frmskip_x >= cart_frmskip_x_max && motherpl_state != MOTHERPL_IDLE){
        TranslateSprite(THIS, cart_vx << delta_time, 0);
        cart_frmskip_x = 0;
    }
    //if(motherpl_state != ENEMY_SLIDE_UP){
        cart_data->et_collision = TranslateSprite(THIS, 0, (cart_vy + cart_gravity) << delta_time);
        if(cart_data->et_collision == 0 && cooldown_augment_gravity > 0){
            cooldown_augment_gravity--;
            if(cooldown_augment_gravity == 0 && cart_vy >= 0){
                cart_vy +=1;
                cooldown_augment_gravity = 40u;
            }
        }
    //}
    //INPUTS
        if(THIS->y < ((UINT16) 25u << 3)){//quando è sotto non può saltare, bona
            if(cart_can_jump == 1 && KEY_TICKED(J_JUMP)){
                change_cart_state(MOTHERPL_JUMP);
                return;
            }
        }
    //BEHAVE
        cart_behave();
    //hit
        //a_cart_freq = 12 | 16 | 20 | 24
}

void DESTROY(){
	
}