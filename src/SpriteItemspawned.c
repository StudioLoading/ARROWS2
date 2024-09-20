#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define ITEMSPAWNED_FRAMESKIP_MAX 4

const UINT8 is_metal_s[] = {1, 9};
const UINT8 is_metal_dis_s[] = {2, 9, 3};
const UINT8 is_metal[] = {1, 0};
const UINT8 is_metal_dis[] = {2, 0, 3};
const UINT8 is_wood[] = {1, 1};
const UINT8 is_wood_dis[] = {2, 1, 3};
const UINT8 is_heart[] = {4, 6, 7, 2, 7};
const UINT8 is_heart_dis[] = {2, 2, 3};
const UINT8 is_money[] = {4, 6, 5, 4, 5};
const UINT8 is_money_dis[] = {2, 4, 3};
const UINT8 is_crossb[] = {4, 10, 7, 6, 7};
const UINT8 is_crossb_dis[] = {2, 10, 3};
const UINT8 is_powd[] = {1, 8};
const UINT8 is_powd_dis[] = {2, 8, 3};
const UINT8 is_hidden[] = {1, 3};

UINT8 powder_cooldown = 60;
extern UINT8 itemspawned_powder_max;
extern UINT8 itemspawned_powder_counter;
extern Sprite* s_motherpl;

void spawnItem(INVITEMTYPE itemtype, UINT16 spawn_at_x, UINT16 spawn_at_y ) BANKED;
UINT8 is_item_equippable(INVITEMTYPE itemtype) BANKED;


void START(){
    THIS->lim_x = 1000u;
    THIS->lim_y = 1000u;
    struct ItemSpawned* spawned_data = (struct ItemSpawned*) THIS->custom_data;
    spawned_data->configured = 0u;
}

void UPDATE(){
    struct ItemSpawned* spawned_data = (struct ItemSpawned*) THIS->custom_data;
    switch(spawned_data->configured){
        case 0u: return; break;
        case 1u:
            spawned_data->hp = 160u;
            spawned_data->vy = -1;
            spawned_data->vx = 0;
            spawned_data->frmskip = ITEMSPAWNED_FRAMESKIP_MAX;
            switch(spawned_data->itemtype){
                case INVITEM_HEARTS: spawned_data->equippable = 0u; SetSpriteAnim(THIS, is_crossb, 8u); break;
                case INVITEM_HEART: spawned_data->equippable = 0u; SetSpriteAnim(THIS, is_heart, 4u); break;
                case INVITEM_MONEY: spawned_data->equippable = 1u; SetSpriteAnim(THIS, is_money, 8u); break;
                case INVITEM_METAL: spawned_data->equippable = 0u; SetSpriteAnim(THIS, is_metal, 4u); break;
                case INVITEM_SILVER: spawned_data->equippable = 0u; SetSpriteAnim(THIS, is_metal_s, 4u); break;
                case INVITEM_WOOD: spawned_data->equippable = 0u; SetSpriteAnim(THIS, is_wood, 4u); break;
                case INVITEM_POWDER: 
                    spawned_data->equippable = 0u; 
                    SetSpriteAnim(THIS, is_powd, 4u);
                break;
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
                if(spawned_tile_coll && 
                    spawned_data->vy <= 0){
                    spawned_data->vy=1;
                }
                spawned_data->vx = 0;
                spawned_data->frmskip--;
                if(spawned_data->frmskip == 0){
                    if(spawned_data->itemtype == INVITEM_MONEY 
                        || spawned_data->itemtype == INVITEM_HEART){
                        spawned_data->vx = 1;
                        if(THIS->x > s_motherpl->x){
                            spawned_data->vx = -1;
                        }
                    }
                    spawned_data->frmskip = ITEMSPAWNED_FRAMESKIP_MAX;
                }
            }
            }
            if(spawned_data->itemtype != INVITEM_SILVER){
                spawned_data->hp--;
            }
            if(spawned_data->hp == 0){
                switch(spawned_data->itemtype){
                    case INVITEM_HEARTS: SetSpriteAnim(THIS, is_crossb_dis, 20u); break;
                    case INVITEM_HEART: SetSpriteAnim(THIS, is_heart_dis, 20u); break;
                    case INVITEM_MONEY: SetSpriteAnim(THIS, is_money_dis, 20u); break;
                    case INVITEM_METAL:  SetSpriteAnim(THIS, is_metal_dis, 20u); break;
                    case INVITEM_SILVER: SetSpriteAnim(THIS, is_metal_dis_s, 20u); break;
                    case INVITEM_WOOD: SetSpriteAnim(THIS, is_wood_dis, 20u); break;
                    case INVITEM_POWDER: SetSpriteAnim(THIS, is_powd_dis, 20u); break;
                }
                spawned_data->hp = 120u;
                spawned_data->configured = 3u;
            }
        break;
        case 3u:
            spawned_data->hp--;
            if(spawned_data->hp == 0u){
                SpriteManagerRemoveSprite(THIS);
            }
        break;
        case 4u://hidden in overworld
            SetSpriteAnim(THIS, is_hidden, 1u);
        break;
    }
}

UINT8 is_item_equippable(INVITEMTYPE itemtype) BANKED{
    UINT8 equippable = 0u;
    switch(itemtype){
        case INVITEM_ARROW_NORMAL:
        case INVITEM_ARROW_BASTARD:
        case INVITEM_ARROW_PERFO:
        case INVITEM_MONEY:
        case INVITEM_BOMB:
            equippable = 1u;
        break;
    }
    return equippable;
}

void spawnItem(INVITEMTYPE itemtype, UINT16 spawn_at_x, UINT16 spawn_at_y ) BANKED{
    if(itemtype == INVITEM_POWDER){
        itemspawned_powder_counter++;
        if(itemspawned_powder_counter > itemspawned_powder_max){
            return;
        }
    }
    //SPAWN ITEM
    UINT16 quantity = 1u;
    if(itemtype == INVITEM_ARROW_BASTARD || itemtype == INVITEM_ARROW_PERFO ||
        itemtype == INVITEM_ARROW_NORMAL){
            quantity = 40u;
        }      
    Sprite* reward = SpriteManagerAdd(SpriteItemspawned, spawn_at_x, spawn_at_y -8u);
    struct ItemSpawned* reward_data = (struct ItemSpawned*) reward->custom_data;
    reward_data->itemtype = itemtype;
    reward_data->quantity = quantity;
    UINT8 eq = is_item_equippable(itemtype);
    reward_data->equippable = eq;
    reward_data->configured = 1u;
}

void DESTROY(){}