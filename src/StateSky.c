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
IMPORT_TILES(skytiles);
IMPORT_MAP(skymap);
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
extern Sprite* s_surf;

const UINT8 coll_tiles_sky[] = { 7u, 14u, 17u, 18u, 19u, 20u, 24u, 28u, 32u, 36u, 40u, 44u, 0};
const UINT8 coll_surface_sky[] = {56u, 0};

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
        s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 4u << 3, (UINT16) 11u << 3);
        if(previous_state == StateInventory || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y); 
        InitScroll(BANK(skymap), &skymap, coll_tiles_sky, coll_surface_sky);    
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
        GetMapSize(BANK(skymap), &skymap, &mapwidth, &mapheight);
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
        if(s_motherpl->y > ((UINT16) 19u << 3)){
            //back to StateMountain
            ChangeState(StateMountain, s_motherpl, -1);
        }
    //BOLTS
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
    
    Log(NONAME);
}

