#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"


const UINT8 itm_nocrossbow[] = {1, 1}; //The first number indicates the number of frames
const UINT8 itm_crossbow[] = {1, 6}; //The first number indicates the number of frames
const UINT8 itm_money[] = {1, 2}; //The first number indicates the number of frames
const UINT8 itm_noarrownormal[] = {1, 3}; //The first number indicates the number of frames
const UINT8 itm_arrownormal[] = {1, 7}; //The first number indicates the number of frames
const UINT8 itm_noarrowperf[] = {1, 4}; //The first number indicates the number of frames
const UINT8 itm_arrowperf[] = {1, 8}; //The first number indicates the number of frames
const UINT8 itm_nobomb[] = {1, 5}; //The first number indicates the number of frames
const UINT8 itm_bomb[] = {1, 9}; //The first number indicates the number of frames

void START(){

}

void UPDATE(){
    struct InvItem* invitem_data = (struct InvItem*) THIS->custom_data;
    if(invitem_data->configured == 1u){
        invitem_data->configured = 2u;
        switch(invitem_data->itemtype){
            case 0u:
                if(invitem_data->owned == 0)SetSpriteAnim(THIS, itm_nocrossbow, 4u);
                else SetSpriteAnim(THIS, itm_crossbow, 4u);
            break;
            case 1u:
                SetSpriteAnim(THIS, itm_money, 4u);
            break;
            case 2u:
                if(invitem_data->quantity == 0)SetSpriteAnim(THIS, itm_noarrownormal, 4u);
                else SetSpriteAnim(THIS, itm_arrownormal, 4u);
            break;
            case 3u:
                if(invitem_data->quantity == 0)SetSpriteAnim(THIS, itm_noarrowperf, 4u);
                else SetSpriteAnim(THIS, itm_arrowperf, 4u);
            break;
            case 4u:
                if(invitem_data->quantity == 0)SetSpriteAnim(THIS, itm_nobomb, 4u);
                else SetSpriteAnim(THIS, itm_bomb, 4u);
            break;
        }
    }
}

void DESTROY(){

}