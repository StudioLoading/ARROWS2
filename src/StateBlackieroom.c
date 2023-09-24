#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "BankManager.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Fade.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "Dialogs.h"

#define HORDE_SNAKE 3//4
#define HORDE_RAT 2//4--
#define HORDE_SPIDER 1
#define HORDE_COBRA 2
#define HORDE_COOLDOWN_MAX 140

IMPORT_MAP(bordercave);
IMPORT_TILES(font);
IMPORT_TILES(blackiecavetiles);
IMPORT_TILES(hudpltiles);
IMPORT_MAP(blackieroommap);
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
extern INT8 motherpl_vx;
extern UINT8 npc_spawned_zone;
extern struct MISSION find_blackie;
extern WHOSTALKING whostalking;
extern UINT8 choice;
extern UINT16 timeout_enemy;
extern struct EtoReload e_to_reload[3];
extern MOTHERPL_STATE motherpl_state;

const UINT8 coll_tiles_blackieroom[] = {1u, 2u, 4u, 5u, 6u, 7u, 14u, 17u, 18u, 19u, 35u, 36u, 37u, 38u, 39u, 40u, 41u, 0};
const UINT8 coll_surface_blackieroom[] = { 16u, 29u, 31u, 33u, 50u, 0};
UINT8 horde_step = 0u;
UINT8 horde_counter = 0u;
UINT16 horde_cooldown = 0u;
UINT8 enemies_alive = 0u;
UINT8 mother_exit_cooldown = 60u;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;

void START(){
    LOAD_SGB_BORDER(bordercave);
    //SOUND
        NR52_REG = 0x80; //Enables sound, you should always setup this first
        NR51_REG = 0xFF; //Enables all channels (left and right)
        NR50_REG = 0x77; //Max volume
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 3u << 3, (UINT16) 8u << 3);
        if(previous_state == StateInventory
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
    if(previous_state != StateInventory && previous_state != StateDialog 
        && horde_step < 8u ){//potrei esser morto durante un orda
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
        if(KEY_PRESSED(J_START)){ChangeState(StateInventory, s_motherpl, -1);}
    //CAMERA MANAGEMENT
        scroll_target->x = (UINT16) 80u;
        scroll_target->y = (UINT16) 56u;
        /*
        //CAMERA MANAGEMENT
        update_camera_position();
        */
    //FORCE MOTHERPL LIMITS
        if(s_motherpl->x < (UINT16)8u){
            s_motherpl->x = 8u;
            mother_exit_cooldown--;
            if(mother_exit_cooldown == 0u && motherpl_state == MOTHERPL_WALK){
                mother_exit_cooldown = 60u;
                previous_state = StateBlackieroom;
                ChangeState(StateBlackiecave, s_motherpl, -1);
                //go back
            }
        }else if(mother_exit_cooldown != 60u){
            mother_exit_cooldown = 60u;
        }
        if(s_motherpl->x > ((UINT16)19u << 3)){
            s_motherpl->x = ((UINT16)19u << 3);
        }
    //INIT ENEMIES
        if(horde_cooldown == 0 && s_motherpl->y > 40u && find_blackie.current_step < 2u){
            if(timeout_enemy > 0){timeout_enemy--;}            
            else{
                UINT8 enemy_type = SpriteEnemysimplesnake;
                UINT8 horde_counter_max = 0u;         
                switch(horde_step){
                    case 0u://SNAKE HORDE
                    case 2u:
                        horde_counter_max = HORDE_SNAKE;
                        enemy_type = SpriteEnemysimplesnake; 
                    break;
                    case 1u://RAT HORDE
                    case 3u:
                        horde_counter_max = HORDE_RAT;
                        enemy_type = SpriteEnemysimplerat;
                    break;                
                    case 5u://COBRA HORDE
                    case 7u:
                        horde_counter_max = HORDE_COBRA;
                        enemy_type = SpriteEnemyAttackerCobra;
                    break;           
                    case 6u://SPIDER HORDE
                    case 8u:
                        horde_counter_max = HORDE_SPIDER;
                        enemy_type = SpriteEnemyThrowerSpider;
                    break;
                    default:
                    //case 9u:
                        find_blackie.current_step = 2u;
                    break;
                }            
                if(horde_counter <= horde_counter_max){
                    Sprite* s_snake2 = SpriteManagerAdd(enemy_type, (UINT16) 9u << 3, (UINT16) 52u);
                    if(horde_counter % 2 == 0){
                        struct EnemyData* s_snake2_data = (struct EnemyData*) s_snake2->custom_data;
                        s_snake2_data->vx = 4;
                    }
                    horde_counter++;
                    timeout_enemy = 100u;
                }else{        
                    UINT8 i = 0u;
                    enemies_alive = 0u;
                    for(i = 0u; i < 3u; ++i){
                        if(e_to_reload[i].alive == 1u){
                            enemies_alive++;
                        }
                    }
                    if(enemies_alive == 0u){
                        horde_counter = 0u;
                        horde_step++;
                        timeout_enemy = 255u;
                        horde_cooldown = HORDE_COOLDOWN_MAX;
                    }
                }
            }
        }else{
            horde_cooldown--;
            if(horde_cooldown > (HORDE_COOLDOWN_MAX/2)){
                horde_cooldown = (HORDE_COOLDOWN_MAX/2);
            }
        }

    
    Log(NONAME);
}