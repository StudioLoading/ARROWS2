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
#include "Dialogs.h"

IMPORT_TILES(fontbw);
IMPORT_TILES(cavetiles);
IMPORT_MAP(border2);
IMPORT_MAP(cavemap);
IMPORT_MAP(hudpl);

const UINT8 coll_tiles_cave[] = {8u, 0};
const UINT8 coll_surface_cave[] = {1u, 0};

extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern INT8 motherpl_hp;
extern INT8 motherpl_ups;
extern Sprite* s_motherpl;
extern UINT8 init_enemy;
extern INT8 hud_motherpl_hp;
extern INT8 hud_motherpl_ups;

void UpdateHUD() BANKED;
void Log() BANKED;

void START(){
    LOAD_SGB_BORDER(border2);
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume
	//SCROLL LIMITS
	scroll_top_movement_limit = 56u;
	scroll_bottom_movement_limit = 80u;
    //SGB PALETTE
    if(sgb_check()){
        set_sgb_palette_2();
    }
    //INIT GRAPHICS    
    s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 4u << 3, (UINT16) 6u << 3);
    scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x, s_motherpl->y);
    InitScroll(BANK(cavemap), &cavemap, coll_tiles_cave, coll_surface_cave);

    //reset init_enemy
    init_enemy = 0u;

	INIT_FONT(fontbw, PRINT_BKG);
    PRINT(5, 5, "CAVE");
    //HUD
    INIT_HUD(hudpl); 
    hud_motherpl_hp = 0;
    UpdateHUD();
}

void UPDATE(){
    //UPDATE HUD
    if(hud_motherpl_hp != motherpl_hp){
        UpdateHUD();
    }
    //RESTART CURRENT STATE
    if(KEY_PRESSED(J_DOWN) && KEY_PRESSED(J_START)){
        SetState(StateCave);
    }
    scroll_target->x = s_motherpl->x + 16u;
    scroll_target->y = s_motherpl->y + 16u;
    
    Log();
}