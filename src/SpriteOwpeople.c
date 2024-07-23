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
const UINT8 a_owpeople_u_walk[] = {6,5,6,7,6,5,8}; //The first number indicates the number of frames
const UINT8 a_owpeople_r_walk[] = {4,9,10,9,11}; //The first number indicates the number of frames

void START(){
    SetSpriteAnim(THIS, a_owpeople_hidden, 8u);
	THIS->lim_x = 6500u;
	THIS->lim_y = 6500u;
    struct OwPeopleData* owpeople_data = (struct OwPeopleData*)THIS->custom_data;
    owpeople_data->vx = 1;
    owpeople_data->wait = 80u;
    owpeople_data->type = OWTYPE_UNCONFIGURED;
    owpeople_data->x_frameskip = 1;
    owpeople_data->ow_state = ENEMY_WAIT;
}

void UPDATE(){
    struct OwPeopleData* owpeople_data = (struct OwPeopleData*)THIS->custom_data;
    owpeople_data->wait--;
    switch(owpeople_data->type){
        case OWTYPE_UNCONFIGURED:
            return;
        break;
        case OWTYPE_BLUETUNIC_STAND:
            switch(owpeople_data->ow_state){
                case ENEMY_WAIT:
                    SetSpriteAnim(THIS, a_bluetunicstand_idle, 8u);
                    owpeople_data->ow_state = ENEMY_IDLE;
                break;
                case ENEMY_IDLE:
                    //DO NOTHING
                break;
            }
        break;
        case OWTYPE_KNIGHT_STAND:
            switch(owpeople_data->ow_state){
                case ENEMY_WAIT:
                    SetSpriteAnim(THIS, a_knight_stand_idle, 8u);
                    owpeople_data->ow_state = ENEMY_IDLE;
                break;
                case ENEMY_IDLE:
                    //DO NOTHING
                break;
            }
        break;
	    case OWTYPE_KNIGHT_HORIZONTAL:
            switch(owpeople_data->ow_state){
                case ENEMY_WAIT:
                    SetSpriteAnim(THIS, a_knight_stand_idle, 8u);
                    owpeople_data->ow_state = ENEMY_IDLE;
                break;
                case ENEMY_IDLE:
                    if(owpeople_data->wait == 0u){
                        SetSpriteAnim(THIS, a_knight_h_walk, 12u);
                        owpeople_data->vx = -owpeople_data->vx;
                        THIS->mirror = NO_MIRROR;
                        if(owpeople_data->vx < 0){THIS->mirror = V_MIRROR;}
                         owpeople_data->wait = 200u;//TODO potrebbe cambiare
                         owpeople_data->ow_state = ENEMY_WALK;
                    }
                break;
                case ENEMY_WALK:
                    if(owpeople_data->x_frameskip == 0){
                        UINT8 t_coll = TranslateSprite(THIS, owpeople_data->vx << delta_time, 0);
                        if(t_coll){
                            owpeople_data->ow_state = ENEMY_IDLE;
                            owpeople_data->wait = 8u;
                        }
                        owpeople_data->x_frameskip = 1;//TODO potrebbe cambiare
                    }else{owpeople_data->x_frameskip--;}
                    if(owpeople_data->wait == 0){
                        SetSpriteAnim(THIS, a_knight_stand_idle, 8u);
                        owpeople_data->ow_state = ENEMY_IDLE;
                        owpeople_data->wait = 120u;
                    }
                break;
            }
        break;
    }
}

void DESTROY(){

}