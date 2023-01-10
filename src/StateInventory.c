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
IMPORT_MAP(invwindowmap);

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
extern INT8 motherpl_hp;
extern INT8 motherpl_ups;

const UINT8 collision_tiles_inv[] = {1, 2, 0};

UINT8 invcursor_posx[] = {8u, 32u, 56u, 8u, 32u, 56u,8u, 32u, 56u, 80u, 104u, 128u};
UINT8 invcursor_posy[] = {0u, 0u, 0u, 24u, 24u, 24u, 104u,104u,104u,104u,104u,104u,};
INT8 invcursor_posi = 1u;
UINT8 invcursor_old_posi = 0u;
const INT8 invcursor_posimax = 12;
Sprite* inv_cursor = 0;
const INT8 invcursor_unequip_posimax = 6;
INT8 invcursor_unequip_posi = 0u;
UINT8 invcursor_unequip_old_posi = 0u;
UINT8 invcursor_unequip_posx[] = {8u, 32u, 56u, 80u, 104u, 128u};
UINT8 invcursor_unequip_posy = 104u;


UINT8 nav_equippable = 0u;

struct InvItem itemMoney = {.itemtype = INVITEM_MONEY, .quantity = 100, .equippable = 1u};
struct InvItem item00 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 1u};
struct InvItem item01 = {.itemtype = INVITEM_ARROW_NORMAL, .quantity = 100, .equippable = 1u};
struct InvItem item02 = {.itemtype = INVITEM_ARROW_PERFO, .quantity = 100, .equippable = 1u};
struct InvItem item03 = {.itemtype = INVITEM_ARROW_BASTARD, .quantity = 100, .equippable = 1u};
struct InvItem item04 = {.itemtype = INVITEM_BOMB, .quantity = 0, .equippable = 1u};
struct InvItem unequip00 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 0u};
struct InvItem unequip01 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 0u};
struct InvItem unequip02 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 0u};
struct InvItem unequip03 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 0u};
struct InvItem unequip04 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 0u};
struct InvItem unequip05 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 0u};
struct InvItem* inventory[12] = {&itemMoney, &item00, &item01, &item02, &item03, &item04,
                            &unequip00, &unequip01, &unequip02, &unequip03, &unequip04, &unequip05};
extern struct InvItem* itemEquipped;

void invselectitem() BANKED;
void fixInvcursor() BANKED;
void fixUnequipInvcursor() BANKED;
void change_navigation();
void refresh_equipped();

extern void change_cursor(UINT8 square_or_arrow) BANKED;
void pickup(struct InvItem* pickedup_data) BANKED;

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
        s_invitem = SpriteManagerAdd(SpriteInvitem, invcursor_posx[i],invcursor_posy[i]+16u);
        struct InvItem* cdata = (struct InvItem*) s_invitem->custom_data;
        cdata->itemtype = inventory[i]->itemtype;
        cdata->quantity = inventory[i]->quantity;
        cdata->configured = 1u;
    }
    invselectitem();
    Inv_change_detail(inventory[invcursor_posi]->itemtype, isEmpty);
    change_detail();

    //HUD
    INIT_HUD(invwindowmap);
    UINT8 uneq_x = 1u;
    for(UINT8 i = 6u; i < 12; i++){
        struct InvItem* unequippable = (struct InvItem*) inventory[i];
        switch(unequippable->itemtype){
            case INVITEM_UNASSIGNED:
                UPDATE_HUD_TILE(uneq_x,1,10);
                UPDATE_HUD_TILE(uneq_x,2,10);
                uneq_x += 1;
                UPDATE_HUD_TILE(uneq_x,1,10);
                UPDATE_HUD_TILE(uneq_x,2,10);
            break;
            case INVITEM_CROSSBOW:
                UPDATE_HUD_TILE(uneq_x,1,6);
                UPDATE_HUD_TILE(uneq_x,2,7);
                uneq_x += 1;
                UPDATE_HUD_TILE(uneq_x,1,8);
                UPDATE_HUD_TILE(uneq_x,2,9);
            break;
            case INVITEM_METAL:
                UPDATE_HUD_TILE(uneq_x,1,11);
                UPDATE_HUD_TILE(uneq_x,2,12);
                uneq_x += 1;
                UPDATE_HUD_TILE(uneq_x,1,13);
                UPDATE_HUD_TILE(uneq_x,2,14);
            break;
            case INVITEM_WOOD:
                UPDATE_HUD_TILE(uneq_x,1,15);
                UPDATE_HUD_TILE(uneq_x,2,16);
                uneq_x += 1;
                UPDATE_HUD_TILE(uneq_x,1,17);
                UPDATE_HUD_TILE(uneq_x,2,18);
            break;
        }
        uneq_x += 2;
    }

}

