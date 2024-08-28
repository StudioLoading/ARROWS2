#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Palette.h"

#include "custom_datas.h"

const UINT8 a_cadaver[] = {1, 1}; //The first number indicates the number of frames

extern UINT8 cadaver_spawned;

void START(){
    THIS->lim_x = 1000u;
    THIS->lim_y = 400u;
    SetSpriteAnim(THIS, a_cadaver, 1u);
    struct NpcInfo* npc_data = (struct NpcInfo*) THIS->custom_data;
    npc_data->configured = 0;
    npc_data->whotalks = CADAVER;
    npc_data->npcname = RICK;
    cadaver_spawned = 1;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE(){
}

void DESTROY(){
    cadaver_spawned = 0;
}