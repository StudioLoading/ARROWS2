#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 itm_unassigned[] = {1 ,0};
const UINT8 itm_arrowbastard[] = {1, 4}; //The first number indicates the number of frames
const UINT8 itm_silver[] = {1, 6}; //The first number indicates the number of frames
const UINT8 itm_money[] = {1, 1}; //The first number indicates the number of frames
const UINT8 itm_arrownormal[] = {1, 2}; //The first number indicates the number of frames
const UINT8 itm_arrowperf[] = {1, 3}; //The first number indicates the number of frames
const UINT8 itm_bomb[] = {1, 5}; //The first number indicates the number of frames

void START(){

}

void UPDATE(){
    struct InvItem* invitem_data = (struct InvItem*) THIS->custom_data;
    switch(invitem_data->itemtype){
        case INVITEM_SILVER:
            SetSpriteAnim(THIS, itm_silver, 4u);
        break;
        case INVITEM_MONEY:
            SetSpriteAnim(THIS, itm_money, 4u);
        break;
        case INVITEM_ARROW_NORMAL:
            SetSpriteAnim(THIS, itm_arrownormal, 4u);
        break;
        case INVITEM_ARROW_PERFO:
            SetSpriteAnim(THIS, itm_arrowperf, 4u);
        break;
        case INVITEM_ARROW_BASTARD:
            SetSpriteAnim(THIS, itm_arrowbastard, 4u);
        break;
        case INVITEM_BOMB:
            SetSpriteAnim(THIS, itm_bomb, 4u);
        break;
        case INVITEM_UNASSIGNED:
            SetSpriteAnim(THIS, itm_unassigned, 4u);
        break;
    }
}

void DESTROY(){

}