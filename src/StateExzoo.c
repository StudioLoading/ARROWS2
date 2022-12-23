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
IMPORT_TILES(exzootiles);
IMPORT_MAP(border2);
IMPORT_MAP(exzoomap0);
IMPORT_MAP(hudpl);

extern UINT8 border_set_exzoo;
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

//START LOG PURPOSE
extern UINT8 motherpl_jpower;
extern UINT8 jump_ticked_delay;
extern UINT8 motherpl_vy;
extern MOTHERPL_STATE motherpl_state;
extern UINT8 motherpl_attack_cooldown;
extern Sprite* s_surf;
extern INT8 motherpl_surf_dx;
//END LOG PURPOSE

const UINT8 coll_tiles_exzoo[] = {5u, 7u, 9u, 10u, 0};
const UINT8 coll_surface_exzoo[] = {1u, 0};

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
    s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 4u << 3, (UINT16) 13u << 3);
    scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x, s_motherpl->y);
    InitScroll(BANK(exzoomap0), &exzoomap0, coll_tiles_exzoo, coll_surface_exzoo);

    //reset init_enemy
    init_enemy = 0u;

	INIT_FONT(fontbw, PRINT_BKG);
    //TEST
    PRINT(0, 11, "WORLD");
    PRINT(17, 5, "FPS");
    PRINT(5, 5, "TETRA");
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
        SetState(StateExzoo);
    }
    //GO TO INVENTORY
    if(KEY_PRESSED(J_START)){SetState(StateInventory);}
    scroll_target->x = s_motherpl->x + 16u;
    scroll_target->y = s_motherpl->y + 16u;
    if(init_enemy == 0u && s_motherpl->x > ((UINT16) 20u << 3)){    
        /*
        Sprite* se = SpriteManagerAdd(SpriteEnemysimple, (UINT16) 26u << 3, (UINT16) 6u << 3);
        struct EnemyData* se_info = (struct EnemyData*) se->custom_data;
        se_info->type = SNAKE;
        se_info->configured = 1u;
        */
        Sprite* s2 = SpriteManagerAdd(SpriteEnemysimple, (UINT16) 28u << 3, (UINT16) 6u << 3);
        struct EnemyData* se2_info = (struct EnemyData*) s2->custom_data;
        se2_info->type = RAT;
        se2_info->configured = 1u;
        
        init_enemy = 1u;
    }

    Log();
}
