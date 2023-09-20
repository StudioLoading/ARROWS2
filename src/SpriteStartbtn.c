#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

const UINT8 a_startbtn[] = {2,0,1}; //The first number indicates the number of frames

void START(){
	THIS->lim_x = 255u;
	THIS->lim_y = 255u;
    SetSpriteAnim(THIS, a_startbtn, 12u);
}

void UPDATE(){
}

void DESTROY(){

}