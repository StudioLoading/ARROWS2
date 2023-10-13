#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "Dialogs.h"

IMPORT_MAP(bordersky);
IMPORT_TILES(font);
IMPORT_TILES(mountaintiles);
IMPORT_MAP(mountainmap);
IMPORT_MAP(hudpl);

extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern INT8 motherpl_hp;
extern INT8 hud_motherpl_hp;
extern Sprite* s_motherpl;
extern UINT8 init_enemy;
extern UINT8 MAX_ENEMY;
extern UINT8 mapwidth;
extern UINT8 mapheight;
extern UINT8 previous_state;
extern UINT16 motherpl_pos_x;
extern UINT16 motherpl_pos_y;
extern MirroMode motherpl_mirror; 
extern INT8 motherpl_vx;
extern UINT8 npc_spawned_zone;
extern UINT8 generic_counter;
extern struct MISSION help_cemetery_woman;
extern WHOSTALKING whostalking;
extern UINT16 timeout_enemy;
extern UINT8 enemy_counter;

const UINT8 coll_tiles_mountain[] = { 14u, 17u, 18u, 19u, 20u, 24u, 25u, 26u, 38u, 
41u, 64u, 0};
const UINT8 coll_surface_mountain[] = {10u, 29u, 37u, 40u, 57u, 60u, 63u, 0};

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;

extern UINT16 test_counter;


void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 4u << 3, (UINT16) 49u << 3);
        if(previous_state == StateInventory || previous_state == StateDialog
            || previous_state == StateSky) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y); 
        InitScroll(BANK(mountainmap), &mountainmap, coll_tiles_mountain, coll_surface_mountain);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        enemy_counter = 0u;
        timeout_enemy = 0;
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(mountainmap), &mountainmap, &mapwidth, &mapheight);
	SHOW_SPRITES;
    test_counter = 0u;
}

void UPDATE(){
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //GO TO INVENTORY
        if(KEY_PRESSED(J_START)){ChangeState(StateInventory, s_motherpl, -1);}
    //CAMERA MANAGEMENT
        update_camera_position();
    //BIGSTONES & SCORPIONS
        if(enemy_counter < 2 && s_motherpl->y > 40){
            timeout_enemy++;
            if(timeout_enemy == 160u){
                test_counter++;                
                timeout_enemy = 0;
                switch(test_counter){
                    case 3:
                        {
                        test_counter = 0;
                        Sprite* s_bigstone_1 = SpriteManagerAdd(SpriteBigstoneanticipation, (UINT16)(s_motherpl->x), (UINT16)(s_motherpl->y - 80u));
                        struct EnemyData* bigstone1_data = (struct EnemyData*) s_bigstone_1->custom_data;
                        bigstone1_data->configured = 0;                        
                        }
                    break;
                    case 1:
                        {
                        Sprite* s_bigstone_2 = SpriteManagerAdd(SpriteBigstoneanticipation, (UINT16)(s_motherpl->x + 50u), (UINT16)(s_motherpl->y - 80u));
                        s_bigstone_2->mirror = V_MIRROR;
                        struct EnemyData* bigstone2_data = (struct EnemyData*) s_bigstone_2->custom_data;
                        bigstone2_data->configured = 1;
                        }
                    break;
                }
            }
            if(timeout_enemy == 70u || timeout_enemy == 140u){
                SpriteManagerAdd(SpriteScorpion, (UINT16) 9u << 3, (UINT16) 39u << 3);
            }
        }
    
    Log(NONAME);
}

