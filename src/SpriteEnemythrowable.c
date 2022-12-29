#include "Banks/SetAutoBank.h"

#include "main.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 throw_web[] = {1, 2};

extern Sprite* s_motherpl;

void START(){
    THIS->lim_x = 255u;
    SetSpriteAnim(THIS, throw_web, 2u);
    struct ThrowableData* throwable_data = (struct ThrowableData*) THIS->custom_data;
    throwable_data->hp = 0;
    throwable_data->wait = 0;
    throwable_data->x_frameskip = 1;
    throwable_data->vx = -1;
    if(s_motherpl->x > THIS->x){throwable_data->vx = 2;}
}

void UPDATE(){
    struct ThrowableData* throwable_data = (struct ThrowableData*) THIS->custom_data;
    UINT8 h_tile_coll = 0u;
    if(throwable_data->x_frameskip == 0){
        h_tile_coll = TranslateSprite(THIS, throwable_data->vx << delta_time, 0);
        throwable_data->x_frameskip = 1;
    }else{
        throwable_data->x_frameskip = 0;
    }
    if(h_tile_coll){
        SpriteManagerRemoveSprite(THIS);
    }
}

void DESTROY(){}