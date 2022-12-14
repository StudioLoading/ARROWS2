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
Sprite* s_motherpl = 0;
struct MotherplData* d_motherpl = 0;
UINT8 init_enemy = 0u;

void START(){
	if(border_set_exzoo == 0u){
        border_set_exzoo = 1u;
        LOAD_SGB_BORDER(border2);
	}
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
    d_motherpl = (struct MotherplData*) s_motherpl->custom_data;
    scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x, s_motherpl->y);
    InitScroll(BANK(exzoomap0), &exzoomap0, coll_tiles_exzoo, coll_surface_exzoo);

    //reset init_enemy
    init_enemy = 0u;

	INIT_FONT(fontbw, PRINT_BKG);
    PRINT(0, 11, "WORLD");
    PRINT(16, 8, "FPS");
    PRINT(9, 4, "TETRA");
    INIT_HUD(hudpl); 
}

void UPDATE(){
    if(KEY_PRESSED(J_DOWN) && KEY_PRESSED(J_START)){
        SetState(StateExzoo);
    }
    scroll_target->x = s_motherpl->x + 16u;
    scroll_target->y = s_motherpl->y + 16u;
    if(init_enemy == 0u && s_motherpl->x > ((UINT16) 20u << 3)){    
        Sprite* se = SpriteManagerAdd(SpriteEnemy, (UINT16) 26u << 3, (UINT16) 6u << 3);
        struct EnemyData* se_info = (struct EnemyData*) se->custom_data;
        se_info->type = SNAKE;
        se_info->configured = 1u;
        init_enemy = 1u;
    }
    if(print_target != PRINT_WIN){
        print_target = PRINT_WIN;
    }
    switch(motherpl_state){
        case MOTHERPL_IDLE:
            PRINT(0, 2, "IDLE");
        break;
        case MOTHERPL_JUMP:
            PRINT(0, 2, "JUMP");
        break;
        case MOTHERPL_WALK:
            PRINT(0, 2, "WALK");
        break;
    }
    if(s_surf){
        PRINT(5, 2, "SURF");
    }else{
        PRINT(5, 2, "    ");
    }
    PRINT(9, 2, "%i", motherpl_surf_dx);
    //PRINT(8, 2, "%u", motherpl_attack_cooldown);
    /*PRINT(0,0,"%u",motherpl_state);
    if(motherpl_vy < 9){
        PRINT(0, 1, "vy %i ", motherpl_vy);
    }
    if(motherpl_jpower < 9){
        PRINT(0, 2, "POW %u", motherpl_jpower);
    }else{
        PRINT(0, 2, "POW%u", motherpl_jpower);
    }
    if(jump_ticked_delay < 10){
        PRINT(0, 3, "DELAY %u",jump_ticked_delay);
    }else{        
        PRINT(0, 3, "DELAY %u",jump_ticked_delay);
    }
    */
}
