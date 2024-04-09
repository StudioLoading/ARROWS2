#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_skull[] = {3,1,2,3};
const UINT8 a_skull_frozen[] = {2,0,1};

void START(){
    SetSpriteAnim(THIS,a_skull, 32u);
    struct EnemyData* skull_data = (struct EnemyData*) THIS->custom_data;
    skull_data->vx = 1;
    skull_data->x_frameskip = 3u;
    skull_data->hp = -1;
    skull_data->wait = 3;
}

void UPDATE(){
    struct EnemyData* skull_data = (struct EnemyData*) THIS->custom_data;
    skull_data->x_frameskip--;
    if(skull_data->vx == 0){
        return;
    }
    if(THIS->y < 32u){
        skull_data->hp = 1;
        skull_data->wait = 6;
    }
    UINT8 v_collision = TranslateSprite(THIS, 0, skull_data->hp << delta_time);
    if(v_collision == 2u || v_collision == 6u || v_collision == 10u){
        skull_data->vx = 0;
        SpriteManagerAdd(SpritePuff, THIS->x + 4u, THIS->y + 8u);
        SetSpriteAnim(THIS,a_skull_frozen, 12u);
    }
    if(skull_data->x_frameskip == 0u || skull_data->x_frameskip > skull_data->wait){
        skull_data->x_frameskip = skull_data->wait-1;
        UINT8 skull_tile_coll = TranslateSprite(THIS, skull_data->vx << delta_time, 0);
        if(skull_tile_coll != 0){
            skull_data->vx = -skull_data->vx;
        }
    }
}

void DESTROY(){
}
