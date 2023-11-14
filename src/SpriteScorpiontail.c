#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_tail[] = {4, 0, 1, 2, 1 };

void START(){
    struct ItemSpawned* tail_data = (struct ItemSpawned*) THIS->custom_data;
    tail_data->itemtype = INVITEM_SCORPIONTAIL;
    tail_data->quantity = 1;
    tail_data->equippable = 0;
    SetSpriteAnim(THIS,a_tail, 16u);
}

void UPDATE(){
}

void DESTROY(){
    SpriteManagerAdd(SpriteGocursor, 1u << 3, 6u << 3);
}