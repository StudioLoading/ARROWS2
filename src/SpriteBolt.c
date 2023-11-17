#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "SpriteManager.h"
#include "Scroll.h"

#include "custom_datas.h"


extern MOTHERPL_STATE motherpl_state;

const UINT8 a_bolt[] = {2, 0, 1}; //The first number indicates the number of frames

extern void motherpl_hitted(Sprite* s_enemy) BANKED;

void START() {
    THIS->lim_x = 255;
    THIS->lim_y = 40;
	SetSpriteAnim(THIS, a_bolt, 24u);
    struct EnemyData* bolt_data = (struct EnemyData*) THIS->custom_data;
    bolt_data->e_state = ENEMY_ATTACK;
}

void UPDATE() {
    UINT8 t_coll_bolt = TranslateSprite(THIS, 0, 2 << delta_time);
    if(t_coll_bolt != 0){
        SpriteManagerRemoveSprite(THIS);
    }
    
    //SPRITE COLLISION
        UINT8 bolt_a_tile;
        Sprite* bolt_s_spr;
        SPRITEMANAGER_ITERATE(bolt_a_tile, bolt_s_spr) {
            if(CheckCollision(THIS, bolt_s_spr)) {
                switch(bolt_s_spr->type){
                    case SpriteMotherpl:
                        if(motherpl_state != MOTHERPL_HIT){
                            motherpl_hitted(THIS);
                            SpriteManagerRemoveSprite(THIS);
                        }
                    break;
                }
            }
        };
}

void DESTROY(){
	SpriteManagerAdd(SpritePuff, THIS->x, THIS->y + 16u);
}