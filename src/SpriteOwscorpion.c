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

const UINT8 a_owscorpion[] = {4,0,1,2,3}; //The first number indicates the number of frames

extern UINT16 lim_up_y;
extern UINT16 lim_down_y;
extern UINT16 lim_east_x;
extern UINT16 lim_west_x;

void owScorpionTurn() BANKED;

void START(){
	THIS->lim_x = 6500u;
	THIS->lim_y = 6500u;
    SetSpriteAnim(THIS, a_owscorpion, 16u);
    OBP1_REG = PAL_DEF(0, 0, 1, 3);
    SPRITE_SET_PALETTE(THIS,1);
    struct EnemyData* scorpionow_data = (struct EnemyData*)THIS->custom_data;
    scorpionow_data->vx = 1;
    scorpionow_data->wait = 80u;
    scorpionow_data->configured = 0;
    scorpionow_data->x_frameskip = 1;
}

void owScorpionTurn() BANKED{
    struct EnemyData* scorpionow_data = (struct EnemyData*)THIS->custom_data;
    scorpionow_data->vx = -scorpionow_data->vx;
    if(scorpionow_data->vx > 0){THIS->mirror = NO_MIRROR;}
    else{THIS->mirror = V_MIRROR;}
    scorpionow_data->wait = 80u;
}

void UPDATE(){
    struct EnemyData* scorpionow_data = (struct EnemyData*)THIS->custom_data;
    if(scorpionow_data->x_frameskip == 1){
        scorpionow_data->x_frameskip = 0;
        return;
    }
    scorpionow_data->x_frameskip = 1;
    if(scorpionow_data->configured != 0){
        scorpionow_data->wait--;
        UINT8 owscorpion_t_collh = TranslateSprite(THIS, scorpionow_data->vx, 0 << delta_time);
        if(owscorpion_t_collh != 0){
            owScorpionTurn();
        }else{
            if(THIS->x < lim_west_x && scorpionow_data->vx < 0){
                THIS->x = lim_west_x + 3u;
                owScorpionTurn();
            }
            if(THIS->x > lim_east_x && scorpionow_data->vx > 0){
                THIS->x = lim_east_x - 3u;
                owScorpionTurn();
            }
        }
        UINT8 owscorpion_t_collv = 0u;
        if(scorpionow_data->wait < 20){
            owscorpion_t_collv = TranslateSprite(THIS, 0, scorpionow_data->hp << delta_time);
        }
        if(scorpionow_data->wait == 0){
            scorpionow_data->hp = -scorpionow_data->hp;
            scorpionow_data->wait = 80u;
        }
    }
}

void DESTROY(){
    THIS->x = 100u;
}