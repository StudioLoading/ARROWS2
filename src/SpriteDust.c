#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

const UINT8 dust_anim[] = {4, 0, 1, 2, 2};

void START(){
    SetSpriteAnim(THIS,dust_anim, 16u);
}

void UPDATE(){
    if(THIS->anim_frame == 3){
        SpriteManagerRemoveSprite(THIS);
    }
}

void DESTROY(){
}