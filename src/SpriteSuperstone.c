#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

const UINT8 superstone_anim0[] = {1, 0};
const UINT8 superstone_anim1[] = {1, 1};
const UINT8 superstone_anim2[] = {1, 1};
extern UINT8 superstone_destroyed;

void START(){
    if(superstone_destroyed == 0u){
        SetSpriteAnim(THIS,superstone_anim0, 16u);
    }else{
        SpriteManagerRemoveSprite(THIS);
    }
}

void UPDATE(){
    TranslateSprite(THIS, 0 << delta_time, 1 << delta_time);
}

void DESTROY(){
}