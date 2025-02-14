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

IMPORT_TILES(font);
IMPORT_TILES(banditstiles);
IMPORT_MAP(banditsmap);
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
extern struct MISSION get_to_the_mountain;
extern struct MISSION find_antidote;
extern CHAPTERS chapter;

const UINT8 coll_tiles_bandits[] = {1u, 10u, 14u, 17u, 18u, 19u, 101u, 102u, 103u, 104u, 105u, 106u, 0};
const UINT8 coll_surface_bandits[] = {57u, 77u, 84u, 88u, 105u, 106u, 0};


extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void motherpl_ckautodialog(Sprite* s_mother, NPCNAME npcname) BANKED;
extern void spawn_policy() BANKED;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        if(chapter == CHAPTER_4_SHIP){
            s_motherpl = SpriteManagerAdd(SpriteMotherplarmor, (UINT16) 5u << 3, (UINT16) 7u << 3);
        }else{
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 5u << 3, (UINT16) 7u << 3);
        }
        if(previous_state == StateInv || previous_state == StateDialog
            || previous_state == StateMountain) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y);
        InitScroll(BANK(banditsmap), &banditsmap, coll_tiles_bandits, coll_surface_bandits);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(banditsmap), &banditsmap, &mapwidth, &mapheight);
	SHOW_SPRITES;
    npc_spawned_zone = 0u;
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
        if(s_motherpl->x > ((UINT16)81u << 3) && 
            get_to_the_mountain.mission_state == MISSION_STATE_DISABLED){
            motherpl_ckautodialog(s_motherpl, OUTWALKER_SIMON);
        } 
    //MANAGE NPC
        spawn_policy();
    
    Log(NONAME);
}

