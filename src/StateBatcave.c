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

#define HORDE_COBRA 1
#define HORDE_SPIDER 2
#define ENEMY_TIMEOUT_MAX 220
#define ENEMY_TIMEOUT_MIN 20

IMPORT_TILES(font);
IMPORT_TILES(batcavetiles);
IMPORT_MAP(mapbatcave);
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
extern UINT8 enemy_wave;
extern UINT8 child_hooked;
extern MOTHERPL_STATE motherpl_state;
extern UINT8 powder_cooldown;
extern UINT8 item_spawned_cooldown;
extern UINT8 itemspawned_powder_max;
extern struct OwSpriteInfo* motherow_info;
extern CHAPTERS chapter;
extern struct MISSION mr_smee;

const UINT8 coll_tiles_batcave[] = {1u, 2u, 4u, 5u, 6u, 7u, 14u,
17u, 18u, 19u, 34u, 35u, 36u, 37u, 38u, 39u, 40u, 41u, 54u, 55u, 0};
const UINT8 coll_surface_batcave[] = { 16u, 29u, 31u, 33u, 0};

UINT8 first_time = 0u;
UINT8 enemy_wave_zone = 0u;
UINT8 current_horde_max = 0u;
UINT8 cadaver_spawned = 0u;

void spawn_enemy_batcave(UINT8 sprite_enemy_type, UINT16 pos_x, UINT16 pos_y) BANKED;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern UINT8 is_item_equippable(INVITEMTYPE itemtype) BANKED;
extern void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos, NPCNAME npcname) BANKED;


void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        if(chapter == CHAPTER_4_SHIP){
            s_motherpl = SpriteManagerAdd(SpriteMotherplarmor, 32u, (UINT16) 6u << 3);
        }else{
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, 32u, (UINT16) 6u << 3);
        }
        if(previous_state == StateInventory || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y); 
        InitScroll(BANK(mapbatcave), &mapbatcave, coll_tiles_batcave, coll_surface_batcave);
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        enemy_counter = 0u;
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(mapbatcave), &mapbatcave, &mapwidth, &mapheight);
	SHOW_SPRITES;
    timeout_enemy = ENEMY_TIMEOUT_MAX;
    generic_counter = 60u;
    itemspawned_powder_max = 4;
    cadaver_spawned = 0;
    if(previous_state == StateOverworld){
        enemy_wave = 0;
    }
}

void UPDATE(){
    if(timeout_enemy > ENEMY_TIMEOUT_MAX){
        timeout_enemy = ENEMY_TIMEOUT_MAX;
    }    
    //COOLDOWNS
        if(powder_cooldown > 0){powder_cooldown--;}
    //SPAWNING ITEM COOLDOWN
        if(item_spawned_cooldown > 0u){
            item_spawned_cooldown--;
        }
    //SPAWNING ITEMS
        if(s_motherpl->x > 80u && first_time == 0u){
            first_time = 1u;
            UINT16 quantity = 1u;
            INVITEMTYPE itemtype = INVITEM_HEARTS;     
            Sprite* s_item_heart = SpriteManagerAdd(SpriteItemspawned, ((UINT16) 12u << 3), ((UINT16) 5u << 3));
            struct ItemSpawned* item_heart_data = (struct ItemSpawned*) s_item_heart->custom_data;
            item_heart_data->itemtype = itemtype;
            item_heart_data->quantity = quantity;
            item_heart_data->equippable = is_item_equippable(itemtype);
            item_heart_data->configured = 1u;
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
        //DROP
            /*generic_counter--;
            switch(generic_counter){
                case 0u:generic_counter = 60u;break;
                case 25u:SpriteManagerAdd(SpriteDrop, ((UINT16) 51u << 3), ((UINT16)14u << 3));break;
                case 50u:SpriteManagerAdd(SpriteDrop, ((UINT16) 53u << 3), ((UINT16)14u << 3));break;
            }*/
        //ENEMIES
            if(motherpl_state != DEATH){
                UINT16 spawn_posx = 0u;
                UINT16 spawn_posy = 0u;
                UINT8 sprite_enemy_type = 0u;
                if(s_motherpl->x > ((UINT16)26u)<<3 && 
                    s_motherpl->x < ((UINT16)50u)<<3){
                    spawn_posx = 38u;
                    spawn_posy = 9u;
                    //sprite_enemy_type = SpriteEnemyAttackerCobra;
                    sprite_enemy_type = SpriteEnemyBat;
                    if(enemy_wave_zone != 1){
                        enemy_wave = 0;
                        enemy_wave_zone = 1;
                        current_horde_max = HORDE_COBRA;
                    }
                }else if(s_motherpl->x > ((UINT16)56u)<<3 && 
                    s_motherpl->x < ((UINT16)81u)<<3){
                    spawn_posx = 68u;
                    spawn_posy = 1u;
                    //sprite_enemy_type = SpriteSpider;
                    sprite_enemy_type = SpriteEnemyBat;
                    if(enemy_wave_zone != 2){
                        enemy_wave = 0;
                        enemy_wave_zone = 2;
                        current_horde_max = HORDE_SPIDER;
                    }
                }else if(s_motherpl->x > ((UINT16)86u)<<3 &&
                    s_motherpl->x < ((UINT16)100u)<<3){
                    spawn_posx = 92u;
                    spawn_posy = 1u;
                    //sprite_enemy_type = SpriteEnemyThrowerTarantula;
                    sprite_enemy_type = SpriteEnemyBat;
                    if(enemy_wave_zone != 3){
                        enemy_wave = 0;
                        enemy_wave_zone = 3;
                        current_horde_max = HORDE_SPIDER;
                    }
                }else if(s_motherpl->x > ((UINT16)100u)<<3){
                    spawn_posx = 108u;
                    spawn_posy = 4u;
                    //sprite_enemy_type = SpriteEnemyThrowerTarantula;
                    sprite_enemy_type = SpriteEnemyBat;
                    if(enemy_wave_zone != 4){
                        enemy_wave = 0;
                        enemy_wave_zone = 4;
                        current_horde_max = HORDE_SPIDER;
                    }
                }else{
                    enemy_wave_zone = 0u;
                }
                if(spawn_posx && spawn_posy && sprite_enemy_type && 
                    enemy_wave_zone && enemy_wave < current_horde_max){
                    if(enemy_counter < 3){//per questione di flickering
                        timeout_enemy--;
                        switch(timeout_enemy){
                            case 100u:
                            case 200u:
                                spawn_enemy_batcave(sprite_enemy_type,spawn_posx,spawn_posy);
                            break;
                            case ENEMY_TIMEOUT_MIN:timeout_enemy = ENEMY_TIMEOUT_MAX;break;
                        }
                    }
                }
            }
        //CADAVER
            if(s_motherpl->x > ((UINT16) 125u << 3) && mr_smee.mission_state > MISSION_STATE_ENABLED && cadaver_spawned == 0){
                spawn_npc(SpritePgCadaver, ((UINT16)137u << 3), ((UINT16) 12u << 3), PIRATE_HEADLESS_RICK, PIRATE_HEADLESS_RICK, NO_MIRROR, CADAVER, RICK);
                cadaver_spawned = 1;
            }
    
    Log(NONAME);
}

void spawn_enemy_batcave(UINT8 sprite_enemy_type, UINT16 pos_x, UINT16 pos_y) BANKED{
    enemy_wave++;
    SpriteManagerAdd(sprite_enemy_type, ((UINT16)pos_x)<<3, ((UINT16)pos_y)<<3);
}