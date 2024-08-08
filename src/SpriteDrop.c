#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 drop0_animdrop[] = {8,0,1,2,2,2,2,2,2};
const UINT8 drop1_animdrop[] = {4, 3, 4, 4, 4};

void START(){
    SetSpriteAnim(THIS,drop0_animdrop, 8u);
    struct EnemyData* drop_data = (struct EnemyData*) THIS->custom_data;
    drop_data->e_state = ENEMY_WALK;
}

void UPDATE(){
    struct EnemyData* drop_data = (struct EnemyData*) THIS->custom_data;
    if(drop_data->e_state != ENEMY_DEAD){
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
                    case SpriteMotherplarmor:
                    case SpriteArrow:
                    case SpriteWolf:
                    case SpriteBossscorpion:
                    case SpriteBossscorpionhead:
                        if(CheckCollision(THIS, spr) && drop_data->e_state != ENEMY_DEAD){
                            SetSpriteAnim(THIS, drop1_animdrop, 6u);
                            drop_data->e_state = ENEMY_DEAD;
                        }
                    break;
                }
            }
        }
    }else{
        if(THIS->anim_frame >= 2){
            SpriteManagerRemoveSprite(THIS);
        }
    }
}

void DESTROY(){
}
