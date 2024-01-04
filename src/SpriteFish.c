#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_fish[] = {4, 0, 1, 0, 2};
const UINT8 a_fish2[] = {1, 3};

extern INT8 np_counter;

void START(){
    THIS->lim_x = 100u;
    THIS->lim_y = 30u;
    struct EnemyData* fish_info = (struct EnemyData*) THIS->custom_data;
    fish_info->wait = 8u;
}

void UPDATE(){
    struct EnemyData* fish_info = (struct EnemyData*) THIS->custom_data;
    switch(fish_info->configured){
        case 0:
            return;
        break;
        case 1:
            fish_info->vx = -1;
            SetSpriteAnim(THIS,a_fish, 16u);
            fish_info->configured = 5;
        case 3:
            fish_info->vx = 1;
            SetSpriteAnim(THIS,a_fish, 16u);
            fish_info->configured = 5;
            THIS->mirror = V_MIRROR;
        break;
        case 2:    
            fish_info->vx = -1;
            SetSpriteAnim(THIS,a_fish2, 16u);
            fish_info->configured = 5;
        break;
        case 4:
            fish_info->vx = 1;
            SetSpriteAnim(THIS,a_fish2, 16u);
            fish_info->configured = 5;
            THIS->mirror = V_MIRROR;
        break;
        case 5:
            {
                fish_info->wait--;
                if((fish_info->wait & 1) == 1){
                    fish_info->et_collision = TranslateSprite(THIS, fish_info->vx << delta_time, 0);
                    if(fish_info->et_collision){
                        SpriteManagerRemoveSprite(THIS);
                    }
                }
                switch(fish_info->wait){
                    case 0u:
                        fish_info->wait = 24;
                    break;
                    case 10u:
                        THIS->y--;
                    break;
                    case 19:
                        THIS->y++;
                    break;
                }
                UINT8 scroll_f_tile;
                Sprite* fuspr;
                SPRITEMANAGER_ITERATE(scroll_f_tile, fuspr) {
                    if(CheckCollision(THIS, fuspr)) {
                        switch (fuspr->type){
                            case SpriteGator:{
                              	struct PlatformInfo* gator_platform = (struct PlatformInfo*)fuspr->custom_data;
                                if(gator_platform->p_state == ENEMY_IDLE
                                    || gator_platform->p_state == ENEMY_ATTACK){
                                    SpriteManagerRemoveSprite(THIS);
                                }
                            }
                            break;
                        }
                    }
                }
            }
        break;

    }
}

void DESTROY(){
    if(np_counter > 0){np_counter--;}
}