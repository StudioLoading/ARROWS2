#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern UINT16 BONUS_PUNTA_INIT_Y;
extern UINT16 BONUS_PUNTA_INIT_X;
extern struct PuntaInfo* punta_info;
extern UINT16 strike_x;
extern UINT16 strike_y;
extern UINT8 gator_killed;

const UINT8 punta_idle[] = {1, 0}; //The first number indicates the number of frames
const UINT8 punta_thrown[] = {3, 1,2,3}; //The first number indicates the number of frames

extern void gator_change_state(Sprite* s_gator, ENEMY_STATE new_gator_state) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;

void START(){

}

void UPDATE(){
    switch(punta_info->punta_state){
        case IDLE:
            SetSpriteAnim(THIS, punta_idle, 4u);
            return;
        break;
        case JUST_THROWN://StateBonus mette questo stato
            SetSpriteAnim(THIS, punta_thrown, 12u);
            punta_info->punta_state = GOING;
            punta_info->vy = -2;
        break;
        case GOING:
            TranslateSprite(THIS, 0 << delta_time, punta_info->vy << delta_time);
            if(THIS->y < strike_y){
                UINT8 scroll_p_tile;
                Sprite* puspr;
                //Check sprite collision platform/enemy
                SPRITEMANAGER_ITERATE(scroll_p_tile, puspr) {
                    if(CheckCollision(THIS, puspr)) {
                        switch (puspr->type){
                            case SpriteGator:{
                                struct PlatformInfo* gator_info = (struct PlatformInfo*)puspr->custom_data; 
                                if(gator_info->p_state == ENEMY_WALK || gator_info->p_state == ENEMY_ATTACK){
                                    gator_change_state(puspr, ENEMY_DEAD);
                                    Sprite* s_plusone = SpriteManagerAdd(SpritePlusone, THIS->x, THIS->y);
                                    struct EnemyData* plusone_data = (struct EnemyData*)s_plusone->custom_data;
                                    plusone_data->configured = 1;
                                    gator_killed++;
                                }
                            }
                            break;
                            case SpriteFish:
                                Sprite* s_plusone_coin = SpriteManagerAdd(SpritePlusone, THIS->x, THIS->y - 8);
                                struct EnemyData* plusonecoin_data = (struct EnemyData*)s_plusone_coin->custom_data;
                                plusonecoin_data->configured = 2;
                                change_quantity(INVITEM_MONEY, 5);
                                SpriteManagerRemoveSprite(puspr);
                            break;
                        }
                    }
                }
                punta_info->punta_state = IDLE;
                SpriteManagerAdd(SpritePuntawater, THIS->x, THIS->y - 2u);
            } 
        break;
    }
}

void DESTROY(){

}