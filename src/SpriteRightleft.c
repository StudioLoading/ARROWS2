#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

const UINT8 rightleft_anim[] = {2, 0, 1};

void START(){
    THIS->lim_x = 255u;
    SetSpriteAnim(THIS, rightleft_anim, 16u);
}

void UPDATE(){}

void DESTROY(){}