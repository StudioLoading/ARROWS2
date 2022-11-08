#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define GRAVITY 3
#define GRAVITY_FRAMESKIP 3
#define FRICTION 128
#define FRICTION_LOW 200

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 flipperball_idle[] = {1, 0}; //The first number indicates the number of frames
const UINT8 flipperball_ccw[] = {4, 0, 1, 2, 3}; //The first number indicates the number of frames
const UINT8 flipperball_cw[] = {4, 3, 2, 1, 0}; //The first number indicates the number of frames
struct FlipperballInfo* flipperball_info;
INT8 counter_gravity_frameskip = 0;
INT8 counter_friction_frameskip = 0;
INT8 rolling_counter = 0;

void START() {
	SetSpriteAnim(THIS, flipperball_ccw, 12u);
    flipperball_info = (struct FlipperballInfo*) THIS->custom_data;
    flipperball_info->impulse_x = 0;
    flipperball_info->impulse_y = 0;
    flipperball_info->vx = 0;
    flipperball_info->vy = 0;
}

void UPDATE() {
    if(KEY_PRESSED(J_JUMP) || KEY_PRESSED(J_FIRE)){
        flipperball_info->impulse_y = -4;
    }
    if(KEY_TICKED(J_LEFT)){
        flipperball_info->impulse_x = -4;
    }
    if(KEY_TICKED(J_RIGHT)){
        flipperball_info->impulse_x = 4;
    }

    //REACT TO IMPULSE
    if(flipperball_info->impulse_y != 0){
        flipperball_info->vy += flipperball_info->impulse_y;
        flipperball_info->impulse_y = 0;
    }
    if(flipperball_info->impulse_x != 0){
        flipperball_info->vx += flipperball_info->impulse_x;
        flipperball_info->impulse_x = 0;
    }

    //GRAVITY
    if(counter_gravity_frameskip < GRAVITY_FRAMESKIP){
        counter_gravity_frameskip++;
    }else{
        flipperball_info->vy += GRAVITY;
        counter_gravity_frameskip = 0;
    }

    //FRICTION
    INT8 friction_to_use = FRICTION;
    if(flipperball_info->vx < 1){
        friction_to_use = FRICTION_LOW;
    }
    if(counter_friction_frameskip < friction_to_use){
        counter_friction_frameskip++;
    }else{
        counter_friction_frameskip = 0;
    }

    //MOVING ACCORDING TO _vx,_vy
    UINT8 tile_collision = 0;
    if(flipperball_info->vx != 0 || flipperball_info->vy != 0){
        tile_collision = TranslateSprite(THIS, flipperball_info->vx << delta_time, flipperball_info->vy << delta_time);
    }

    switch(tile_collision){
        case 2u:
            //flipperball_info->impulse_y = -12;
        break;
        case 6u:
            flipperball_info->vx += 2;
            flipperball_info->vy -= 4;
        break;
        case 7u:
            flipperball_info->vx = -flipperball_info->vx +1;
        break;
        case 8u:
            flipperball_info->vy = 0;
        break;
        case 9u:
            flipperball_info->vx -= 2;
            flipperball_info->vy -= 4;
        break;
    }
    
    //ZEROING _vx,_vy
    if(counter_friction_frameskip == 0){
        if(tile_collision != 2u){
            if(flipperball_info->vx > 0){flipperball_info->vx -= 1;}
            if(flipperball_info->vx < 0){flipperball_info->vx += 1;}
        }
    }     
    if (tile_collision == 6u || tile_collision == 9u){
        THIS->y+=1;
    }

    if(flipperball_info->vy > 0){flipperball_info->vy -= 1;}
    if(flipperball_info->vy < 0){flipperball_info->vy += 1;}

    if(flipperball_info->vx > 1){
        SetSpriteAnim(THIS, flipperball_cw, 12u);
    }else if (flipperball_info->vx < -1){        
        SetSpriteAnim(THIS, flipperball_ccw, 12u);
    }else{
        SetSpriteAnim(THIS, flipperball_idle, 4u);
    }
}

void DESTROY() {
}
