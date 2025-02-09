#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"
#include "TAnim0.h"
#include "Dialogs.h"

IMPORT_TILES(font);
IMPORT_TILES(hudpltiles);
IMPORT_MAP(bossbatmap);
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
extern struct MISSION outwalker_glass;
extern WHOSTALKING whostalking;
extern UINT8 choice;
extern UINT16 timeout_enemy;
extern MOTHERPL_STATE motherpl_state;
extern UINT8 timeout_drop;
extern UINT8 mother_exit_cooldown;
extern struct MISSION mr_smee;
extern UINT8 tiles_anim_interval;
extern UINT8 timeout_drop;
extern CHAPTERS chapter;

const UINT8 coll_tiles_bat[] = {1u, 2u, 4u, 5u, 6u, 7u, 14u,
17u, 18u, 19u, 34u, 35u, 36u, 37u, 38u, 39u, 40u, 41u, 54u, 55u, 0};
const UINT8 coll_surface_bat[] = { 16u, 29u, 31u, 33u, 0};

INT8 bossbat_exit_cooldown = 0;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        s_motherpl = SpriteManagerAdd(SpriteMotherplarmor, (UINT16)16u << 3, (UINT16) 10u << 3);
    //INIT CHAR & MAP
        SpriteManagerAdd(SpriteBossbat, ((UINT16)15u << 3), 14u);
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, ((UINT16) 12u << 3), ((UINT16) 10u << 3));
        InitScroll(BANK(bossbatmap), &bossbatmap, coll_tiles_bat, coll_surface_bat); 
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(bossbatmap), &bossbatmap, &mapwidth, &mapheight);
    timeout_enemy = 200u;
    mother_exit_cooldown = 60u;
    tiles_anim_interval = 0u;
    timeout_drop = 1u;
    bossbat_exit_cooldown = 0;
	SHOW_SPRITES;
}

void UPDATE(){
    //EXIT COOLDOWN
        if(bossbat_exit_cooldown > 0){
            bossbat_exit_cooldown--;
            if(bossbat_exit_cooldown == 0){
                mr_smee.mission_state = MISSION_STATE_ACCOMPLISHED;
                ChangeState(StateOw, s_motherpl, MAP_SOUTHEAST);
            }
        }
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //FORCE MOTHERPL LIMITS
        if(s_motherpl->x < (UINT16)24u){
            s_motherpl->x = 24u;
        }
    
    Log(NONAME);
}