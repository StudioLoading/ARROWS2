#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

const UINT8 drop0_animdrop[] = {8,0,1,2,2,2,2,2,2};
const UINT8 drop1_animdrop[] = {4, 3, 4, 4, 4};

void START(){
    SetSpriteAnim(THIS,drop0_animdrop, 8u);
}

void UPDATE(){
    UINT8 sand_coll = TranslateSprite(THIS, 0, 1 << delta_time);
    if(sand_coll){
        SetSpriteAnim(THIS, drop1_animdrop, 8u);
        if(THIS->anim_frame >= 3){
            SpriteManagerRemoveSprite(THIS);
        }
    }else{
        UINT8 i;
	    Sprite* spr;
	    SPRITEMANAGER_ITERATE(i, spr) {
		    switch(spr->type){
                case SpriteMotherpl:
                case SpriteArrow:
                case SpriteWolf:
                case SpriteBossscorpion:
                case SpriteBossscorpionhead:
                    if(CheckCollision(THIS, spr)) {
                        SetSpriteAnim(THIS, drop1_animdrop, 6u);
                        if(THIS->anim_frame > 3){
                            SpriteManagerRemoveSprite(THIS);
                        }
                    }
                break;
		    }
	    }
    }
}

void DESTROY(){
}
