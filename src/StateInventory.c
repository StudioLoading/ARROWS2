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
#include "DiaryDialogs.h"

IMPORT_TILES(fontbw);
DECLARE_MUSIC(bgm_credits);
IMPORT_MAP(border);
IMPORT_MAP(inventorymap);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

extern UINT8 border_set_diary;
extern UINT8 border_set_ow;
extern UINT8 current_map;//0=south-west, 1=south-east, 2=north-west, 3=north-east
extern unsigned char dd1[];
extern unsigned char dd2[];
extern unsigned char dd3[];
extern unsigned char dd4[];
extern unsigned char dd5[];
extern unsigned char dd6[];
extern unsigned char dd7[];
extern unsigned char dd8[];

const UINT8 collision_tiles_inv[] = {1, 2, 0};

UINT8 invcursor_posx[] = {8u, 32u, 56u, 8u, 32u, 56u};
UINT8 invcursor_posy[] = {16u, 16u, 16u, 40u, 40u, 40u};
INT8 invcursor_posi = 0u;
UINT8 invcursor_old_posi = 0u;
const INT8 invcursor_posimax = 6;
Sprite* inv_cursor = 0;
struct InvItem itemCrossbow = {.itemtype = INVITEM_CROSSBOW, .quantity = 0, .owned = 1u};
struct InvItem itemDontknow = {.itemtype = INVITEM_CROSSBOW, .quantity = 0, .owned = 0u};
struct InvItem itemMoney = {.itemtype = INVITEM_MONEY, .quantity = 10, .owned = 1u};
struct InvItem itemArrowNormal = {.itemtype = INVITEM_ARROW_NORMAL, .quantity = 100, .owned = 1u};
struct InvItem itemArrowPerf = {.itemtype = INVITEM_ARROW_PERFO, .quantity = 0, .owned = 0u};
struct InvItem itemBomb = {.itemtype = INVITEM_BOMB, .quantity = 0, .owned = 0u};
const struct InvItem* inventory[6] = {&itemCrossbow, &itemMoney, &itemDontknow, &itemArrowNormal, &itemArrowPerf, &itemBomb};


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
    UINT8 isEmpty = inventory[invcursor_posi]->owned == 0 ? 1 : 0;
    Sprite* s_invitem = 0;
    for(UINT8 i = 0u; i < 6; i++){
        s_invitem = SpriteManagerAdd(SpriteInvitem, invcursor_posx[i],invcursor_posy[i]);
        struct InvItem* cdata = (struct InvItem*) s_invitem->custom_data;
        cdata->itemtype = inventory[i]->itemtype;
        cdata->quantity = inventory[i]->quantity;
        cdata->owned = inventory[i]->owned;
        cdata->configured = 1u;
    }
    Inv_change_detail(inventory[invcursor_posi]->itemtype, isEmpty);

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
        UINT8 isEmpty = inventory[invcursor_posi]->owned == 0 ? 1 : 0;
        Inv_change_detail(inventory[invcursor_posi]->itemtype, isEmpty);
    }
}