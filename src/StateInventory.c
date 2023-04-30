#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Sound.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "InventoryDialogs.h"

IMPORT_TILES(fontbw);
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

const UINT8 collision_tiles_inv[] = {1, 2, 0};

UINT8 invcursor_posx[] = {8u, 32u, 56u, 8u, 32u, 56u,8u, 32u, 56u, 80u, 104u, 128u};
UINT8 invcursor_posy[] = {0u, 0u, 0u, 24u, 24u, 24u, 104u,104u,104u,104u,104u,104u,};
INT8 invcursor_posi = 0u;
UINT8 invcursor_old_posi = 0u;
const INT8 invcursor_posimax = 12;
Sprite* inv_cursor = 0;
const INT8 invcursor_unequip_posimax = 6;
INT8 invcursor_unequip_posi = 0u;
UINT8 invcursor_unequip_old_posi = 0u;
UINT8 invcursor_unequip_posx[] = {8u, 32u, 56u, 80u, 104u, 128u};
UINT8 invcursor_unequip_posy = 104u;

extern struct InvItem inventory[12];
extern struct InvItem itemEquipped;
extern UINT8 previous_state;

void invselectitem(INT8 max_idx) BANKED;
void fixInvcursor(INT8 max_idx) BANKED;
void fixUnequipInvcursor() BANKED;
void change_navigation();
void refresh_equipped();
void pickup(struct ItemSpawned* pickedup_data) BANKED;
void change_detail();
UINT8 get_quantity(INVITEMTYPE itemtype) BANKED;
INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;

extern void change_cursor(UINT8 square_or_arrow) BANKED;
extern void ChangeState(UINT8 new_state) BANKED;
extern void Inv_change_detail(UINT8 item, UINT8 isEmpty) BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;

void START(){
	//SOUND
        NR52_REG = 0x80; //Enables sound, you should always setup this first
        NR51_REG = 0xFF; //Enables all channels (left and right)
        NR50_REG = 0x77; //Max volume
    //SPRITES SPAWNING 
        HIDE_WIN;
        inv_cursor = SpriteManagerAdd(SpriteInvcursor, 8u, 24u);
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 10u << 3, (UINT16) 9u << 3);    
        invcursor_old_posi = invcursor_posi;
        inv_cursor->x = invcursor_posx[invcursor_old_posi];
        inv_cursor->y = invcursor_posy[invcursor_old_posi];
        UINT8 isEmpty = inventory[invcursor_posi].quantity == 0 ? 1 : 0;
        Sprite* s_invitem = 0;
        for(UINT8 i = 0u; i < 6; i++){
            if(inventory[i].quantity > 0){
                s_invitem = SpriteManagerAdd(SpriteInvitem, invcursor_posx[i],invcursor_posy[i]+16u);
                struct InvItem* cdata = (struct InvItem*) s_invitem->custom_data;
                cdata->itemtype = inventory[i].itemtype;
                cdata->quantity = inventory[i].quantity;
            }
        }
        invselectitem(12);
        Inv_change_detail(inventory[invcursor_posi].itemtype, isEmpty);
        change_detail();    
        InitScroll(BANK(inventorymap), &inventorymap, collision_tiles_inv, 0);
    //HUD
        INIT_FONT(fontbw, PRINT_BKG);
        INIT_HUD(invwindowmap);
        UINT8 uneq_x = 1u;
        for(UINT8 i = 6u; i < 12; i++){
            switch(inventory[i].itemtype){
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
                case INVITEM_METAL_SPECIAL:
                    UPDATE_HUD_TILE(uneq_x,1,19);
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
                case INVITEM_POWDER:
                    UPDATE_HUD_TILE(uneq_x,1,20);
                    UPDATE_HUD_TILE(uneq_x,2,21);
                    uneq_x += 1;
                    UPDATE_HUD_TILE(uneq_x,1,22);
                    UPDATE_HUD_TILE(uneq_x,2,23);
                break;
                case INVITEM_LETTER:
                    UPDATE_HUD_TILE(uneq_x,1,24);
                    UPDATE_HUD_TILE(uneq_x,2,25);
                    uneq_x += 1;
                    UPDATE_HUD_TILE(uneq_x,1,26);
                    UPDATE_HUD_TILE(uneq_x,2,27);
                break;
            }
            uneq_x += 2;
        }
    refresh_equipped();
    SHOW_BKG;
	SHOW_SPRITES;
}

