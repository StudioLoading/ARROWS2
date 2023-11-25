#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 blackiechild_anim[] = {1, 0};


void START(){
    SetSpriteAnim(THIS,blackiechild_anim, 4u);
    THIS->lim_x = 255u;
    THIS->lim_y = 255u;
    struct NpcInfo* s_this_data = (struct NpcInfo*) THIS->custom_data;
    s_this_data->npcname = WOLF_BLACKIE_CHILD;
}

void UPDATE(){
}

void DESTROY(){
}