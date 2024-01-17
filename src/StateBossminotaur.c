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

IMPORT_MAP(bordercave);
IMPORT_TILES(font);
IMPORT_TILES(hudpltiles);
IMPORT_MAP(bossminotaurmap);
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
extern struct EtoReload e_to_reload[3];
extern MOTHERPL_STATE motherpl_state;
extern UINT8 timeout_drop;

const UINT8 coll_tiles_mino[] = {2u, 6u, 10u, 21u,
27u, 29u, 35u, 0};
const UINT8 coll_surface_mino[] = {14u, 17u, 18u, 19u, 57u, 68u, 81u, 85u, 89u, 0};

extern UINT8 mother_exit_cooldown;

Sprite* s_minotaur;
extern struct EnemyData* minotaur_data;
extern struct MISSION golden_armor;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 6u << 3, (UINT16) 8u << 3);
        if(previous_state == StateInventory
            || (previous_state == StateDialog && choice == 0u)) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
    //INIT CHAR & MAP
        s_minotaur = SpriteManagerAdd(SpriteBossminotaur, (UINT16)18u << 3, (UINT16) 88u);
        InitScroll(BANK(bossminotaurmap), &bossminotaurmap, coll_tiles_mino, coll_surface_mino); 
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 80u, (UINT16) 112u);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(bossminotaurmap), &bossminotaurmap, &mapwidth, &mapheight);
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
                previous_state = StateBossscorpion;
                ChangeState(StateOverworld, s_motherpl, 3);
                //go back
            }
        }else if(mother_exit_cooldown != 60u){
            mother_exit_cooldown = 60u;
        }
        if(s_motherpl->x > ((UINT16)19u << 3)){
            s_motherpl->x = ((UINT16)19u << 3);
        }
    //DROPS
        /*
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
        */
    
    Log(NONAME);
}