void pickup(struct ItemSpawned* pickedup_data) BANKED{
    UINT8 item_added = 0u;
    UINT8 sfx_played = 0u;
    if(pickedup_data->itemtype == INVITEM_HEART){
        my_play_fx(CHANNEL_1, 60, 0x76, 0x7a, 0xe9, 0x5a, 0x86);//SFX_HEART
        sfx_played = 1u;
        if(motherpl_hp < 5){
            motherpl_hp++;
        }
        item_added = 1u;
    }
    if(pickedup_data->itemtype == INVITEM_CROSSBOW){
        motherpl_hp = 5;
        item_added = 1u;
        my_play_fx(CHANNEL_1, 60, 0x26, 0xba, 0xe9, 0x06, 0x87);//SFX_CROSSBOW
        sfx_played = 1u;
    }
    if(item_added == 0){
        for(UINT8 i = 0; item_added == 0u && i<12; ++i){
            if(inventory[i].itemtype == pickedup_data->itemtype){
                item_added = 1u;
                if((inventory[i].quantity + pickedup_data->quantity) > 999){
                    inventory[i].quantity = 999;
                }else{
                    inventory[i].quantity += pickedup_data->quantity;
                }
            }
        }
    }
    if(item_added == 0){//non l'ho ancora aggiunto perché nuovo
        switch(pickedup_data->equippable){
            case 1u://equippable
                for(UINT8 i = 0; item_added == 0u && i<6; ++i){
                    if(inventory[i].itemtype == INVITEM_UNASSIGNED){
                        item_added = 1u;
                        inventory[i].itemtype = pickedup_data->itemtype; 
                        inventory[i].quantity = pickedup_data->quantity;
                        inventory[i].equippable = pickedup_data->equippable; 
                    }
                }
            break;
            case 0u://not equippable
                for(UINT8 j = 6; item_added == 0u && j<12; ++j){
                    if(inventory[j].itemtype == INVITEM_UNASSIGNED){
                        item_added = 1u;
                        inventory[j].itemtype = pickedup_data->itemtype; 
                        inventory[j].quantity = pickedup_data->quantity;
                        inventory[j].equippable = pickedup_data->equippable; 
                    }
                }
            break;
        }
    }
    if(item_added == 0u){//non ancora aggiunto causa mancanza di posto

    }
    //SFX
        if(item_added == 1 && sfx_played == 0u){
			my_play_fx(CHANNEL_1, 60, 0x74, 0x94, 0x8f, 0x73, 0x86);//SFX_ITEM
        }
}

void invselectitem(INT8 max_idx) BANKED{
    UINT8 qOk = 0u;
    while (qOk == 0u){
        if(inventory[invcursor_posi].quantity > 0){qOk = 1u;}
        else{
            invcursor_posi++;
            fixInvcursor(max_idx);
            if(inventory[invcursor_posi].quantity > 0){qOk = 1u;}
        }
    }
    if(invcursor_posi < 6 && inventory[invcursor_posi].quantity > 0){
        itemEquipped = inventory[invcursor_posi];
    }
}

void UPDATE(){
    if(KEY_PRESSED(J_START)){
        ChangeState(previous_state);
    }
    if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){
        if(invcursor_posi < 6 && inventory[invcursor_posi].quantity > 0){
            invselectitem(6);
        }
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
        fixInvcursor(12);
        refresh_equipped();
    }
}

void refresh_equipped(){
    invcursor_old_posi = invcursor_posi;
    inv_cursor->x = (UINT16) invcursor_posx[invcursor_posi];
    inv_cursor->y = (UINT16) invcursor_posy[invcursor_posi];
    UINT8 isEmpty = inventory[invcursor_posi].quantity == 0 ? 1 : 0;
    Inv_change_detail(inventory[invcursor_posi].itemtype, isEmpty);
    change_detail();
}

