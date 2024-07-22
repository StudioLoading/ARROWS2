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

const UINT8 a_owpeople_d_idle[] = {2,0,1}; //The first number indicates the number of frames
const UINT8 a_owpeople_d_walk[] = {6,0,1,2,1,0,3}; //The first number indicates the number of frames
const UINT8 a_owpeople_u_walk[] = {6,4,5,6,5,4,7}; //The first number indicates the number of frames
const UINT8 a_owpeople_r_walk[] = {4,8,9,8,10}; //The first number indicates the number of frames

void START(){
    SetSpriteAnim(THIS, a_owpeople_d_idle, 8u);
	THIS->lim_x = 6500u;
	THIS->lim_y = 6500u;
    struct EnemyData* owpeople_data = (struct EnemyData*)THIS->custom_data;
    owpeople_data->vx = 1;
    owpeople_data->wait = 80u;
    owpeople_data->type = OWTYPE_UNCONFIGURED;
    owpeople_data->x_frameskip = 1;
}

void UPDATE(){
    struct EnemyData* owpeople_data = (struct EnemyData*)THIS->custom_data;
    switch(owpeople_data->TYPE){
        case OWTYPE_UNCONFIGURED:
            return;
        break;
    }
}

void DESTROY(){

}