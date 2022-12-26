#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "BankManager.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Fade.h"
#include "Music.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "InventoryDialogs.h"

IMPORT_TILES(fontbw);
DECLARE_MUSIC(bgm_credits);
IMPORT_MAP(border);
IMPORT_MAP(inventorymap);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

extern UINT8 border_set_diary;
extern UINT8 border_set_ow;
extern UINT8 current_map;//0=south-west, 1=south-east, 2=north-west, 3=north-east
extern unsigned char ddinv1[];
extern unsigned char ddinv2[];
extern unsigned char ddinv3[];
extern unsigned char ddinv4[];
extern unsigned char ddinv5[];
extern unsigned char ddinv6[];
extern unsigned char ddinv7[];
extern unsigned char ddinv8[];

const UINT8 collision_tiles_inv[] = {1, 2, 0};

UINT8 invcursor_posx[] = {8u, 32u, 56u, 8u, 32u, 56u};
UINT8 invcursor_posy[] = {16u, 16u, 16u, 40u, 40u, 40u};
INT8 invcursor_posi = 0u;
UINT8 invcursor_old_posi = 0u;
const INT8 invcursor_posimax = 6;
Sprite* inv_cursor = 0;
struct InvItem itemCrossbow = {.itemtype = INVITEM_CROSSBOW, .quantity = 0};
struct InvItem itemMoney = {.itemtype = INVITEM_MONEY, .quantity = 100};
struct InvItem itemArrowNormal = {.itemtype = INVITEM_ARROW_NORMAL, .quantity = 0};
struct InvItem itemArrowPerf = {.itemtype = INVITEM_ARROW_PERFO, .quantity = 0};
struct InvItem itemArrowBastard = {.itemtype = INVITEM_ARROW_BASTARD, .quantity = 0};
struct InvItem itemBomb = {.itemtype = INVITEM_BOMB, .quantity = 0};
const struct InvItem* inventory[6] = {&itemCrossbow, &itemMoney, &itemBomb, &itemArrowNormal, &itemArrowPerf, &itemArrowBastard};


void invselectitem();

void START(){
	/*if(border_set_diary == 0u){
        border_set_diary = 1u;
        LOAD_SGB_BORDER(border);        
	}*/
    LOAD_SGB_BORDER(border);        
    HIDE_WIN;

	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume
	//PlayMusic(bgm_credits, 0);
    
    //SGB palette
    if(sgb_check()){
        set_sgb_palette_2();
    }
    //scroll_target = 
    inv_cursor = SpriteManagerAdd(SpriteInvcursor, 8u, 24u);
	InitScroll(BANK(inventorymap), &inventorymap, collision_tiles_inv, 0);
    scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 10u << 3, (UINT16) 9u << 3);
	INIT_FONT(fontbw, PRINT_BKG);
    SHOW_BKG;
    
    invcursor_old_posi = invcursor_posi;
    inv_cursor->x = invcursor_posx[invcursor_old_posi];
    inv_cursor->y = invcursor_posy[invcursor_old_posi];
    UINT8 isEmpty = inventory[invcursor_posi]->quantity == 0 ? 1 : 0;
    Sprite* s_invitem = 0;
    for(UINT8 i = 0u; i < 6; i++){
        s_invitem = SpriteManagerAdd(SpriteInvitem, invcursor_posx[i],invcursor_posy[i]);
        struct InvItem* cdata = (struct InvItem*) s_invitem->custom_data;
        cdata->itemtype = inventory[i]->itemtype;
        cdata->quantity = inventory[i]->quantity;
        cdata->configured = 1u;
    }
    Inv_change_detail(inventory[invcursor_posi]->itemtype, isEmpty);
    change_detail();
}

void invselectitem(){

}