void fixInvcursor(INT8 max_idx) BANKED{
    if(invcursor_posi < 0){
        if(max_idx){
            invcursor_posi = max_idx;
        }else{
            invcursor_posi = 6;
        }
    }
    if(invcursor_posi >= invcursor_posimax || invcursor_posi > max_idx){
        invcursor_posi = 0;        
    }
}

void change_detail(){
    print_target = PRINT_BKG;
    if(inventory[invcursor_posi].equippable){
        PRINT(3, 8, "   ");
        if(inventory[invcursor_posi].quantity < 10){
            PRINT(2, 14, "X 00%u", inventory[invcursor_posi].quantity);
        } else if(inventory[invcursor_posi].quantity < 100){
            PRINT(2, 14, "X 0%u", inventory[invcursor_posi].quantity);
        } else {
            PRINT(2, 14, "X %u", inventory[invcursor_posi].quantity);
        }
    }else{
        PRINT(2, 14, "     ");
        if(inventory[invcursor_posi].quantity < 10){
            PRINT(3, 8, "X0%u", inventory[invcursor_posi].quantity);
        } else if(inventory[invcursor_posi].quantity < 100){
            PRINT(3, 8, "X%u", inventory[invcursor_posi].quantity);
        } else {
            PRINT(3, 8, "%u", inventory[invcursor_posi].quantity);
        }
    }    
    if(inventory[invcursor_posi].quantity == 0){
        GetLocalizedINVLabel_EN(INV_EMPTY_STRING, ddinv1);
        GetLocalizedINVLabel_EN(INV_EMPTY_STRING, ddinv2);
        GetLocalizedINVLabel_EN(INV_EMPTY_STRING, ddinv3);
        GetLocalizedINVLabel_EN(INV_EMPTY_STRING, ddinv4);
        GetLocalizedINVLabel_EN(INV_EMPTY_STRING, ddinv5);
    }else{
        switch(inventory[invcursor_posi].itemtype){
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
            case INVITEM_METAL:
                GetLocalizedINVLabel_EN(METAL_NAME, ddinv1);
                GetLocalizedINVLabel_EN(METAL_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(METAL_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(METAL_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(METAL_DETAIL4, ddinv5);
            break;
            case INVITEM_WOOD:
                GetLocalizedINVLabel_EN(WOOD_NAME, ddinv1);
                GetLocalizedINVLabel_EN(WOOD_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(WOOD_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(WOOD_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(WOOD_DETAIL4, ddinv5);
            break;
            case INVITEM_POWDER:
                GetLocalizedINVLabel_EN(POWDER_NAME, ddinv1);
                GetLocalizedINVLabel_EN(POWDER_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(POWDER_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(POWDER_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(POWDER_DETAIL4, ddinv5);
            break;
            case INVITEM_LETTER:
                GetLocalizedINVLabel_EN(LETTER_NAME, ddinv1);
                GetLocalizedINVLabel_EN(LETTER_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(LETTER_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(LETTER_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(LETTER_DETAIL4, ddinv5);
            break;
        }
    }
    PRINT(8, 8, "%s", ddinv1);
    PRINT(8, 10, "%s", ddinv2);
    PRINT(8, 11, "%s", ddinv3);
    PRINT(8, 12, "%s", ddinv4);
    PRINT(8, 13, "%s", ddinv5);
}

UINT8 get_quantity(INVITEMTYPE itemtype) BANKED{
    UINT8 q = 0u;
    for(UINT8 i = 0u; i < 12; i++){
        if(itemtype == inventory[i].itemtype){
            q = inventory[i].quantity;
        }
    }
    return q;
}

INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED{
    INT8 idx = -1;
    for(UINT8 i = 0u; i < 12; i++){
        if(itemtype == inventory[i].itemtype){
            if(l < 0){
                if(inventory[i].quantity > 0){
                    idx = i;
                }
            }else{
                idx = i;
            }
            i = 12;
        }
    }
    if(idx > -1){inventory[idx].quantity+=l;}else{idx = 0;}
    if(inventory[idx].quantity == 0){
        inventory[idx].itemtype = INVITEM_UNASSIGNED; 
    }
    return inventory[idx].quantity;
}