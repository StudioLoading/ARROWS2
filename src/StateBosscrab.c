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
#include "TilesAnimations0.h"
#include "Dialogs.h"

IMPORT_MAP(bordercrab);
IMPORT_TILES(font);
IMPORT_TILES(bosscrabtiles);
IMPORT_TILES(hudpltiles);
IMPORT_MAP(bosscrabmap);
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
extern UINT8 npc_spawned_zone;
extern struct MISSION outwalker_glass;
extern WHOSTALKING whostalking;
extern UINT8 choice;
extern UINT16 timeout_enemy;
extern MOTHERPL_STATE motherpl_state;
extern CHAPTERS chapter;

const UINT8 coll_tiles_crab[] = {63u, 83u, 84u, 86u, 87u, 89u, 90u, 92u, 93u, 95u, 0};
const UINT8 coll_surface_crab[] = { 62u, 0};

extern UINT8 mother_exit_cooldown;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        if(chapter == CHAPTER_4_SHIP){
            s_motherpl = SpriteManagerAdd(SpriteMotherplarmor, (UINT16) 6u << 3, (UINT16) 8u << 3);
        }else{
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 6u << 3, (UINT16) 8u << 3);
        }
        if(previous_state == StateInventory
            || (previous_state == StateDialog && choice == 0u)) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
    //INIT CHAR & MAP
        Sprite* s_crab = SpriteManagerAdd(SpriteBosscrab, (UINT16)10u << 3, (UINT16) 40u);
        struct NpcInfo*  s_crab_data= (struct NpcInfo*) s_crab->custom_data;
        s_crab_data->whotalks = CRAB;
        InitScroll(BANK(bosscrabmap), &bosscrabmap, coll_tiles_crab, coll_surface_crab); 
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 88u, (UINT16) 120u);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(bosscrabmap), &bosscrabmap, &mapwidth, &mapheight);
    timeout_enemy = 200u;
    mother_exit_cooldown = 60u;
	SHOW_SPRITES;
}

void UPDATE(){
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //CAMERA MANAGEMENT
        scroll_target->x = (UINT16) 80u;
        scroll_target->y = (UINT16) 56u;
    //FORCE MOTHERPL LIMITS
        if(s_motherpl->x < (UINT16)8u){
            s_motherpl->x = 8u;
            mother_exit_cooldown--;
            if(outwalker_glass.mission_state >= MISSION_STATE_ACCOMPLISHED){
               mother_exit_cooldown = 0; 
            }
            if(mother_exit_cooldown == 0u ){
                mother_exit_cooldown = 60u;
                previous_state = StateBosscrab;
                ChangeState(StateOverworld, s_motherpl, 1);
                //go back
            }
        }else if(mother_exit_cooldown != 60u){
            mother_exit_cooldown = 60u;
        }
        if(s_motherpl->x > ((UINT16)19u << 3)){
            s_motherpl->x = ((UINT16)19u << 3);
        }    
    Log(NONAME);
}