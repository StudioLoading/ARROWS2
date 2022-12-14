#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define ITEMSPAWNED_FRAMESKIP_MAX 6

const UINT8 is_metal[] = {1, 0};
const UINT8 is_metal_dis[] = {2, 0, 3};
const UINT8 is_wood[] = {1, 1};
const UINT8 is_wood_dis[] = {2, 1, 3};
const UINT8 is_heart[] = {1, 2};
const UINT8 is_heart_dis[] = {2, 2, 3};
const UINT8 is_money[] = {4, 6, 5, 4, 5};
const UINT8 is_money_dis[] = {2, 4, 3};
const UINT8 is_crossb[] = {1, 7};
const UINT8 is_crossb_dis[] = {2, 7, 3};
const UINT8 is_powd[] = {1, 8};
const UINT8 is_powd_dis[] = {2, 8, 3};


void START(){
    THIS->lim_x = 255u;
    struct InvItem* spawned_data = (struct InvItem*) THIS->custom_data;
    spawned_data->configured = 0u;
}

void UPDATE(){
    struct InvItem* spawned_data = (struct InvItem*) THIS->custom_data;
    switch(spawned_data->configured){
        case 0u: return; break;
        case 1u:
            spawned_data->hp = 160u;
            spawned_data->vy = -2;
            spawned_data->vx = 0;
            spawned_data->frmskip = ITEMSPAWNED_FRAMESKIP_MAX;
            switch(spawned_data->itemtype){
                case INVITEM_CROSSBOW: spawned_data->equippable = 0u; SetSpriteAnim(THIS, is_crossb, 4u); break;
                case INVITEM_HEART: spawned_data->equippable = 0u; SetSpriteAnim(THIS, is_heart, 4u); break;
                case INVITEM_MONEY: spawned_data->equippable = 1u; SetSpriteAnim(THIS, is_money, 8u); break;
                case INVITEM_METAL: spawned_data->equippable = 0u; SetSpriteAnim(THIS, is_metal, 4u); break;
                case INVITEM_WOOD: spawned_data->equippable = 0u; SetSpriteAnim(THIS, is_wood, 4u); break;
            }
            spawned_data->configured = 2u;
        break;
        case 2u:
            {
            UINT8 spawned_tile_coll = TranslateSprite(THIS, spawned_data->vx << delta_time, 
                spawned_data->vy << delta_time);
            if(spawned_data->vy <= 0){
                spawned_data->frmskip--;
                if(spawned_data->frmskip == 0){
                    spawned_data->vy++;
                    spawned_data->frmskip = ITEMSPAWNED_FRAMESKIP_MAX;
                }
            }
            if(spawned_tile_coll || spawned_data->vy > 0){
                spawned_data->vx = 0;
                spawned_data->frmskip--;
                if(spawned_data->frmskip == 0){
                    if(spawned_data->itemtype == INVITEM_MONEY){
                        spawned_data->vx = 1;
                    }
                    spawned_data->frmskip = ITEMSPAWNED_FRAMESKIP_MAX;
                }
            }
            spawned_data->hp--;
            if(spawned_data->hp == 0){
                switch(spawned_data->itemtype){
                    case INVITEM_CROSSBOW: SetSpriteAnim(THIS, is_crossb_dis, 20u); break;
                    case INVITEM_HEART: SetSpriteAnim(THIS, is_heart_dis, 20u); break;
                    case INVITEM_MONEY: SetSpriteAnim(THIS, is_money_dis, 20u); break;
                    case INVITEM_METAL: SetSpriteAnim(THIS, is_metal_dis, 20u); break;
                    case INVITEM_WOOD: SetSpriteAnim(THIS, is_wood_dis, 20u); break;
                }
                spawned_data->hp = 120u;
                spawned_data->configured = 3u;
            }
            }
        break;
        case 3u:
            spawned_data->hp--;
            if(spawned_data->hp == 0u){
                SpriteManagerRemoveSprite(THIS);
            }
        break;
    }
}

void DESTROY(){}