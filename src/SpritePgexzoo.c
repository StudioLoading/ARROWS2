#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas_npc.h"
#include "Dialogs.h"

const UINT8 whead1_anim_idle[] = {1, 0}; //The first number indicates the number of frames
const UINT8 whead2_anim_idle[] = {1, 1}; //The first number indicates the number of frames
const UINT8 wbody1_anim_idle[] = {3, 2, 2, 3}; //The first number indicates the number of frames
const UINT8 wbody2_anim_idle[] = {3, 4, 4, 5}; //The first number indicates the number of frames


void START(){
    struct NpcInfo* npc_data = (struct NpcInfo*) THIS->custom_data;
    npc_data->configured = 0;
    THIS->lim_x = 255u;
}

void UPDATE(){
    struct NpcInfo* npc_data = (struct NpcInfo*) THIS->custom_data;
    switch(npc_data->configured){
        case 0u:
            return;
        break;
        case 1u:
            switch(npc_data->type){
                case WOMAN_HEAD1:
                    SetSpriteAnim(THIS, whead1_anim_idle, 4u);
                break;
                case WOMAN_HEAD2:
                    SetSpriteAnim(THIS, whead2_anim_idle, 4u);
                break;
                case WOMAN_BODY1:
                    SetSpriteAnim(THIS, wbody1_anim_idle, 2u);
                break;
                case WOMAN_BODY2:
                    SetSpriteAnim(THIS, wbody2_anim_idle, 2u);
                break;
            }
            npc_data->configured = 2u;
        break;
    }
}

void DESTROY(){

}