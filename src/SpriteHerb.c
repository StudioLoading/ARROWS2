#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_herb[] = {2, 0,1}; //The first number indicates the number of frames

void START(){
	THIS->lim_x = 6000u;
	THIS->lim_y = 6000u;
    struct ItemSpawned* herb_data = (struct ItemSpawned*) THIS->custom_data;
    herb_data->itemtype = INVITEM_HERB;
    herb_data->quantity = 1;
    herb_data->equippable = 0;
    SetSpriteAnim(THIS, a_herb, 16u);
}

void UPDATE(){
    
}

void DESTROY(){

}