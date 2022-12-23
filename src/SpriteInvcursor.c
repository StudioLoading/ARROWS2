#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"


const UINT8 inv_cursor_anim[] = {2,0,1}; //The first number indicates the number of frames

void START(){
	THIS->lim_x = 255u;
	THIS->lim_y = 255u;
    SetSpriteAnim(THIS, inv_cursor_anim, 32u);
}

void UPDATE(){
    
}

void DESTROY(){

}