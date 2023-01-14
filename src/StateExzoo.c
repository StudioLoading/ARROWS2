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
extern UINT16 motherpl_pos_x;
extern UINT16 motherpl_pos_y;

const UINT8 coll_tiles_exzoo[] = {5u, 7u, 9u, 10u, 14u, 17u, 18u, 19u, 0};
const UINT8 coll_surface_exzoo[] = {1u, 0};


void UpdateHUD() BANKED;
void Log() BANKED;
void update_camera_position() BANKED;

extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;

UINT8 test_countdown = 255u;

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
    ///*
        if(motherpl_pos_x != 0u && motherpl_pos_y != 0u){
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, motherpl_pos_x, motherpl_pos_y);
        }else{
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 4u << 3, (UINT16) 6u << 3);
        }
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x, s_motherpl->y); 
    //*/
    /*
    scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 10u << 3, ((UINT16) 13u) << 3);
    */
    InitScroll(BANK(exzoomap0), &exzoomap0, coll_tiles_exzoo, coll_surface_exzoo);

	INIT_FONT(fontbw, PRINT_BKG);
    //reset init_enemy
    init_enemy = 0u;
    test_countdown = 255u;

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
    if(test_countdown > 0u){
        test_countdown--;
    }
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp || hud_motherpl_ups != motherpl_ups){
            UpdateHUD();
        }
    //GO TO INVENTORY
        if(KEY_PRESSED(J_START)){ChangeState(StateInventory, s_motherpl);}
    //SCROLL CAMERA
        update_camera_position();    
    //INIT ENEMIES
    //init_enemy == 0u &&
    if(test_countdown == 0u){
        test_countdown = 255u;
        init_enemy = 1u;
        //s_motherpl->x > ((UINT16) 20u << 3)){
        //spawn_item = 1u;
        //Sprite* s_item = SpriteManagerAdd(SpriteItemspawned, (UINT16) 12u << 3, (UINT16) 10u << 3);
        /*
        Sprite* s1 = SpriteManagerAdd(SpriteEnemysimplesnake, (UINT16) 12u << 3, (UINT16) 6u << 3);
        struct EnemyData* se1_info = (struct EnemyData*) s1->custom_data;
        se1_info->type = SNAKE;
        se1_info->configured = 1u;
        */
        ///*
        SpriteManagerAdd(SpriteEnemyAttackerCobra, (UINT16) 12u << 3, (UINT16) 6u << 3);
        //SpriteManagerAdd(SpriteEnemysimplerat, (UINT16) 12u << 3, (UINT16) 6u << 3);
        //*/
        /*
        Sprite* s3 = SpriteManagerAdd(SpriteEnemyattacker, (UINT16) 10u << 3, (UINT16) 6u << 3);
        struct EnemyData* se3_info = (struct EnemyData*) s3->custom_data;
        se3_info->type = PORCUPINE;
        se3_info->configured = 1u;
        */
        /*
        Sprite* s4 = SpriteManagerAdd(SpriteEnemyattacker, (UINT16) 13u << 3, (UINT16) 7u << 3);
        struct EnemyData* se4_info = (struct EnemyData*) s4->custom_data;
        se4_info->type = COBRA;
        se4_info->configured = 1u;
        */
        /*
        Sprite* s5 = SpriteManagerAdd(SpriteEnemythrower, (UINT16) 10u << 3, (UINT16) 8u << 3);
        struct EnemyData* se5_info = (struct EnemyData*) s5->custom_data;
        se5_info->type = SPIDER;
        se5_info->configured = 1u;
        */
        /*
        Sprite* s6 = SpriteManagerAdd(SpriteEnemythrower, (UINT16) 10u << 3, (UINT16) 7u << 3);
        struct EnemyData* se6_info = (struct EnemyData*) s6->custom_data;
        se6_info->type = TARANTULA;
        se6_info->configured = 1u;
        */        
    }

    Log();
}

