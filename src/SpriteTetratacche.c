#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 tacche_0_idle[] = {1, 0}; //The first number indicates the number of frames
const UINT8 tacche_1_idle[] = {1, 1}; //The first number indicates the number of frames
const UINT8 tacche_2_idle[] = {1, 2}; //The first number indicates the number of frames
const UINT8 tacche_3_idle[] = {1, 3}; //The first number indicates the number of frames
const UINT8 tacche_4_idle[] = {1, 4}; //The first number indicates the number of frames
const UINT8 tacche_5_idle[] = {1, 5}; //The first number indicates the number of frames
const UINT8 tacche_6_idle[] = {1, 6}; //The first number indicates the number of frames

void START(){ 
    THIS->lim_x = 255u;
    THIS->lim_y = 255u;
}

void UPDATE(){
}

void DESTROY(){
    
}