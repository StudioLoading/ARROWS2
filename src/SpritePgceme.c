#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 whead1_cemetery[] = {1, 0}; //The first number indicates the number of frames
const UINT8 whead2_cemetery[] = {1, 6}; //The first number indicates the number of frames
const UINT8 wbody1_cemetery[] = {1, 1}; //The first number indicates the number of frames
const UINT8 wbody2_cemetery[] = {1, 7}; //The first number indicates the number of frames
const UINT8 mhead1_cemetery[] = {1, 4}; //The first number indicates the number of frames
const UINT8 mbody1_cemetery[] = {1, 5}; //The first number indicates the number of frames

extern UINT8 np_counter;

void START(){
    struct NpcInfo* npc_data = (struct NpcInfo*) THIS->custom_data;
    npc_data->configured = 0;
    npc_data->whotalks = NOBODY;
    THIS->lim_x = 100u;
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
                    SetSpriteAnim(THIS, whead1_cemetery, 4u);
                break;
                case WOMAN_HEAD2:
                    SetSpriteAnim(THIS, whead2_cemetery, 4u);
                break;
                case WOMAN_BODY1:
                    SetSpriteAnim(THIS, wbody1_cemetery, 2u);
                break;
                case WOMAN_BODY2:
                    SetSpriteAnim(THIS, wbody2_cemetery, 2u);
                break;
                case MAN_HEAD1:
                    SetSpriteAnim(THIS, mhead1_cemetery, 4u);
                break;
                case MAN_BODY1:
                    SetSpriteAnim(THIS, mbody1_cemetery, 2u);
                break;
            }
            npc_data->configured = 2u;
        break;
    }
}

void DESTROY(){
    np_counter--;
}