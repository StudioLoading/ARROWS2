#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Sound.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "InventoryDialogs.h"

IMPORT_TILES(fontbw);
IMPORT_MAP(inventorymap);
IMPORT_MAP(invwindowmap);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

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
INT8 invcursor_posi = 0;
UINT8 invcursor_old_posi = 0u;
const INT8 invcursor_posimax = 12;
Sprite* inv_cursor = 0;
const INT8 invcursor_unequip_posimax = 6;
INT8 invcursor_unequip_posi = 0u;
UINT8 invcursor_unequip_old_posi = 0u;
UINT8 invcursor_unequip_posx[] = {8u, 32u, 56u, 80u, 104u, 128u};
UINT8 invcursor_unequip_posy = 104u;
INT8 blinker_enabled = 1;
INT8 blinker_timeout = 0;
INT8 blinker_timeout_max = 100;
INT8 blinker_idx = 0;
INT8 blinker_idx_max = 3;
INT16 blinker_coords_x[] = {24u, 38u, 30u, 32u};
INT16 blinker_coords_y[] = {80u, 88u, 88u, 96u};
UINT8 blinking_dessa_timeout = 240u;
UINT8 blinking_dessa_counter = 0;
UINT8 blinking_dessa_counter_max = 20;

extern struct InvItem inventory[12];
extern struct InvItem itemEquipped;
extern UINT8 previous_state;
extern WHOSTALKING whostalking;
extern INT8 current_map;
extern struct MISSION enable_hospital;
extern INT8 motherpl_hp;

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
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void Inv_change_detail(UINT8 item, UINT8 isEmpty) BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;
extern UINT8 is_item_equippable(INVITEMTYPE itemtype) BANKED;
extern void LogItem(INVITEMTYPE invitemtype) BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;
extern void manage_bgm(UINT8 new_state, UINT8 previous_state, INT8 next_map) BANKED;

void START(){
    //SPRITES SPAWNING 
        HIDE_WIN;
        inv_cursor = SpriteManagerAdd(SpriteInvcursor, 8u, 24u);
        struct ItemSpawned* invcursor_data = (struct ItemSpawned*)inv_cursor->custom_data;
        invcursor_data->configured = 1;
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
                case INVITEM_METAL:
                    UPDATE_HUD_TILE(uneq_x,1,11);
                    UPDATE_HUD_TILE(uneq_x,2,12);
                    uneq_x += 1;
                    UPDATE_HUD_TILE(uneq_x,1,13);
                    UPDATE_HUD_TILE(uneq_x,2,14);
                break;
                case INVITEM_SILVER:
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
                case INVITEM_PASS:
                case INVITEM_LIAM_HANDWRITTEN:
                    UPDATE_HUD_TILE(uneq_x,1,24);
                    UPDATE_HUD_TILE(uneq_x,2,25);
                    uneq_x += 1;
                    UPDATE_HUD_TILE(uneq_x,1,26);
                    UPDATE_HUD_TILE(uneq_x,2,27);
                break;
                case INVITEM_GLASSES:
                    UPDATE_HUD_TILE(uneq_x,1,28);
                    UPDATE_HUD_TILE(uneq_x,2,29);
                    uneq_x += 1;
                    UPDATE_HUD_TILE(uneq_x,1,30);
                    UPDATE_HUD_TILE(uneq_x,2,31);
                break;
                case INVITEM_SCORPIONTAIL:
                    UPDATE_HUD_TILE(uneq_x,1,36);
                    UPDATE_HUD_TILE(uneq_x,2,37);
                    uneq_x += 1;
                    UPDATE_HUD_TILE(uneq_x,1,38);
                    UPDATE_HUD_TILE(uneq_x,2,39);
                break;
                case INVITEM_FLOWER:
                    UPDATE_HUD_TILE(uneq_x,1,6);
                    UPDATE_HUD_TILE(uneq_x,2,7);
                    uneq_x += 1;
                    UPDATE_HUD_TILE(uneq_x,1,8);
                    UPDATE_HUD_TILE(uneq_x,2,9);
                break;
                case INVITEM_BOX:
                    UPDATE_HUD_TILE(uneq_x,1,32);
                    UPDATE_HUD_TILE(uneq_x,2,33);
                    uneq_x += 1;
                    UPDATE_HUD_TILE(uneq_x,1,34);
                    UPDATE_HUD_TILE(uneq_x,2,35);
                break;
                case INVITEM_HERB:
                    UPDATE_HUD_TILE(uneq_x,1,40);
                    UPDATE_HUD_TILE(uneq_x,2,41);
                    uneq_x += 1;
                    UPDATE_HUD_TILE(uneq_x,1,42);
                    UPDATE_HUD_TILE(uneq_x,2,43);
                break;
                case INVITEM_SILVERSKULL:
                    UPDATE_HUD_TILE(uneq_x,1,48);
                    UPDATE_HUD_TILE(uneq_x,2,49);
                    uneq_x += 1;
                    UPDATE_HUD_TILE(uneq_x,1,50);
                    UPDATE_HUD_TILE(uneq_x,2,51);
                break;
                case INVITEM_MUSHROOM:
                    UPDATE_HUD_TILE(uneq_x,1,52);
                    UPDATE_HUD_TILE(uneq_x,2,53);
                    uneq_x += 1;
                    UPDATE_HUD_TILE(uneq_x,1,54);
                    UPDATE_HUD_TILE(uneq_x,2,55);
                break;
            }
            uneq_x += 2;
        }
        PRINT(0, 0, "INVENTORY           ");
    //HEARTS
        for(INT8 i=0; i<5; i++){
            Sprite* s_heart = SpriteManagerAdd(SpriteInvheart, (((UINT16) 13u + i) << 3), 56u);
	        struct ItemSpawned* invheart_data = (struct ItemSpawned*)s_heart->custom_data;
            if(motherpl_hp > i){
                invheart_data->configured = 2;
            }else{
                invheart_data->configured = 1;
            }
        }
    refresh_equipped();
    blinker_timeout = blinker_timeout_max;
    SHOW_BKG;
	SHOW_SPRITES;
}

