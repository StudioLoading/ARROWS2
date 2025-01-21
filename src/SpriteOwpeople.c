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

const UINT8 a_owpeople_hidden[] = {1,0}; //The first number indicates the number of frames
const UINT8 a_bluetunicstand_idle[] = {2,1,2}; //The first number indicates the number of frames
const UINT8 a_knight_stand_idle[] = {6,12,12,12,12,13,13}; //The first number indicates the number of frames
const UINT8 a_orange_h_idle[] = {2,9,10}; //The first number indicates the number of frames
const UINT8 a_orange_h_walk[] = {4,9,10,9,11}; //The first number indicates the number of frames
const UINT8 a_yellow_u_walk[] = {2,7,8}; //The first number indicates the number of frames
const UINT8 a_yellow_d_idle[] = {2,5,6}; //The first number indicates the number of frames
const UINT8 a_yellow_d_walk[] = {4,5,4,5,3}; //The first number indicates the number of frames

INT8 owpeople_checkmotherowcoll() BANKED;

extern UINT8 colliding_owpeople;
extern UINT8 show_tip;
extern Sprite* s_motherow;

void START(){
    SetSpriteAnim(THIS, a_owpeople_hidden, 8u);
	THIS->lim_x = 1000u;
	THIS->lim_y = 1000u;
    struct OwPeopleData* owpeople_data = (struct OwPeopleData*)THIS->custom_data;
    //owpeople_data->vx = 1;
    //owpeople_data->wait = 80u;
    owpeople_data->type = OWTYPE_UNCONFIGURED;
    owpeople_data->x_frameskip = 1;
    owpeople_data->ow_state = ENEMY_IDLE;
    //owpeople_data->max_wait = 80u;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE(){
    struct OwPeopleData* owpeople_data = (struct OwPeopleData*)THIS->custom_data;
    INT8 collided_with_motherow = owpeople_checkmotherowcoll();
    if(collided_with_motherow == 0 && show_tip == 1 && owpeople_data->ow_state != ENEMY_HIDDEN && ((THIS->y - s_motherow->y) > 80u)){
        SetSpriteAnim(THIS, a_owpeople_hidden, 8u);
        owpeople_data->ow_state = ENEMY_HIDDEN;
        owpeople_data->wait = 10;
    }else if(owpeople_data->ow_state == ENEMY_HIDDEN){
        owpeople_data->ow_state = ENEMY_IDLE;
        return;
    }
    if(owpeople_data->x_frameskip == 0){
        owpeople_data->wait--;
        owpeople_data->x_frameskip++;
    }else{owpeople_data->x_frameskip--; return;}
    switch(owpeople_data->type){
        case OWTYPE_UNCONFIGURED:
            return;
        break;
        case OWTYPE_BLUETUNIC_STAND:
            switch(owpeople_data->ow_state){
                case ENEMY_IDLE:
                    SetSpriteAnim(THIS, a_bluetunicstand_idle, 8u);
                    owpeople_data->ow_state = ENEMY_WAIT;
                break;
            }
        break;
        case OWTYPE_KNIGHT_STAND:
            switch(owpeople_data->ow_state){
                case ENEMY_IDLE:
                    SetSpriteAnim(THIS, a_knight_stand_idle, 8u);
                    owpeople_data->ow_state = ENEMY_WAIT;
                break;
            }
        break;
	    case OWTYPE_ORANGE_HORIZONTAL:
            switch(owpeople_data->ow_state){
                case ENEMY_IDLE:
                    SetSpriteAnim(THIS, a_orange_h_idle, 8u);
                    owpeople_data->ow_state = ENEMY_WAIT;
                    owpeople_data->vx = 1;
                break;
                case ENEMY_WAIT:
                    if(owpeople_data->wait == 0u){
                        SetSpriteAnim(THIS, a_orange_h_walk, 8u);
                        owpeople_data->vx = -owpeople_data->vx;
                        THIS->mirror = NO_MIRROR;
                        if(owpeople_data->vx < 0){THIS->mirror = V_MIRROR;}
                        owpeople_data->wait = owpeople_data->max_wait;
                        owpeople_data->ow_state = ENEMY_WALK;
                    }
                break;
                case ENEMY_WALK:
                    if(collided_with_motherow){return;}
                    //if(owpeople_data->x_frameskip == 0){
                        UINT8 t_coll = TranslateSprite(THIS, owpeople_data->vx << delta_time, 0);
                        if(t_coll){
                            SetSpriteAnim(THIS, a_orange_h_idle, 8u);   
                            owpeople_data->ow_state = ENEMY_WAIT;
                            owpeople_data->wait = (owpeople_data->max_wait) + ((owpeople_data->max_wait) >> 1);
                        }
                        owpeople_data->x_frameskip = 3;
                    //}
                    if(owpeople_data->wait == 0){
                        SetSpriteAnim(THIS, a_orange_h_idle, 8u);
                        owpeople_data->ow_state = ENEMY_WAIT;
                        owpeople_data->wait = (owpeople_data->max_wait) + ((owpeople_data->max_wait) >> 1);
                    }
                break;
            }
        break;
        case OWTYPE_RED_VERTICAL:
            switch(owpeople_data->ow_state){
                case ENEMY_IDLE:
                    SetSpriteAnim(THIS, a_yellow_d_idle, 8u);
                    owpeople_data->ow_state = ENEMY_WAIT;
                    owpeople_data->vy = 1;
                break;
                case ENEMY_WAIT:
                    if(owpeople_data->wait == 0u){
                        SetSpriteAnim(THIS, a_yellow_d_walk, 8u);
                        owpeople_data->vy = -owpeople_data->vy;
                        if(owpeople_data->vy < 0){
                            SetSpriteAnim(THIS, a_yellow_u_walk, 8u);
                        }
                        owpeople_data->wait = owpeople_data->max_wait;
                        owpeople_data->ow_state = ENEMY_WALK;
                    }
                break;
                case ENEMY_WALK:
                    if(collided_with_motherow){return;}
                    //if(owpeople_data->x_frameskip == 0){
                        UINT8 v_coll = TranslateSprite(THIS, 0, owpeople_data->vy << delta_time);
                        if(v_coll){
                            SetSpriteAnim(THIS, a_yellow_d_idle, 8u);
                            owpeople_data->ow_state = ENEMY_WAIT;
                            owpeople_data->wait = (owpeople_data->max_wait) + ((owpeople_data->max_wait) >> 1);
                        }
                        owpeople_data->x_frameskip = 3;
                    //}
                    if(owpeople_data->wait == 0){
                        SetSpriteAnim(THIS, a_yellow_d_idle, 8u);
                        owpeople_data->ow_state = ENEMY_WAIT;
                        owpeople_data->wait = (owpeople_data->max_wait) + ((owpeople_data->max_wait) >> 1);
                    }
                break;
            }

        break;
    }
}

INT8 owpeople_checkmotherowcoll() BANKED{
    struct OwPeopleData* owpeople_data = (struct OwPeopleData*) THIS->custom_data;
    UINT8 owp_a_tile;
    Sprite* iowpspr;
    UINT8 colliding = 0u;
    SPRITEMANAGER_ITERATE(owp_a_tile, iowpspr) {
        if(CheckCollision(THIS, iowpspr)) {
            switch(iowpspr->type){
                case SpriteMotherow:
                case SpriteMotherowarmor:
                    owpeople_data->wait++;
                    colliding = 1u;
                    colliding_owpeople = 1;
                break;
            }
        }
    };
    return colliding;
}

void DESTROY(){

}