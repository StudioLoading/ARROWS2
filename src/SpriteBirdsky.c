#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_birdsky[] = {4, 0, 1, 2, 1};
const UINT8 a_birdsky2[] = {4, 3, 4, 5, 4};

extern INT8 np_counter;

void START(){
    THIS->lim_x = 30u;
    THIS->lim_y = 30u;
    struct EnemyData* birdsky_info = (struct EnemyData*) THIS->custom_data;
    birdsky_info->vx = -1;
    birdsky_info->wait = 8u;
    birdsky_info->hp = 24;
}

void UPDATE(){
    struct EnemyData* birdsky_info = (struct EnemyData*) THIS->custom_data;
    switch(birdsky_info->configured){
        case 0:
            return;
        break;
        case 1:
            SetSpriteAnim(THIS,a_birdsky, 16u);
            birdsky_info->configured = 3;
        break;
        case 2:    
            SetSpriteAnim(THIS,a_birdsky2, 16u);
            birdsky_info->configured = 3;
        break;
        case 3:
            {
                birdsky_info->wait--;
                switch(birdsky_info->wait){
                    case 0u:
                        birdsky_info->wait = 8;
                        THIS->x--;
                        birdsky_info->wait = 24;
                    break;
                    case 12u:
                        THIS->y--;
                        birdsky_info->hp--;
                        if(birdsky_info->hp <= 0){
                            SpriteManagerRemoveSprite(THIS);
                        }
                    break;
                    case 20:
                        THIS->y++;
                    break;
                }
            }
        break;

    }
}

void DESTROY(){
    if(np_counter > 0){np_counter--;}
}