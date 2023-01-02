#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"


const UINT8 rightleft_anim[] = {2, 0, 1};

extern Sprite* s_motherpl;

void START(){
    THIS->lim_x = 255u;
    THIS->lim_y = 255u;
    SetSpriteAnim(THIS, rightleft_anim, 20u);
}

void UPDATE(){
    //THIS->x = (UINT16)s_motherpl->x;
    //THIS->y = (UINT16)s_motherpl->y - 24u;
}

void DESTROY(){
    //THIS->lim_x = 255u;
}