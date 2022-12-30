#include "Banks/SetAutoBank.h"

#include "main.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 throw_ball[] = {1, 0};
const UINT8 throw_web0[] = {1, 1};
const UINT8 throw_web1[] = {1, 2};

extern Sprite* s_motherpl;
extern Sprite* s_blocking;
extern UINT8 motherpl_blocked;

void START(){
    THIS->lim_x = 255u;
    SetSpriteAnim(THIS, throw_ball, 2u);
    struct ThrowableData* throwable_data = (struct ThrowableData*) THIS->custom_data;
    throwable_data->hp = 4;
    throwable_data->wait = 8;
    throwable_data->x_frameskip = 0;
    throwable_data->configured = 0;
    throwable_data->type = T_UNASSIGNED;
    throwable_data->vx = -2;
    if(s_motherpl->x > THIS->x){throwable_data->vx = 2;}
}

void UPDATE(){
    struct ThrowableData* throwable_data = (struct ThrowableData*) THIS->custom_data;
    if(throwable_data->hp <= 0 || throwable_data->type == T_DESTROYED){
        SpriteManagerRemoveSprite(THIS);
        return;
    }
    throwable_data->wait--;
    if(throwable_data->wait == 0 && throwable_data->hp > 2){
        throwable_data->wait = 8;
        throwable_data->hp--;
        switch(throwable_data->hp){
            case 3:
                SetSpriteAnim(THIS, throw_web0, 2u);
            break;
            case 2:
                SetSpriteAnim(THIS, throw_web1, 2u);
            break;
        }
    }
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

    //SPRITE COLLISION
        UINT8 throwable_a_tile;
        Sprite* throwablespr;
        SPRITEMANAGER_ITERATE(throwable_a_tile, throwablespr) {
            if(CheckCollision(THIS, throwablespr)) {
                switch(throwablespr->type){
                    case SpriteArrow:
                        throwable_data->hp=0;
                        {
                            struct ArrowData* arrow_data = (struct ArrowData*) throwablespr->custom_data;
                            if(arrow_data->arrow_type == ARROW_NORMAL){
                                SpriteManagerRemoveSprite(throwablespr);
                            }
                        }
                    break;
                    case SpriteMotherpl:
                        if(motherpl_blocked == 0u && throwable_data->type != T_DESTROYED){
                            s_blocking = THIS;
                            throwable_data->vx = 0u;
                            motherpl_blocked = 1u;
                        }
                    break;
                }
            }
        };
}

void DESTROY(){
    struct ThrowableData* throwable_data = (struct ThrowableData*) THIS->custom_data;
    throwable_data->type = T_DESTROYED;
}