void UPDATE(){
    if(KEY_PRESSED(J_START)){
        SetState(StateExzoo);
    }
    
    if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){
        invselectitem();
    }
    if(KEY_RELEASED(J_UP)){
        invcursor_posi-=3;
    }
    if(KEY_RELEASED(J_DOWN)){
        invcursor_posi+=3;
    }
    if(KEY_RELEASED(J_RIGHT)){
        invcursor_posi++;
    }
    if(KEY_RELEASED(J_LEFT)){
        invcursor_posi--;
    }
    if(invcursor_posi < 0){
        invcursor_posi = invcursor_posimax +invcursor_posi;
    }
    if(invcursor_posi >= invcursor_posimax){
        invcursor_posi = invcursor_posi-invcursor_posimax;
    }
    if(invcursor_old_posi != invcursor_posi){//muovo cursor verso prossima posizione
        invcursor_old_posi = invcursor_posi;
        inv_cursor->x = invcursor_posx[invcursor_posi];
        inv_cursor->y = invcursor_posy[invcursor_posi];
        UINT8 isEmpty = inventory[invcursor_posi]->quantity == 0 ? 1 : 0;
        Inv_change_detail(inventory[invcursor_posi]->itemtype, isEmpty);
        change_detail();
    }
}

void change_detail(){
    print_target = PRINT_BKG;
    if(inventory[invcursor_posi]->quantity == 0){PRINT(2, 14, "     ");}
    else if(inventory[invcursor_posi]->quantity < 10){PRINT(2, 14, "X 00%u", inventory[invcursor_posi]->quantity);}
    else if(inventory[invcursor_posi]->quantity < 100){PRINT(2, 14, "X 0%u", inventory[invcursor_posi]->quantity);}
    else {PRINT(2, 14, "X %u", inventory[invcursor_posi]->quantity);}
    if(inventory[invcursor_posi]->quantity == 0){
        GetLocalizedINVLabel_EN(INV_EMPTY_STRING, ddinv1);
        GetLocalizedINVLabel_EN(INV_EMPTY_STRING, ddinv2);
        GetLocalizedINVLabel_EN(INV_EMPTY_STRING, ddinv3);
        GetLocalizedINVLabel_EN(INV_EMPTY_STRING, ddinv4);
        GetLocalizedINVLabel_EN(INV_EMPTY_STRING, ddinv5);
    }else{
        switch(inventory[invcursor_posi]->itemtype){
            case INVITEM_CROSSBOW:
                GetLocalizedINVLabel_EN(CROSSBOW_NAME, ddinv1);
                GetLocalizedINVLabel_EN(CROSSBOW_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(CROSSBOW_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(CROSSBOW_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(CROSSBOW_DETAIL4, ddinv5);
            break;
            case INVITEM_MONEY:
                GetLocalizedINVLabel_EN(MONEY_NAME, ddinv1);
                GetLocalizedINVLabel_EN(MONEY_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(MONEY_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(MONEY_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(MONEY_DETAIL4, ddinv5);
            break;
            case INVITEM_BOMB:
                GetLocalizedINVLabel_EN(BOMB_NAME, ddinv1);
                GetLocalizedINVLabel_EN(BOMB_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(BOMB_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(BOMB_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(BOMB_DETAIL4, ddinv5);
            break;
            case INVITEM_ARROW_NORMAL:
                GetLocalizedINVLabel_EN(ARROWNORMAL_NAME, ddinv1);
                GetLocalizedINVLabel_EN(ARROWNORMAL_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(ARROWNORMAL_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(ARROWNORMAL_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(ARROWNORMAL_DETAIL4, ddinv5);
            break;
            case INVITEM_ARROW_PERFO:
                GetLocalizedINVLabel_EN(ARROWPERFO_NAME, ddinv1);
                GetLocalizedINVLabel_EN(ARROWPERFO_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(ARROWPERFO_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(ARROWPERFO_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(ARROWPERFO_DETAIL4, ddinv5);
            break;
            case INVITEM_ARROW_BASTARD:
                GetLocalizedINVLabel_EN(ARROWBASTARD_NAME, ddinv1);
                GetLocalizedINVLabel_EN(ARROWBASTARD_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(ARROWBASTARD_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(ARROWBASTARD_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(ARROWBASTARD_DETAIL4, ddinv5);
            break;
        }
    }
    PRINT(8, 8, "%s", ddinv1);
    PRINT(8, 10, "%s", ddinv2);
    PRINT(8, 11, "%s", ddinv3);
    PRINT(8, 12, "%s", ddinv4);
    PRINT(8, 13, "%s", ddinv5);
}