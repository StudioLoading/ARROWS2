#include "Banks/SetAutoBank.h"

#include "SGB.h"
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
#include "sgb_palette.h"
#include "Dialogs.h"

IMPORT_MAP(bordercart);
IMPORT_TILES(fontbw);
IMPORT_TILES(minetiles);
IMPORT_MAP(cartmap);
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
extern UINT8 current_map;
extern Sprite* s_surf;

const UINT8 coll_tiles_cart[] = { 1u, 7u, 3u, 12u, 14u, 17u, 18u, 19u, 20u, 21u, 22u, 27u, 28u, 32u, 36u, 40u, 44u, 109u, 111u, 0};
const UINT8 coll_surface_cart[] = {56u, 64u, 66u, 67u, 68u, 79u, 80u, 81u, 82u, 83u, 88u, 89u, 90u, 91u, 0};

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;

extern UINT16 test_counter;

void START(){
    LOAD_SGB_BORDER(bordercart);
    //SOUND
        NR52_REG = 0x80; //Enables sound, you should always setup this first
        NR51_REG = 0xFF; //Enables all channels (left and right)
        NR50_REG = 0x77; //Max volume
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;//56u;
        scroll_bottom_movement_limit = 80u;//80u;
    //INIT GRAPHICS
        s_motherpl = SpriteManagerAdd(SpriteCart, (UINT16) 2u << 3, (UINT16) 4u << 3);
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y - 16u); 
        InitScroll(BANK(cartmap), &cartmap, coll_tiles_cart, coll_surface_cart);    
    //HUD
        INIT_FONT(fontbw, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        enemy_counter = 0u;
        timeout_enemy = 0;
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(cartmap), &cartmap, &mapwidth, &mapheight);
	SHOW_SPRITES;
    test_counter = 0u;
}

void UPDATE(){
    //TEST PURPOSE REMOVEME
        if(KEY_PRESSED(J_START) && KEY_PRESSED(J_DOWN)){
            SetState(StateCart);
        }
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //GO TO INVENTORY
        //if(KEY_PRESSED(J_START)){ChangeState(StateInventory, s_motherpl, -1);}
    //CAMERA MANAGEMENT
        update_camera_position();
        //scroll_target->x = s_motherpl->x + 76u;
        //scroll_target->y = s_motherpl->y - 16u;
    //BOLTS
        /*
        if(enemy_counter < 2 && s_motherpl->y > 40){
            timeout_enemy++;
            UINT8 timeout_enemy_max = 90u;
            if(s_motherpl->x > ((UINT16) 90u << 3)){
                timeout_enemy_max = 52u;
            }
            if(timeout_enemy == timeout_enemy_max){
                test_counter++;                
                timeout_enemy = 0;
                INT8 delta_for_running = 0;
                if(motherpl_vx != 0){
                    if(motherpl_vx > 0){delta_for_running = 10;}
                    else{delta_for_running = -10;}
                }
                if(s_surf != NULL){
                    delta_for_running = +60;
                }
                UINT16 bolt_x = delta_for_running;
                UINT16 bolt_y = s_motherpl->y;
                switch(test_counter){
                    case 6:
                        test_counter = 0;
                        bolt_x = s_motherpl->x + 80u;
                    break;
                    case 5:bolt_x = s_motherpl->x-4u;break;
                    case 4:bolt_x = s_motherpl->x + 40u;break;
                    case 3:bolt_x = s_motherpl->x - 30u;break;
                    case 2:bolt_x = s_motherpl->x + 30u;break;
                    case 1:bolt_x = s_motherpl->x +10u;break;
                }
                SpriteManagerAdd(SpriteBoltground, (UINT16) bolt_x, (UINT16)bolt_y);
            }
        }
        */
    
    Log(NONAME);
}

