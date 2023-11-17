#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Sound.h"

#include "custom_datas.h"

#define THROWABLE_VX 2
#define THROWABLE_GRAVITY 2

const UINT8 throw_ball[] = {1, 0};
const UINT8 throw_web0[] = {1, 1};
const UINT8 throw_web1[] = {1, 2};
const UINT8 throw_acid0[] = {1, 3};
const UINT8 throw_acid1[] = {6, 4, 5, 6, 7, 7, 7};
const UINT8 throw_projectile[] = {1, 8};

extern Sprite* s_motherpl;
extern Sprite* s_blocking;
extern UINT8 motherpl_blocked;
extern MOTHERPL_STATE motherpl_state;
extern UINT8 motherpl_hit;

extern void motherpl_hitted(Sprite* s_enemy) BANKED;

void START(){
    THIS->lim_x = 64u;
    SetSpriteAnim(THIS, throw_ball, 2u);
    struct ThrowableData* throwable_data = (struct ThrowableData*) THIS->custom_data;
    throwable_data->hp = 1;
    throwable_data->wait = 8u;
    throwable_data->x_frameskip = 0;
    throwable_data->configured = 0;
    throwable_data->type = T_UNASSIGNED;
    throwable_data->vx = -THROWABLE_VX;
    throwable_data->vy = 0u;
    if(s_motherpl->x > THIS->x){throwable_data->vx = THROWABLE_VX;}
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE(){
    struct ThrowableData* throwable_data = (struct ThrowableData*) THIS->custom_data;
    if(throwable_data->configured == 0u){return;}
    if(throwable_data->configured == 1u){
        //if(throwable_data->wait == 8u){
            switch(throwable_data->type){
                case WEB:
                    SetSpriteAnim(THIS, throw_web1, 2u);
                break;
                case ACID:
                    throwable_data->vy = -3;
                    throwable_data->vx = -2;
                    if(s_motherpl->x > THIS->x){throwable_data->vx = 2;}
                    if(((INT8)(s_motherpl->x - THIS->x)) > 48 || 
                        ((INT8)(THIS->x - s_motherpl->x) > 48)){
                        throwable_data->vx *= 2;
                        throwable_data->vy = -5;
                    }
                    SetSpriteAnim(THIS, throw_acid0, 2u);
                break;
                case PROJECTILE: 
                    SetSpriteAnim(THIS, throw_projectile, 1u);
                break;
            }
            throwable_data->configured = 2u;
        return;
    }    
    if(throwable_data->type == T_DESTROYED){
        SpriteManagerRemoveSprite(THIS);
        return;
    }
    if(throwable_data->type == T_DESTROYING){
        if(THIS->anim_frame == 5){
            throwable_data->type = T_DESTROYED;
        }
        return;
    }
    if(throwable_data->hp <= 0){
        throwable_data->type = T_DESTROYED;
        return;
    }
    throwable_data->wait--;
    //VERTICAL MOVEMENT
    if(throwable_data->type == ACID){
        if(throwable_data->vy < THROWABLE_GRAVITY){
            throwable_data->vy++;
        }
        UINT8 v_tile_coll = TranslateSprite(THIS, 0, throwable_data->vy << delta_time);
        if(v_tile_coll == 0){v_tile_coll = TranslateSprite(THIS, throwable_data->vx << delta_time, 0);}
        else{TranslateSprite(THIS, throwable_data->vx << delta_time, 0);}
        if(v_tile_coll){
            throwable_data->type = T_DESTROYING;
            throwable_data->wait = 64u;
            SetSpriteAnim(THIS, throw_acid1, 8u);
        }    
    }//HORIZONTAL MOVEMENT
    else if(throwable_data->type == PROJECTILE){
            UINT8 h_tile_coll = 0u;
            h_tile_coll = TranslateSprite(THIS, throwable_data->vx << delta_time, 0);
            if(h_tile_coll){
                SpriteManagerRemoveSprite(THIS);
            }
    }else{
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
                            switch(throwable_data->type){
                                case PROJECTILE:
                                    motherpl_blocked = 0u;
                                    if(motherpl_state == MOTHERPL_CRAWL || motherpl_state == MOTHERPL_DASH){
                                        if(THIS->y >= (s_motherpl->y + 4)){
                                            //cioè se mi prenderebbe, ma 
                                            //sono in ginocchio o dasho
                                            //non collidere ma simulo schivata
                                            motherpl_hitted(THIS);
                                        }
                                    }else{
                                        motherpl_hitted(THIS);
                                    }
                                break;
                                case ACID:
                                    motherpl_blocked = 0u;
                                    motherpl_hitted(THIS);
                                break;
                                case WEB:
                                    if(motherpl_state != MOTHERPL_BLOCKED){
                                        s_blocking = THIS;
                                        motherpl_blocked = 1u;
                                        throwable_data->vx = 0u;
                                    }
                                break;
                            }
                            if(motherpl_hit == 1){throwable_data->hp=0;}
                        }
                    break;
                }
            }
        };
}

void DESTROY(){
    SpriteManagerAdd(SpritePuff, THIS->x, THIS->y - 4u);
}