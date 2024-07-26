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
const UINT8 a_knight_h_walk[] = {4,9,10,9,11}; //The first number indicates the number of frames
const UINT8 a_yellow_u_walk[] = {2,7,8}; //The first number indicates the number of frames
const UINT8 a_yellow_d_idle[] = {2,5,6}; //The first number indicates the number of frames
const UINT8 a_yellow_d_walk[] = {4,5,4,5,3}; //The first number indicates the number of frames

INT8 owpeople_checkmotherowcoll() BANKED;

extern UINT8 colliding_owpeople;

void START(){
    SetSpriteAnim(THIS, a_owpeople_hidden, 8u);
	THIS->lim_x = 6500u;
	THIS->lim_y = 6500u;
    struct OwPeopleData* owpeople_data = (struct OwPeopleData*)THIS->custom_data;
    owpeople_data->vx = 1;
    owpeople_data->wait = 80u;
    owpeople_data->type = OWTYPE_UNCONFIGURED;
    owpeople_data->x_frameskip = 1;
    owpeople_data->ow_state = ENEMY_IDLE;
    owpeople_data->max_wait = 80u;
}

void UPDATE(){
    INT8 collided_with_motherow = owpeople_checkmotherowcoll();
    struct OwPeopleData* owpeople_data = (struct OwPeopleData*)THIS->custom_data;
    if(owpeople_data->x_frameskip == 0){
        owpeople_data->wait--;
        owpeople_data->x_frameskip++;
    }else{owpeople_data->x_frameskip--;}
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
                case ENEMY_WAIT:
                    //DO NOTHING
                break;
            }
        break;
        case OWTYPE_KNIGHT_STAND:
            switch(owpeople_data->ow_state){
                case ENEMY_IDLE:
                    SetSpriteAnim(THIS, a_knight_stand_idle, 8u);
                    owpeople_data->ow_state = ENEMY_WAIT;
                break;
                case ENEMY_WAIT:
                    //DO NOTHING
                break;
            }
        break;
	    case OWTYPE_KNIGHT_HORIZONTAL:
            switch(owpeople_data->ow_state){
                case ENEMY_IDLE:
                    SetSpriteAnim(THIS, a_knight_stand_idle, 8u);
                    owpeople_data->ow_state = ENEMY_WAIT;
                    owpeople_data->vx = 1;
                break;
                case ENEMY_WAIT:
                    if(owpeople_data->wait == 0u){
                        SetSpriteAnim(THIS, a_knight_h_walk, 8u);
                        owpeople_data->vx = -owpeople_data->vx;
                        THIS->mirror = NO_MIRROR;
                        if(owpeople_data->vx < 0){THIS->mirror = V_MIRROR;}
                        owpeople_data->wait = owpeople_data->max_wait;
                        owpeople_data->ow_state = ENEMY_WALK;
                    }
                break;
                case ENEMY_WALK:
                    if(collided_with_motherow){return;}
                    if(owpeople_data->x_frameskip == 0){
                        UINT8 t_coll = TranslateSprite(THIS, owpeople_data->vx << delta_time, 0);
                        if(t_coll){
                            SetSpriteAnim(THIS, a_knight_stand_idle, 8u);   
                            owpeople_data->ow_state = ENEMY_WAIT;
                            owpeople_data->wait = 8u;
                        }
                        owpeople_data->x_frameskip = 3;//TODO potrebbe cambiare
                    }
                    if(owpeople_data->wait == 0){
                        SetSpriteAnim(THIS, a_knight_stand_idle, 8u);
                        owpeople_data->ow_state = ENEMY_WAIT;
                        owpeople_data->wait = 255u;
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
                    if(owpeople_data->x_frameskip == 0){
                        UINT8 v_coll = TranslateSprite(THIS, 0, owpeople_data->vy << delta_time);
                        if(v_coll){
                            SetSpriteAnim(THIS, a_yellow_d_idle, 8u);
                            owpeople_data->ow_state = ENEMY_WAIT;
                            owpeople_data->wait = 8u;
                        }
                        owpeople_data->x_frameskip = 3;//TODO potrebbe cambiare
                    }
                    if(owpeople_data->wait == 0){
                        SetSpriteAnim(THIS, a_yellow_d_idle, 8u);
                        owpeople_data->ow_state = ENEMY_WAIT;
                        owpeople_data->wait = 255u;
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