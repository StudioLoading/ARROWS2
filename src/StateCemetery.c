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

#define MAX_CLOUD_TIMEOUT 200

IMPORT_TILES(font);
IMPORT_TILES(cemeterytiles);
IMPORT_TILES(hudpltiles);
IMPORT_MAP(cemeterymap);
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
extern struct MISSION engage_smith;
extern struct MISSION help_cemetery_woman;
extern struct MISSION outwalker_smith;
extern CHAPTERS chapter;

const UINT8 coll_tiles_cemetery[] = {0u, 0};
const UINT8 coll_surface_cemetery[] = {1u, 16u, 0};

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos, NPCNAME npcname) BANKED;
extern void spawn_npa(UINT8 type, UINT16 posx, UINT16 posy, UINT8 configured) BANKED;
extern void spawn_policy() BANKED;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        if(chapter == CHAPTER_4_SHIP){
            s_motherpl = SpriteManagerAdd(SpriteMotherplarmor, (UINT16) 4u << 3, (UINT16) 10u << 3);
        }else{
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 4u << 3, (UINT16) 10u << 3);
        }
        if(previous_state == StateInv || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
        if(outwalker_smith.mission_state == MISSION_STATE_STARTED){
            UINT8 spawn_flower_4 = outwalker_smith.current_step & 0b00001000;
            if(spawn_flower_4 == 0){
                Sprite* s_flower4 = SpriteManagerAdd(SpriteFlower, (UINT16)31u << 3, (UINT16)12u << 3);
                struct ItemSpawned* flower4_data = (struct ItemSpawned*) s_flower4->custom_data;
                flower4_data->hp = 0b00001000;
            }
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x, s_motherpl->y); 
        InitScroll(BANK(cemeterymap), &cemeterymap, coll_tiles_cemetery, coll_surface_cemetery);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(cemeterymap), &cemeterymap, &mapwidth, &mapheight);
    SHOW_SPRITES;
}

void UPDATE(){
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //GO TO INVENTORY
        if(KEY_PRESSED(J_START)){ChangeState(StateInv, s_motherpl, -1);}
    //CAMERA MANAGEMENT
        update_camera_position();
    //MANAGE NPC
        spawn_policy();
    Log(NONAME);
}

