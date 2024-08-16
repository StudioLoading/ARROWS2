#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Palette.h"

#include "custom_datas.h"

const UINT8 a_captain[] = {3, 1,1,11}; //The first number indicates the number of frames
const UINT8 a_spugna[] = {3, 4,4,5}; //The first number indicates the number of frames
const UINT8 a_panzone[] = {5, 2,2,3,2,3}; //The first number indicates the number of frames
const UINT8 a_marinaio[] = {5, 6,6,6,6,7}; //The first number indicates the number of frames
const UINT8 a_marinaio_walker[] = {4, 8,9,8,10}; //The first number indicates the number of frames

extern UINT8 pirate_counter_30_100;

void START(){
    struct NpcInfo* npc_data = (struct NpcInfo*) THIS->custom_data;
    npc_data->configured = 0;
    npc_data->whotalks = NOBODY;
    THIS->lim_x = 1000u;
    THIS->lim_y = 1000u;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE(){
    struct NpcInfo* npc_data = (struct NpcInfo*) THIS->custom_data;
    switch(npc_data->configured){
        case 0u:
            return;
        break;
        case 1u:
            switch(npc_data->type){
                case PIRATE_CAPTAIN:
                    SetSpriteAnim(THIS, a_captain, 8u);
                break;
                case PIRATE_MARINAIO:
                    SetSpriteAnim(THIS, a_marinaio, 4u);
                break;
                case PIRATE_PANZONE:
                    SetSpriteAnim(THIS, a_panzone, 2u);
                break;
                case PIRATE_WALKER:
                    SetSpriteAnim(THIS, a_marinaio_walker, 16u);
                break;
                case PIRATE_SPUGNA:
                    SetSpriteAnim(THIS, a_spugna, 4u);
                break;
            }
            npc_data->configured = 2u;
        break;
        case 2u:
            switch(npc_data->type){
                case PIRATE_WALKER:
                    if(pirate_counter_30_100 == 30){
                        npc_data->vx = -npc_data->vx;
                        THIS->mirror = NO_MIRROR;
                        if(npc_data->vx < 0){THIS->mirror = V_MIRROR;}
                    }
                    if(npc_data->frameskip == npc_data->max_frameskip){
                        npc_data->frameskip = 0;
                        TranslateSprite(THIS, npc_data->vx, 1 << delta_time);
                    }else{npc_data->frameskip++;}
                break;
                default:
                    TranslateSprite(THIS, 0, 1 << delta_time);
                break;
            }
        break;
    }
}

void DESTROY(){
}