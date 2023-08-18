#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_glasses[] = {4, 0, 1, 0, 2 };

void START(){
    struct ItemSpawned* glasses_data = (struct ItemSpawned*) THIS->custom_data;
    glasses_data->itemtype = INVITEM_GLASSES;
    glasses_data->quantity = 1;
    glasses_data->equippable = 0;
    SetSpriteAnim(THIS,a_glasses, 16u);
}

void UPDATE(){
}

void DESTROY(){
    SpriteManagerAdd(SpriteGocursor, 1u << 3, 6u << 3);
}