#include "Banks/SetAutoBank.h"

#include "BankManager.h"
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
IMPORT_TILES(blackiecavetiles);
IMPORT_MAP(scorpioncave);
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
extern UINT8 generic_counter;
extern struct MISSION help_cemetery_woman;
extern WHOSTALKING whostalking;
extern UINT16 timeout_enemy;
extern UINT8 enemy_counter;
extern INT8 current_map;
extern UINT8 enemy_wave;
extern UINT8 child_hooked;
extern MOTHERPL_STATE motherpl_state;
extern UINT8 powder_cooldown;
extern UINT8 item_spawned_cooldown;
extern UINT8 itemspawned_powder_max;

const UINT8 coll_tiles_scorpioncave[] = {1u, 2u, 4u, 5u, 6u, 7u, 14u,
17u, 18u, 19u, 34u, 35u, 36u, 37u, 38u, 39u, 40u, 41u, 54u, 55u, 0};
const UINT8 coll_surface_scorpioncave[] = { 16u, 29u, 31u, 33u, 0};

void spawn_enemy_scorpioncave() BANKED;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;


void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        s_motherpl = SpriteManagerAdd(SpriteMotherpl, 10u, (UINT16) 9u << 3);
        if(previous_state == StateInventory || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y); 
        InitScroll(BANK(scorpioncave), &scorpioncave, coll_tiles_scorpioncave, coll_surface_scorpioncave);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        enemy_counter = 0u;
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(scorpioncave), &scorpioncave, &mapwidth, &mapheight);
	SHOW_SPRITES;
    timeout_enemy = 400u;
    generic_counter = 60u;
    itemspawned_powder_max = 4;
    if(previous_state == StateOverworld){
        enemy_wave = 12;
    }
}

void UPDATE(){
    if(timeout_enemy > 600){
        timeout_enemy = 600;
    }    
    //COOLDOWNS
        if(powder_cooldown > 0){powder_cooldown--;}
    //SPAWNING ITEM COOLDOWN
        if(item_spawned_cooldown > 0u){
            item_spawned_cooldown--;
        }
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //GO TO INVENTORY
        if(KEY_PRESSED(J_START)){ChangeState(StateInventory, s_motherpl, -1);}
    //CAMERA MANAGEMENT
        update_camera_position();
    //MANAGE NPC
        //LEAF
            /*
            generic_counter--;
            switch(generic_counter){
                case 0u:generic_counter = 255u;break;
                case 80u:SpriteManagerAdd(SpriteLeaf, s_motherpl->x, ((UINT16)6 << 3));break;
                case 160u: {
                    UINT16 leafx = s_motherpl->x + 100u;
                    if(s_motherpl->mirror == V_MIRROR){leafx = s_motherpl->x - 80u;}
                    Sprite* leaf2 = SpriteManagerAdd(SpriteLeaf, leafx, ((UINT16)7 << 3));
                    struct PlatformInfo* leaf2_info = (struct PlatformInfo*)leaf2->custom_data;
                    leaf2_info->step = 100u;}break;
                case 200u:{
                    UINT16 leafx = s_motherpl->x + 80u;
                    if(s_motherpl->mirror == V_MIRROR){leafx = s_motherpl->x - 50u;}
                    Sprite* leaf2 = SpriteManagerAdd(SpriteLeaf, leafx, ((UINT16)7 << 3));
                    struct PlatformInfo* leaf2_info = (struct PlatformInfo*)leaf2->custom_data;
                    leaf2_info->step = 130u;}break;
                case 240u:{
                    UINT16 leafx = s_motherpl->x + 20u;
                    //if(s_motherpl->mirror == V_MIRROR){leafx = s_motherpl->x - 20u;}
                    Sprite* leaf2 = SpriteManagerAdd(SpriteLeaf, leafx, ((UINT16)7 << 3));
                    struct PlatformInfo* leaf2_info = (struct PlatformInfo*)leaf2->custom_data;
                    leaf2_info->step = 160u;}
                break;
            }
            */
        //ENEMIES
        if(motherpl_state != DEATH && 
            help_cemetery_woman.mission_state < MISSION_STATE_ACCOMPLISHED){
            if(s_motherpl->x > (UINT16)80u && s_motherpl->x < ((mapwidth << 3) - 40u)){
                if(enemy_counter < 3 && enemy_wave > 0){
                    timeout_enemy--;
                    if(timeout_enemy == 200u){
                        enemy_wave--;
                        UINT16 e_x = s_motherpl->x + 80u;
                        if(s_motherpl->mirror == V_MIRROR){
                            e_x = s_motherpl->x - 80u;
                        }
                        SpriteManagerAdd(SpriteEnemyAttackerPine, e_x, (UINT16) 6u << 3);
                    }
                    if(timeout_enemy == 300u || timeout_enemy == 450u){
                        enemy_wave--;
                        UINT16 e_x = s_motherpl->x + 32u;
                        if(s_motherpl->mirror == V_MIRROR){
                            e_x = s_motherpl->x - 32u;
                        }
                        SpriteManagerAdd(SpriteEnemysimplesnake, e_x, (UINT16) 7u << 3);
                    }
                    if(timeout_enemy == 400u){
                        enemy_wave--;
                        UINT16 e_x = s_motherpl->x + 100u;
                        if(s_motherpl->mirror == V_MIRROR){
                            e_x = s_motherpl->x - 100u;
                        }
                        SpriteManagerAdd(SpriteEnemyAttackerPine, e_x, (UINT16) 6u << 3);
                        timeout_enemy = 500;
                    }
                }       
            }
        }
    
    Log(NONAME);
}