void pickup(struct ItemSpawned* pickedup_data) BANKED{
    UINT8 item_added = 0u;
    UINT8 sfx_played = 0u;
    if(pickedup_data->itemtype == INVITEM_HEART){
        my_play_fx(CHANNEL_2, 60, 0xab, 0xf2, 0x37, 0x87, 0x00);//SFX_HEART
        sfx_played = 1u;
        if(motherpl_hp < 5){
            motherpl_hp++;
        }
        item_added = 1u;
    }else if(pickedup_data->itemtype == INVITEM_HEARTS){
        motherpl_hp = 5;
        item_added = 1u;
        my_play_fx(CHANNEL_2, 60, 0xab, 0xf2, 0x37, 0x87, 0x00);//SFX_HEART
        sfx_played = 1u;
    }else if(pickedup_data->itemtype == INVITEM_SILVER){
		enable_hospital.mission_state = MISSION_STATE_ACCOMPLISHED;
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

    }else if(current_state != StateOverworld){
        LogItem(pickedup_data->itemtype);
    }
    //SFX
        if(item_added == 1 && sfx_played == 0u){
            my_play_fx(CHANNEL_2, 60, 0xab, 0xf2, 0x37, 0x87, 0x00);//SFX_HEART
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
    blinking_dessa_counter++;
    if(blinking_dessa_counter < blinking_dessa_counter_max ||
        (blinking_dessa_counter > 40 && 
        blinking_dessa_counter < (40+blinking_dessa_counter_max))){
        //occhi chiusi
        Anim_Inventory_1();
    }else{
        Anim_Inventory_0();
    }
    if(blinking_dessa_counter >= blinking_dessa_timeout){
        //azzera
        blinking_dessa_counter = 0;
    }
    if(blinker_enabled == 1){
        blinker_timeout--;
        if(blinker_timeout <= 0){
            blinker_timeout = blinker_timeout_max;
            blinker_idx++;
            if(blinker_idx > blinker_idx_max){blinker_idx=0;}
            SpriteManagerAdd(SpriteInvblinker, blinker_coords_x[blinker_idx], blinker_coords_y[blinker_idx]);
        }
    }
    if(KEY_PRESSED(J_START)){
        ChangeState(previous_state, 0, -1);
    }
    if(KEY_TICKED(J_JUMP)){
        if(invcursor_posi < 6 && inventory[invcursor_posi].quantity > 0){
            invselectitem(6);
        }
    }else if(KEY_TICKED(J_FIRE)){
        if(inventory[invcursor_posi].itemtype == INVITEM_LIAM_HANDWRITTEN){
            whostalking = ITEMDETAIL_LIAM_HANDWRITTEN;
            manage_bgm(StateDialog, StateInventory, current_map);
            SetState(StateDialog);
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
    if(((INT8)invcursor_old_posi) != invcursor_posi){//muovo cursor verso prossima posizione
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
        PRINT(2, 8, "     ");
        if(inventory[invcursor_posi].quantity == 0){
            PRINT(2, 14, "      ");
        }else if(inventory[invcursor_posi].quantity < 10){
            PRINT(2, 14, "X 00%u", inventory[invcursor_posi].quantity);
        }else if(inventory[invcursor_posi].quantity < 100){
            PRINT(2, 14, "X 0%u", inventory[invcursor_posi].quantity);
        }else{
            PRINT(2, 14, "X %u", inventory[invcursor_posi].quantity);
        }
    }else{
        PRINT(2, 14, "      ");
        if(inventory[invcursor_posi].quantity == 0){
            PRINT(2, 8, "     ");
        }else if(inventory[invcursor_posi].quantity < 10){
            PRINT(2, 8, "X 00%u", inventory[invcursor_posi].quantity);
        }else if(inventory[invcursor_posi].quantity < 100){
            PRINT(2, 8, "X 0%u", inventory[invcursor_posi].quantity);
        }else{
            PRINT(2, 8, "X %u", inventory[invcursor_posi].quantity);
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
            case INVITEM_SILVER:
                GetLocalizedINVLabel_EN(STEEL_NAME, ddinv1);
                GetLocalizedINVLabel_EN(STEEL_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(STEEL_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(STEEL_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(STEEL_DETAIL4, ddinv5);
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
            case INVITEM_PASS:
                GetLocalizedINVLabel_EN(PASS_NAME, ddinv1);
                GetLocalizedINVLabel_EN(PASS_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(PASS_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(PASS_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(PASS_DETAIL4, ddinv5);
            break;
            case INVITEM_GLASSES:
                GetLocalizedINVLabel_EN(GLASS_NAME, ddinv1);
                GetLocalizedINVLabel_EN(GLASS_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(GLASS_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(GLASS_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(GLASS_DETAIL4, ddinv5);
            break;
            case INVITEM_FLOWER:
                GetLocalizedINVLabel_EN(FLOWER_NAME, ddinv1);
                GetLocalizedINVLabel_EN(FLOWER_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(FLOWER_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(FLOWER_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(FLOWER_DETAIL4, ddinv5);
            break;
            case INVITEM_BOX:
                GetLocalizedINVLabel_EN(BOX_NAME, ddinv1);
                GetLocalizedINVLabel_EN(BOX_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(BOX_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(BOX_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(BOX_DETAIL4, ddinv5);
            break;
            case INVITEM_SCORPIONTAIL:
                GetLocalizedINVLabel_EN(SCORPIONTAIL_NAME, ddinv1);
                GetLocalizedINVLabel_EN(SCORPIONTAIL_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(SCORPIONTAIL_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(SCORPIONTAIL_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(SCORPIONTAIL_DETAIL4, ddinv5);
            break;
            case INVITEM_HERB:
                GetLocalizedINVLabel_EN(HERBS_NAME, ddinv1);
                GetLocalizedINVLabel_EN(HERBS_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(HERBS_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(HERBS_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(HERBS_DETAIL4, ddinv5);
            break;
            case INVITEM_LIAM_HANDWRITTEN:
                GetLocalizedINVLabel_EN(MAP_NAME, ddinv1);
                GetLocalizedINVLabel_EN(MAP_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(MAP_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(MAP_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(MAP_DETAIL4, ddinv5);
            break;
            case INVITEM_SILVERSKULL:
                GetLocalizedINVLabel_EN(SILVERSKULL_NAME, ddinv1);
                GetLocalizedINVLabel_EN(SILVERSKULL_DETAIL1, ddinv2);
                GetLocalizedINVLabel_EN(SILVERSKULL_DETAIL2, ddinv3);
                GetLocalizedINVLabel_EN(SILVERSKULL_DETAIL3, ddinv4);
                GetLocalizedINVLabel_EN(SILVERSKULL_DETAIL4, ddinv5);
            break;
        }
    }
    switch(inventory[invcursor_posi].itemtype){
        case INVITEM_ARROW_NORMAL: case INVITEM_ARROW_PERFO:
	    case INVITEM_ARROW_BASTARD: case INVITEM_SILVERSKULL:
        case INVITEM_SILVER: case INVITEM_ARMOR: 
        case INVITEM_GLASSES: case INVITEM_METAL:
            blinker_enabled = 1;
        break;
        default:
            blinker_enabled = 0;
        break;
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
    UINT8 equippable = is_item_equippable(itemtype);
    for(UINT8 i = 0u; i < 12u; ++i){
        if(itemtype == inventory[i].itemtype){
            if(l < 0){
                if(inventory[i].quantity > 0){
                    idx = i;
                }
            }else{
                idx = i;
            }
            i = 65;
        }
    }
    if(idx > -1 && idx < 12){//i<12 sennò non va e non so perchè
        UINT16 new_quantity = inventory[idx].quantity + l;
        if(new_quantity > 999u){
            new_quantity = 999u;
        } 
        inventory[idx].quantity = new_quantity;
    }else{
        idx = -1;
        if(equippable == 0){
            for(UINT8 i = 6u; i < 12; i++){
                if(INVITEM_UNASSIGNED == inventory[i].itemtype){
                    idx = i;
                    i = 65;
                }
            }
        }else{
            for(UINT8 i = 0u; i < 6; i++){
                if(INVITEM_UNASSIGNED == inventory[i].itemtype){
                    idx = i;
                    i = 65;
                }
            }
        }
        if(idx > -1){
            inventory[idx].itemtype = itemtype;
            inventory[idx].equippable = equippable;
            inventory[idx].quantity+=l;
        }
    }
    if(inventory[idx].quantity == 0){
        inventory[idx].itemtype = INVITEM_UNASSIGNED; 
    }
    return inventory[idx].quantity;
}