#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

const UINT8 sand0_anim[] = {8, 1, 2, 3, 2, 3, 2, 3, 2};
const UINT8 sand1_anim[] = {4, 3, 4, 4, 4};

void START(){
    SetSpriteAnim(THIS,sand0_anim, 12u);
}

void UPDATE(){
    UINT8 sand_coll = TranslateSprite(THIS, 0, 1 << delta_time);
    if(sand_coll){
        SetSpriteAnim(THIS, sand1_anim, 16u);
        if(THIS->anim_frame == 3){
            SpriteManagerRemoveSprite(THIS);
        }
    }
}

void DESTROY(){
}
