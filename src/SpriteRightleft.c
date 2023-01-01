#include "Banks/SetAutoBank.h"

#include "main.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"


const UINT8 rightleft_anim[] = {2, 0, 1};

void START(){
    THIS->lim_x = 255u;
    THIS->lim_y = 255u;
    SetSpriteAnim(THIS, rightleft_anim, 8u);
}

void UPDATE(){}

void DESTROY(){
     THIS->lim_x = 255u;
}