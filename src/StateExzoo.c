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
#include "TAnim0.h"
#include "Dialogs.h"

IMPORT_TILES(font);//font
IMPORT_TILES(exzootiles);
IMPORT_MAP(exzoomap0);
IMPORT_MAP(hudpl);

extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
extern INT8 motherpl_hp;
extern INT8 hud_motherpl_hp;
extern Sprite* s_motherpl;
extern UINT8 init_enemy;
extern UINT8 mapwidth;
extern UINT8 mapheight;
extern UINT8 previous_state;
extern UINT16 motherpl_pos_x;
extern UINT16 motherpl_pos_y;
extern MirroMode motherpl_mirror; 
extern UINT8 npc_spawned_zone;
extern struct MISSION outwalker_chief;
extern struct MISSION outwalker_smith;
extern CHAPTERS chapter;

const UINT8 coll_tiles_exzoo[] = {5u, 7u, 9u, 10u, 14u, 17u, 18u, 19u, 28u, 48u,
88u, 89u, 90u, 92u, 94u, 0};
const UINT8 coll_surface_exzoo[] = {1u, 27u, 0};


extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void spawn_policy() BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        if(chapter == CHAPTER_4_SHIP){
            s_motherpl = SpriteManagerAdd(SpriteMotherplarmor, (UINT16) 10u << 3, (UINT16) 9u << 3);
        }else{
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 10u << 3, (UINT16) 9u << 3);
        }
        if(previous_state == StateInv || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
        if(outwalker_smith.mission_state == MISSION_STATE_STARTED){
            UINT8 spawn_flower_3 = outwalker_smith.current_step & 0b00000100;
            if(spawn_flower_3 == 0){
                Sprite* s_flower3 = SpriteManagerAdd(SpriteFlower, (UINT16)32u << 3, (UINT16)9u << 3);
                struct ItemSpawned* flower3_data = (struct ItemSpawned*) s_flower3->custom_data;
                flower3_data->hp = 0b00000100;
            }
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y); 
        InitScroll(BANK(exzoomap0), &exzoomap0, coll_tiles_exzoo, coll_surface_exzoo);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(exzoomap0), &exzoomap0, &mapwidth, &mapheight);
	//CHECK DIALOG TO ACTIVATE
        if(outwalker_chief.mission_state == MISSION_STATE_ACCOMPLISHED && 
            outwalker_chief.current_step < 3){
                motherpl_pos_x = (UINT16) 82u << 3;
                motherpl_pos_y = (UINT16) 10u << 3;
                outwalker_chief.current_step = 3;
				SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
                trigger_dialog(OUTWALKER_CHIEF_FOUND, THIS);
            }

    SHOW_SPRITES;
    npc_spawned_zone = 0;
}

void UPDATE(){
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //GO TO INVENTORY
        if(KEY_PRESSED(J_START)){ChangeState(StateInv, s_motherpl,-1);}
    //CAMERA MANAGEMENT
        update_camera_position();
    //MANAGE NPC
        spawn_policy();
    
    Log(NONAME);
}

