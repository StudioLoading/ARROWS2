#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define ITEMSPAWNED_FRAMESKIP_MAX 8

const UINT8 itmspwn_anim_metal[] = {1, 0};
const UINT8 itmspwn_anim_metal_dis[] = {2, 0, 3};
const UINT8 itmspwn_anim_wood[] = {1, 1};
const UINT8 itmspwn_anim_wood_dis[] = {2, 1, 3};
const UINT8 itmspwn_anim_heart[] = {1, 2};
const UINT8 itmspwn_anim_heart_dis[] = {2, 2, 3};
const UINT8 itmspwn_anim_money[] = {4, 6, 5, 4, 5};
const UINT8 itmspwn_anim_money_dis[] = {2, 4, 3};
const UINT8 itmspwn_anim_crossb[] = {1, 7};
const UINT8 itmspwn_anim_crossb_dis[] = {2, 7, 3};
const UINT8 itmspwn_anim_powd[] = {1, 8};
const UINT8 itmspwn_anim_powd_dis[] = {2, 8, 3};

void START(){
    THIS->lim_x = 255u;
    THIS->lim_y = 255u;
    struct InvItem* spawned_data = (struct InvItem*) THIS->custom_data;
    spawned_data->configured = 0u;
    spawned_data->itemtype = INVITEM_UNASSIGNED;
    spawned_data->quantity = 0u;
    spawned_data->hp = 160u;
    spawned_data->vy = -2;
    spawned_data->vx = 0;
    spawned_data->frmskip = ITEMSPAWNED_FRAMESKIP_MAX;
}

void UPDATE(){
    struct InvItem* spawned_data = (struct InvItem*) THIS->custom_data;
    switch(spawned_data->configured){
        case 0u: return; break;
        case 1u:
            if(spawned_data->itemtype == INVITEM_CROSSBOW){SetSpriteAnim(THIS, itmspwn_anim_crossb, 4u);}
            if(spawned_data->itemtype == INVITEM_MONEY){
                SetSpriteAnim(THIS, itmspwn_anim_money, 8u);
                spawned_data->vx = 0u;
            }
            spawned_data->configured = 2u;
            return;
        break;
        case 2u:
            {
                UINT8 spawned_tile_coll = TranslateSprite(THIS, spawned_data->vx << delta_time, spawned_data->vy << delta_time);
                if(spawned_data->vy <= 0){
                    if(spawned_data->frmskip == 0){
                        spawned_data->vy++;
                        spawned_data->frmskip = ITEMSPAWNED_FRAMESKIP_MAX;
                    }else{
                        spawned_data->frmskip--;
                    }
                }
                if(spawned_tile_coll || spawned_data->vy > 0){
                    if(spawned_data->frmskip == 0){spawned_data->vx = 1;
                            spawned_data->frmskip = ITEMSPAWNED_FRAMESKIP_MAX;
                    }else{
                        spawned_data->frmskip--;
                        spawned_data->vx = 0;
                    }
                }
                spawned_data->hp--;
                if(spawned_data->hp == 0){
                    if(spawned_data->itemtype == INVITEM_CROSSBOW){SetSpriteAnim(THIS, itmspwn_anim_crossb_dis, 20u);}
                    if(spawned_data->itemtype == INVITEM_MONEY){SetSpriteAnim(THIS, itmspwn_anim_money_dis, 20u);}
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