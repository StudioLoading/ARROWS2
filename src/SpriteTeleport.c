#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_teleporting[] = {1, 0}; //The first number indicates the number of frames


void START(){
    THIS->lim_x = 255u;
    THIS->lim_y = 255u;
    SetSpriteAnim(THIS, a_teleporting, 10u);
}


void UPDATE(){    

}

void DESTROY(){
    
}