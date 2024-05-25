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
IMPORT_TILES(hoodswnwtiles);
IMPORT_MAP(hoodswnwmap);
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

const UINT8 coll_tiles_hood[] = {1u, 10u, 14u, 17u, 18u, 19u, 92u, 94u, 105u, 0};
const UINT8 coll_surface_hood[] = {83u, 0};
Sprite* s_child;
UINT16 spawn_child_cooldown = 100u;

void spawn_enemy_hood() BANKED;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;

UINT16 test_counter = 120u;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        switch(current_map){
            case 0://comes from south-west
                s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 10u << 3, (UINT16) 9u << 3);
            break;
            case 1://comes from north-west
                s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 91u << 3, (UINT16) 9u << 3);
                s_motherpl->mirror = V_MIRROR;
            break;
        }
        if(previous_state == StateInventory || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
         if(help_cemetery_woman.mission_state == MISSION_STATE_STARTED && child_hooked == 1){
            s_child = SpriteManagerAdd(SpriteChild, (UINT16)(s_motherpl->x + 12u), (UINT16)84u);
            if(help_cemetery_woman.current_step < 3){
                SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
            }
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y); 
        InitScroll(BANK(hoodswnwmap), &hoodswnwmap, coll_tiles_hood, coll_surface_hood);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        enemy_counter = 0u;
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(hoodswnwmap), &hoodswnwmap, &mapwidth, &mapheight);
	SHOW_SPRITES;
    timeout_enemy = 400u;
    generic_counter = 60u;
    spawn_child_cooldown = 100u;
    if(previous_state == StateOverworld){
        enemy_wave = 12;
    }
}

void UPDATE(){
    if(timeout_enemy > 600){
        timeout_enemy = 600;
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
        /*if(test_counter > 0){
            test_counter--;
            if(test_counter == 0){
                SpriteManagerAdd(SpriteArmor, s_motherpl->x + 20u, s_motherpl->y - 8u);
            }
        }*/
        //LEAF
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
                    leaf2_info->step = 160u;}break;
            }
        //ENEMIES
        if(motherpl_state != DEATH){
            if(help_cemetery_woman.mission_state < MISSION_STATE_ACCOMPLISHED){
                if(s_motherpl->x > (UINT16)80u && s_motherpl->x < ((mapwidth << 3) - 80u)){
                    switch(help_cemetery_woman.current_step){
                        case 2u:
                            spawn_enemy_hood();
                        break;
                        case 3u:
                            if(s_motherpl->x > ((UINT16) 60u << 3) && child_hooked == 0){
                                if(s_motherpl->x > ((UINT16) 70u << 3)){
                                    s_motherpl->x = (UINT16) 70u << 3;
                                }
                                s_child = SpriteManagerAdd(SpriteChild, (UINT16)(s_motherpl->x + 24u), (UINT16) 84u);
                                child_hooked = 1;
                                help_cemetery_woman.current_step = 2;
                            }
                        break;
                        case 1u:
                            if(s_motherpl->x > ((UINT16) 60u << 3) && child_hooked == 0){
                                if(s_motherpl->x > ((UINT16) 70u << 3)){
                                    s_motherpl->x = (UINT16) 70u << 3;
                                }
                                spawn_child_cooldown--;
                                if(spawn_child_cooldown){
                                    child_hooked = 1;
                                    SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
                                    trigger_dialog(CHILD, s_motherpl);
                                }
                            }
                        break; 
                        case 4u:
                            if(enemy_counter < 2){
                                timeout_enemy--;
                                if(timeout_enemy == 200u){
                                    SpriteManagerAdd(SpriteEnemyAttackerPine, (UINT16)(s_motherpl->x - 120u), (UINT16) 6u << 3);
                                }
                                if(timeout_enemy == 0u){
                                    timeout_enemy = 600u;
                                    SpriteManagerAdd(SpriteEnemysimplesnake, (UINT16)(s_motherpl->x - 80u), (UINT16) 6u << 3);
                                }
                            }                   
                    }            
                }
            }else{//help_cemetery_woman rewarded already
                spawn_enemy_hood();
            }
        }
    
    Log(NONAME);
}

void spawn_enemy_hood() BANKED{
    if(enemy_counter < 2 && enemy_wave > 0){
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

