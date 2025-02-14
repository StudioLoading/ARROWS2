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

#define HORDE_SNAKE 3//4
#define HORDE_RAT 2//4--
#define HORDE_SPIDER 1
#define HORDE_COBRA 2
#define HORDE_COOLDOWN_MAX 140

IMPORT_TILES(font);
IMPORT_TILES(blackiecavetiles);
IMPORT_TILES(hudpltiles);
IMPORT_MAP(blackieroommap);
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
extern WHOSTALKING whostalking;
extern UINT8 choice;
extern UINT16 timeout_enemy;
extern MOTHERPL_STATE motherpl_state;
extern CHAPTERS chapter;
extern UINT8 enemy_counter;

const UINT8 coll_tiles_blackieroom[] = {1u, 2u, 4u, 5u, 6u, 7u, 14u, 17u, 18u, 19u, 35u, 36u, 37u, 38u, 39u, 40u, 41u, 0};
const UINT8 coll_surface_blackieroom[] = { 16u, 29u, 31u, 33u, 50u, 0};
UINT8 horde_step = 2u;
UINT8 horde_counter = 0u;
UINT16 horde_cooldown = 0u;
UINT8 enemies_alive = 0u;
UINT8 mother_exit_cooldown = 60u;
UINT8 enemies_spawned = 0u;
UINT8 spider_countdown = 140u;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        if(chapter == CHAPTER_4_SHIP){
            s_motherpl = SpriteManagerAdd(SpriteMotherplarmor, (UINT16) 3u << 3, (UINT16) 8u << 3);
        }else{
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 3u << 3, (UINT16) 8u << 3);
        }
        if(previous_state == StateInv
            || (previous_state == StateDialog && choice == 0u)) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
    //INIT CHAR & MAP
        Sprite* s_blackie = SpriteManagerAdd(SpriteBlackie, (UINT16)16u << 3, (UINT16) 0u);
        struct NpcInfo* s_blackie_data = (struct NpcInfo*) s_blackie->custom_data;
        s_blackie_data->whotalks = BLACKIE;
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 80u, (UINT16) 56u); 
        InitScroll(BANK(blackieroommap), &blackieroommap, coll_tiles_blackieroom, coll_surface_blackieroom);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(blackieroommap), &blackieroommap, &mapwidth, &mapheight);
    //wolf_spawned = 0u;
    //timeout_drop = 0u;
    horde_cooldown = (HORDE_COOLDOWN_MAX/2);
    timeout_enemy = 200u;
    if(previous_state != StateInv && previous_state != StateDialog 
        && horde_step <= 5u ){//potrei esser morto durante un orda
        // azzerare counter
        horde_counter = 0u;
    }
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
        scroll_target->x = (UINT16) 80u;
        scroll_target->y = (UINT16) 56u;
    //FORCE MOTHERPL LIMITS
        if(s_motherpl->x < (UINT16)8u){
            s_motherpl->x = 8u;
        }
        if(s_motherpl->x > ((UINT16)19u << 3)){
            s_motherpl->x = ((UINT16)19u << 3);
        }
    //INIT ENEMIES
        if(horde_step > 5 && find_blackie.current_step < 2){
            if(enemies_spawned == 1){
                spider_countdown--;
                if(spider_countdown == 0){
                    if(enemy_counter == 0){
                        find_blackie.current_step = 2u;
                        SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
                    }
                }
            }
        }else{
            if(horde_cooldown == 0 && s_motherpl->y > 40u && find_blackie.current_step < 2u){
                if(timeout_enemy > 0){timeout_enemy--;}            
                else if (horde_step <= 5){
                    UINT8 enemy_type = SpriteEsimplesnake;
                    UINT8 horde_counter_max = 0u;
                    UINT16 spawn_enemy_x = (UINT16) 9u << 3;
                    UINT16 spawn_enemy_y = (UINT16) 52u;
                    switch(horde_step){
                        case 2u:
                            horde_counter_max = HORDE_SNAKE;
                            enemy_type = SpriteEsimplesnake; 
                        break;
                        case 3u://RAT HORDE
                            horde_counter_max = HORDE_RAT;
                            enemy_type = SpriteEsimplerat;
                        break;                
                        case 4u://COBRA HORDE
                            horde_counter_max = HORDE_COBRA;
                            enemy_type = SpriteEAttackerCobra;
                        break;           
                        case 5u://SPIDER HORDE
                            horde_counter_max = HORDE_SPIDER;
                            spawn_enemy_x = 20u;
                            spawn_enemy_y = 20u;
                            enemy_type = SpriteSpider;//SpriteEThrowerSpider;
                        break;
                    }            
                    if(horde_counter < horde_counter_max){
                        Sprite* s_snake2 = SpriteManagerAdd(enemy_type, spawn_enemy_x, spawn_enemy_y);
                        enemies_spawned = 1u;
                        if(horde_counter % 2 == 0){
                            struct EnemyData* s_snake2_data = (struct EnemyData*) s_snake2->custom_data;
                            s_snake2_data->vx = 4;
                        }
                        horde_counter++;
                        timeout_enemy = 120u;
                    }else{        
                        horde_counter = 0u;
                        horde_step++;
                        if(horde_step < 5){
                            enemies_spawned = 0;
                        }
                        timeout_enemy = 255u;
                        horde_cooldown = HORDE_COOLDOWN_MAX;
                    }
                }
            }else{
                horde_cooldown--;
                if(horde_cooldown > HORDE_COOLDOWN_MAX){
                    horde_cooldown = HORDE_COOLDOWN_MAX;
                }
            }
        }
    
    Log(NONAME);
}