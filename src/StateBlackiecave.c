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
IMPORT_TILES(blackiecavetiles);
IMPORT_TILES(hudpltiles);
IMPORT_MAP(blackiecavemap);
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
extern struct MISSION find_blackie;
extern struct MISSION outwalker_smith;
extern struct MISSION find_antidote;
extern struct MISSION visit_blackie;
extern WHOSTALKING whostalking;
extern CURRENT_BORDER current_border;
extern CHAPTERS chapter;

const UINT8 coll_tiles_blackiecave[] = {1u, 2u, 4u, 5u, 6u, 7u, 14u, 17u, 18u, 19u, 35u, 36u, 37u, 38u, 39u, 40u, 41u, 0};
const UINT8 coll_surface_blackiecave[] = { 16u, 29u, 31u, 33u, 0};

UINT8 wolf_spawned = 0u;
UINT8 timeout_drop = 0u;
extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        if(chapter == CHAPTER_4_SHIP){
            s_motherpl = SpriteManagerAdd(SpriteMotherplarmor, (UINT16) 4u << 3, (UINT16) 20u << 3);
        }else{
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 4u << 3, (UINT16) 20u << 3);
        }
        if(previous_state == StateInv || previous_state == StateDialog
            || previous_state == StateBlackieroom) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
        if(s_motherpl->x > (UINT16)62u << 3){
            s_motherpl->x = (UINT16)60u << 3;
        }
        if(outwalker_smith.mission_state == MISSION_STATE_STARTED){
            UINT8 spawn_flower_2 = outwalker_smith.current_step & 0b00000010;
            if(spawn_flower_2 == 0){
                Sprite* s_flower2 = SpriteManagerAdd(SpriteFlower, (UINT16)24u << 3, (UINT16)20u << 3);
                struct ItemSpawned* flower2_data = (struct ItemSpawned*) s_flower2->custom_data;
                flower2_data->hp = 0b00000010;
            }
        }    
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x, s_motherpl->y); 
        InitScroll(BANK(blackiecavemap), &blackiecavemap, coll_tiles_blackiecave, coll_surface_blackiecave);    
        SHOW_BKG;
        if(visit_blackie.mission_state >= MISSION_STATE_ENABLED && visit_blackie.mission_state < MISSION_STATE_REWARDED){
            SpriteManagerAdd(SpriteBlackiechild, ((UINT16) 15u << 3), ((UINT16) 21u << 3));
            SpriteManagerAdd(SpriteBlackie, ((UINT16) 18u << 3), ((UINT16) 21u << 3));
        }
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(blackiecavemap), &blackiecavemap, &mapwidth, &mapheight);
    if(find_blackie.mission_state < MISSION_STATE_ACCOMPLISHED){
        wolf_spawned = 0u;
    }
    timeout_drop = 0u;
	SHOW_SPRITES;
}

void UPDATE(){
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //GO TO INVENTORY
        if(KEY_PRESSED(J_START)){
            ChangeState(StateInv, s_motherpl,-1);
        }
    //CAMERA MANAGEMENT
        update_camera_position();
    //MANAGE NPC 
        if(s_motherpl->x > ((UINT16)57u << 3) && find_blackie.mission_state < MISSION_STATE_REWARDED){
            if(wolf_spawned == 0u){
                wolf_spawned = 1u;
                SpriteManagerAdd(SpriteWolf, (UINT16)70u << 3, (UINT16) 76u);
            }else if(wolf_spawned > 0u){
                if(s_motherpl->x > ((UINT16)62u << 3)){
                    trigger_dialog(WOLF01, s_motherpl);
                }
            }
        }
     //DROPS ANIM
        if(s_motherpl->x<((UINT16)50u)<<3){
            timeout_drop--;
            if(timeout_drop == 80u){
                SpriteManagerAdd(SpriteDrop, s_motherpl->x+24u, (UINT16) 44u);
            }
            if(timeout_drop == 160u){
                SpriteManagerAdd(SpriteDrop, s_motherpl->x-12u, (UINT16) 44u);
            }
            if(timeout_drop == 0u){
                SpriteManagerAdd(SpriteDrop, s_motherpl->x+64u, (UINT16) 44u);
                timeout_drop = 240u;
            }
        }

    
    Log(NONAME);
}