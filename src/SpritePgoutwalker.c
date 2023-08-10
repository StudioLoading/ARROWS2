#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_whead1[] = {1, 0}; //The first number indicates the number of frames
const UINT8 a_whead2[] = {1, 1}; //The first number indicates the number of frames
const UINT8 a_wbody1[] = {3, 2, 2, 3}; //The first number indicates the number of frames
const UINT8 a_wbody2[] = {3, 4, 4, 5}; //The first number indicates the number of frames
const UINT8 a_wbody3[] = {1, 6}; //The first number indicates the number of frames
const UINT8 a_mhead1[] = {1, 7}; //The first number indicates the number of frames
const UINT8 a_mhead2[] = {1, 9}; //The first number indicates the number of frames
const UINT8 a_mhead3[] = {1, 11}; //The first number indicates the number of frames
const UINT8 a_mbody1[] = {1, 8}; //The first number indicates the number of frames
const UINT8 a_mbody2[] = {1, 10}; //The first number indicates the number of frames


void START(){
    struct NpcInfo* npc_data = (struct NpcInfo*) THIS->custom_data;
    npc_data->configured = 0;
    npc_data->whotalks  =NOBODY;
    THIS->lim_x = 160u;
    THIS->lim_y = 100u;
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
                    SetSpriteAnim(THIS, a_whead1, 4u);
                break;
                case WOMAN_HEAD2:
                    SetSpriteAnim(THIS, a_whead2, 4u);
                break;
                case WOMAN_BODY1:
                    SetSpriteAnim(THIS, a_wbody1, 2u);
                break;
                case WOMAN_BODY2:
                    SetSpriteAnim(THIS, a_wbody2, 2u);
                break;
                case WOMAN_BODY3:
                    SetSpriteAnim(THIS, a_wbody3, 2u);
                break;                
                case MAN_HEAD1:
                    SetSpriteAnim(THIS, a_mhead1, 4u);
                break;
                case MAN_HEAD2:
                    SetSpriteAnim(THIS, a_mhead2, 4u);
                break;
                case MAN_HEAD3:
                    SetSpriteAnim(THIS, a_mhead3, 4u);
                break;
                case MAN_BODY1:
                    SetSpriteAnim(THIS, a_mbody1, 2u);
                break;
                case MAN_BODY2:
                    SetSpriteAnim(THIS, a_mbody2, 2u);
                break;
            }
            npc_data->configured = 2u;
        break;
    }
}

void DESTROY(){

}