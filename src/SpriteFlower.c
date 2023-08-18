#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_flower[] = {4, 0,1,0,0}; //The first number indicates the number of frames

void START(){
	THIS->lim_x = 255u;
	THIS->lim_y = 255u;
    struct ItemSpawned* flower_data = (struct ItemSpawned*) THIS->custom_data;
    flower_data->itemtype = INVITEM_FLOWER;
    flower_data->quantity = 1;
    flower_data->equippable = 0;
    SetSpriteAnim(THIS, a_flower, 8u);
}

void UPDATE(){
    
}

void DESTROY(){

}