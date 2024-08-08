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

IMPORT_TILES(font);
IMPORT_TILES(countrytiles);
IMPORT_TILES(hudpltiles);
IMPORT_MAP(countrymap);
IMPORT_MAP(beachmap);
IMPORT_MAP(hudpl);

extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
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
extern struct MISSION defeat_scorpions;
extern struct MISSION find_antidote;
extern WHOSTALKING whostalking;
extern UINT8 choice;
extern UINT16 timeout_enemy;
extern struct EtoReload e_to_reload[3];
extern MOTHERPL_STATE motherpl_state;
extern UINT8 mother_exit_cooldown;
extern UINT8 ow_is_beach;
extern CHAPTERS chapter;

const UINT8 coll_tiles_country[] = {1,2,3,4,5,39,41,43,44,0};
const UINT8 coll_surface_country[] = {0};
const UINT8 coll_tiles_beach[] = {63u, 83u, 84u, 86u, 87u, 89u, 90u, 92u, 93u, 95u, 0};
const UINT8 coll_surface_beach[] = { 62u, 0};

UINT8 scorpion_mission_goal = 0b00000000;
struct EnemyData* s_scorpion_data = 0;

void enemy_death() BANKED;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;

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
        Sprite* s_scorpion = SpriteManagerAdd(SpriteEnemyThrowerScorpion, (UINT16)15u << 3, (UINT16) 64u);
        s_scorpion_data= (struct EnemyData*) s_scorpion->custom_data;
        s_scorpion_data->configured = 1;
        if(ow_is_beach == 0){
            InitScroll(BANK(countrymap), &countrymap, coll_tiles_country, coll_surface_country); 
        }else{
            InitScroll(BANK(beachmap), &beachmap, coll_tiles_beach, coll_surface_beach); 
        }
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 80u, (UINT16) 96u);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(countrymap), &countrymap, &mapwidth, &mapheight);
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
        //scroll_target->x = (UINT16) 80u;
        //scroll_target->y = (UINT16) 56u;
    //FORCE MOTHERPL LIMITS
        if(s_motherpl->x < (UINT16)8u || s_motherpl->x > ((UINT16)19u << 3)){
            if(s_motherpl->x < (UINT16)8u){
                s_motherpl->x = 8u;
            }            
            if(s_motherpl->x > ((UINT16)19u << 3)){
                s_motherpl->x = ((UINT16)19u << 3);
            }
            mother_exit_cooldown--;
            if(mother_exit_cooldown == 0u ){
                mother_exit_cooldown = 60u;
                previous_state = StateScorpions;
                ChangeState(StateOverworld, s_motherpl, -1);
                //go back
            }
        }else if(mother_exit_cooldown != 60u){
            mother_exit_cooldown = 60u;
        }
    Log(NONAME);
}

void enemy_death() BANKED{
    defeat_scorpions.current_step = defeat_scorpions.current_step | scorpion_mission_goal;
    if(defeat_scorpions.current_step == 0b11111111 && defeat_scorpions.phase == 0u){
        defeat_scorpions.current_step = 0;
        defeat_scorpions.phase = 1u;
        SpriteManagerAdd(SpriteDiary, s_motherpl->x, s_motherpl->y - 24u);
    }else if(defeat_scorpions.current_step == 0b00011111 && 
        defeat_scorpions.phase == 1){
        defeat_scorpions.phase = 2u;
        defeat_scorpions.mission_state = MISSION_STATE_REWARDED;
        find_antidote.mission_state = MISSION_STATE_ENABLED;
        find_antidote.phase = 0;
        find_antidote.current_step = 0;
        SpriteManagerAdd(SpriteDiary, (UINT16)s_motherpl->x, (UINT16)s_motherpl->y);
    }
}