void pickup(struct InvItem* pickedup_data) BANKED{
    UINT8 item_added = 0u;
    if(pickedup_data->itemtype == INVITEM_HEART){
        if(motherpl_hp < 4){
            motherpl_hp++;
        }
        item_added = 1u;
    }
    if(pickedup_data->itemtype == INVITEM_CROSSBOW){
        if(motherpl_ups < 99){
            motherpl_ups++;
        }
        item_added = 1u;
    }
    if(item_added == 0){
        for(UINT8 i = 0; item_added == 0u && i<12; ++i){
            if(inventory[i]->itemtype == pickedup_data->itemtype){
                item_added = 1u;
                if((inventory[i]->quantity + pickedup_data->quantity) > 999){
                    inventory[i]->quantity = 999;
                }else{
                    inventory[i]->quantity += pickedup_data->quantity;
                }
            }
        }
    }
    if(item_added == 0){//non l'ho ancora aggiunto perché nuovo
        switch(pickedup_data->equippable){
            case 1u://equippable
                for(UINT8 i = 0; item_added == 0u && i<6; ++i){
                    if(inventory[i]->itemtype == INVITEM_UNASSIGNED){
                        item_added = 1u;
                        inventory[i]->itemtype = pickedup_data->itemtype; 
                        inventory[i]->quantity = pickedup_data->quantity; 
                        inventory[i]->configured = pickedup_data->configured;
                        inventory[i]->hp = pickedup_data->hp; 
                        inventory[i]->vx = pickedup_data->vx; 
                        inventory[i]->vy = pickedup_data->vy; 
                        inventory[i]->frmskip = pickedup_data->frmskip; 
                        inventory[i]->equippable = pickedup_data->equippable; 
                    }
                }
            break;
            case 0u://not equippable
                for(UINT8 j = 6; item_added == 0u && j<12; ++j){
                    if(inventory[j]->itemtype == INVITEM_UNASSIGNED){
                        item_added = 1u;
                        inventory[j]->itemtype = pickedup_data->itemtype; 
                        inventory[j]->quantity = pickedup_data->quantity; 
                        inventory[j]->configured = pickedup_data->configured;
                        inventory[j]->hp = pickedup_data->hp; 
                        inventory[j]->vx = pickedup_data->vx; 
                        inventory[j]->vy = pickedup_data->vy; 
                        inventory[j]->frmskip = pickedup_data->frmskip; 
                        inventory[j]->equippable = pickedup_data->equippable; 
                    }
                }
            break;
        }
    }
    if(item_added == 0u){//non ancora aggiunto causa mancanza di posto

    }
}

void invselectitem() BANKED{
    UINT8 qOk = 0u;
    while (qOk == 0u){
        if(inventory[invcursor_posi]->quantity > 0){qOk = 1u;}
        else{
            invcursor_posi++;
            fixInvcursor();
            if(inventory[invcursor_posi]->quantity > 0){qOk = 1u;}
        }
    }
    
    itemEquipped = inventory[invcursor_posi];
}

void UPDATE(){
    if(KEY_PRESSED(J_START)){
        SetState(StateExzoo);
    }
    
    switch(nav_equippable){
        case 0u:
            if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){
                invselectitem();
            }
            if(KEY_RELEASED(J_UP)){
                invcursor_posi-=3;      
                if(invcursor_posi > 5){
                    invcursor_posi = 0u;
                }   
            }
            if(KEY_RELEASED(J_DOWN)){
                invcursor_posi+=3;
                if(invcursor_posi > 8){
                    invcursor_posi -= 9u;
                }
            }
            if(KEY_RELEASED(J_RIGHT)){
                invcursor_posi++;
            }
            if(KEY_RELEASED(J_LEFT)){
                invcursor_posi--;
            }
            if(invcursor_old_posi != invcursor_posi){//muovo cursor verso prossima posizione
                fixInvcursor();
                refresh_equipped();
            }
        break;
    }
}

void refresh_equipped(){
    invcursor_old_posi = invcursor_posi;
    inv_cursor->x = (UINT16) invcursor_posx[invcursor_posi];
    inv_cursor->y = (UINT16) invcursor_posy[invcursor_posi];
    UINT8 isEmpty = inventory[invcursor_posi]->quantity == 0 ? 1 : 0;
    Inv_change_detail(inventory[invcursor_posi]->itemtype, isEmpty);
    change_detail();
}

void fixInvcursor() BANKED{
    if(invcursor_posi < 0){
        invcursor_posi = 6;
    }
    if(invcursor_posi >= invcursor_posimax){
        invcursor_posi = 0;        
    }
}

void change_detail(){
    print_target = PRINT_BKG;
    if(inventory[invcursor_posi]->quantity == 0){
        if(inventory[invcursor_posi]->equippable){PRINT(2, 14, "     ");}
        else{PRINT(2, 14, "     ");PRINT(3, 8, "   ");}
    }
    else if(inventory[invcursor_posi]->quantity < 10){
        if(inventory[invcursor_posi]->equippable){PRINT(2, 14, "X 00%u", inventory[invcursor_posi]->quantity);}
        else{PRINT(2, 14, "     ");PRINT(3, 8, "X0%u", inventory[invcursor_posi]->quantity);}
    }
    else if(inventory[invcursor_posi]->quantity < 100){
        if(inventory[invcursor_posi]->equippable){PRINT(2, 14, "X 0%u", inventory[invcursor_posi]->quantity);}
        else{PRINT(2, 14, "     ");PRINT(3, 8, "X%u", inventory[invcursor_posi]->quantity);}
    }
    else {
        if(inventory[invcursor_posi]->equippable){PRINT(2, 14, "X %u", inventory[invcursor_posi]->quantity);}
        else{PRINT(2, 14, "     ");PRINT(3, 8, "%u", inventory[invcursor_posi]->quantity);}
    }
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