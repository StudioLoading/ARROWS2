#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "Dialogs.h"

IMPORT_MAP(bordermine);
IMPORT_TILES(font);
IMPORT_TILES(harbortiles);
IMPORT_MAP(harbormap);
IMPORT_MAP(hudpl);

extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern INT8 motherpl_hp;
extern INT8 hud_motherpl_hp;
extern Sprite* s_motherpl;
extern UINT8 init_enemy;
extern UINT8 enemy_counter;
extern UINT8 MAX_ENEMY;
extern UINT8 mapwidth;
extern UINT8 mapheight;
extern UINT8 previous_state;
extern UINT16 motherpl_pos_x;
extern UINT16 motherpl_pos_y;
extern MirroMode motherpl_mirror; 
extern UINT8 npc_spawned_zone;
extern UINT8 item_spawned_cooldown;
extern struct MISSION golden_armor;
extern UINT8 powder_cooldown;
extern UINT8 itemspawned_powder_max;
extern UINT16 counter_birdsky;

const UINT8 coll_tiles_harbor[] = {1u, 63u, 82u, 83u, 89u, 90u,
                                91u, 92u, 93u, 95u, 96u, 98u, 100u, 140u, 0};
const UINT8 coll_surface_harbor[] = {75u, 86u, 89u, 90u, 0};

extern UINT8 tiles_anim_interval;
extern UINT16 timeout_enemy;
extern UINT8 timeout_cavesand;
extern UINT8 enemy_wave;
extern INT8 current_map;
extern CHAPTERS chapter;
extern struct MISSION mr_smee;
extern struct MISSION broken_ship;
extern struct MISSION pirate_strike;
extern struct MISSION captain;

extern UINT16 perc_10;
extern UINT16 perc_40; 
extern UINT16 perc_90;
extern INT8 motherpl_vx;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void spawnItem(INVITEMTYPE itemtype, UINT16 spawn_at_x, UINT16 spawn_at_y ) BANKED;
extern void spawn_npa(UINT8 type, UINT16 posx, UINT16 posy, UINT8 configured) BANKED;

Sprite* s_spugna;
struct NpcInfo* s_spugna_data;
Sprite* s_panzone;
struct NpcInfo* s_panzone_data;
Sprite* s_marine;
struct NpcInfo* s_marine_data;
Sprite* s_walker1;
struct NpcInfo* s_walker1_data;
Sprite* s_walker2;
struct NpcInfo* s_walker2_data;
Sprite* s_captain;
struct NpcInfo* s_captain_data;

UINT8 pirate_counter_30_100 = 0;
UINT8 max_seagulls_alive = 3;

void spawn_seagull_h(UINT16 gull_x, UINT16 gull_y) BANKED;
void harbor_init_pirates() BANKED;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        UINT16 spawnmotherplx = (UINT16) 10u << 3;
        UINT16 spawnmotherply = (UINT16) 6u << 3;
        s_motherpl = SpriteManagerAdd(SpriteMotherplarmor, spawnmotherplx, spawnmotherply);
        if(previous_state == StateInventory || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
        //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y); 
        InitScroll(BANK(harbormap), &harbormap, coll_tiles_harbor, coll_surface_harbor);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        enemy_counter = 0u;
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(harbormap), &harbormap, &mapwidth, &mapheight);
    //INIT PIRATES
        harbor_init_pirates();
    tiles_anim_interval = 0u;
    powder_cooldown = 60u;
    itemspawned_powder_max = 4;
    tiles_anim_interval = 0u;
    timeout_enemy = 10u;
    timeout_cavesand = 0u;
    counter_birdsky = 0u;
    perc_10 = (mapwidth/10) << 3;
    perc_40 = perc_10 << 2; 
    perc_90 = (mapwidth << 3) - perc_10;
    enemy_wave = 0u;
    SHOW_SPRITES;
    if(_cpu != CGB_TYPE){max_seagulls_alive = 1;}
}

void harbor_init_pirates() BANKED{
    UINT16 pirate_spawn_y = ((UINT16) 8u << 3) + 2u;
    //SPUGNA MR SMEE
        s_spugna = SpriteManagerAdd(SpritePgPirate, ((UINT16) 26u << 3) + 3u, pirate_spawn_y);
        s_spugna_data = (struct NpcInfo*) s_spugna->custom_data;
        s_spugna_data->npcname = SMEE;
        s_spugna_data->type = PIRATE_SPUGNA;
        switch(mr_smee.mission_state){
            case MISSION_STATE_DISABLED:
                s_spugna_data->whotalks = PIRATE_SPUGNA_0;
            break;
            case MISSION_STATE_ENABLED:
            case MISSION_STATE_STARTED:
                s_spugna_data->whotalks = PIRATE_SPUGNA_1;
            break;
            case MISSION_STATE_ACCOMPLISHED:
                s_spugna_data->whotalks = PIRATE_SPUGNA_2;
            break;
            case MISSION_STATE_REWARDED:
                s_spugna_data->whotalks = PIRATE_SPUGNA_3;
            break;
        }
        s_spugna_data->configured = 1;
    //SPUGNA PANZONE BOB
        s_panzone = SpriteManagerAdd(SpritePgPirate, ((UINT16) 28u << 3), pirate_spawn_y);
        s_panzone->mirror = V_MIRROR;
        s_panzone_data = (struct NpcInfo*) s_panzone->custom_data;
        s_panzone_data->npcname = BOB;
        s_panzone_data->type = PIRATE_PANZONE;
        s_panzone_data->whotalks = PIRATE_PANZONE_0; 
        s_panzone_data->configured = 1;
    //SPUGNA MARINE MARTIN
        s_marine = SpriteManagerAdd(SpritePgPirate, ((UINT16) 65u << 3), pirate_spawn_y);
        s_marine_data = (struct NpcInfo*) s_marine->custom_data;
        s_marine_data->npcname = MARTIN;
        s_marine_data->type = PIRATE_MARINAIO;
        //TODO s_marine_data->whotalks = 
        s_marine_data->configured = 1;
    //SPUGNA WALKER1 MAURICE
        s_walker1 = SpriteManagerAdd(SpritePgPirate, ((UINT16) 88u << 3), pirate_spawn_y);
        s_walker1_data = (struct NpcInfo*) s_walker1->custom_data;
        s_walker1_data->npcname = MAURICE;
        s_walker1_data->type = PIRATE_WALKER;
        s_walker1_data->vx = 1;
        s_walker1_data->max_frameskip = 1;
        //TODO s_walker1_data->whotalks = 
        s_walker1_data->configured = 1;
    /* SPUGNA WALKER2 MAURICE
        s_walker2 = SpriteManagerAdd(SpritePgPirate, ((UINT16) 37u << 3), pirate_spawn_y);
        s_walker2_data = (struct NpcInfo*) s_walker2->custom_data;
        s_walker2_data->npcname = MAURICE;
        s_walker2_data->type = PIRATE_WALKER;
        s_walker2_data->vx = -1;
        s_walker2_data->max_frameskip = 2;
        //TODO s_walker2_data->whotalks = 
        s_walker2_data->configured = 1;
    */
    // CAPTAIN ONE EYED JACK
        s_captain = SpriteManagerAdd(SpritePgPirate, ((UINT16) 108u << 3) - 4u, pirate_spawn_y);
        s_captain->mirror = V_MIRROR;
        s_captain_data = (struct NpcInfo*) s_captain->custom_data;
        s_captain_data->npcname = ONE_EYED_JACK;
        s_captain_data->type = PIRATE_CAPTAIN;
        s_captain_data->vx = 0;
        s_captain_data->max_frameskip = 0;
        if(mr_smee.mission_state < MISSION_STATE_REWARDED || broken_ship.mission_state < MISSION_STATE_REWARDED || pirate_strike.mission_state < MISSION_STATE_REWARDED){
            s_captain_data->whotalks = PIRATE_CAPTAIN_0;
        }else{
            s_captain_data->whotalks = PIRATE_CAPTAIN_1;
        }
        s_captain_data->configured = 1;
    // PARROT
        SpriteManagerAdd(SpritePgParrot, s_captain->x + 2u, s_captain->y - 6u);
}
void UPDATE(){
    pirate_counter_30_100++;
    if(pirate_counter_30_100 > 100u){
        pirate_counter_30_100 = 30u;
    }else{
        pirate_counter_30_100 += motherpl_vx;
    }
    //MOTHERPL RIGHT LIMIT FORCED   
        if(s_motherpl->x > ((UINT16) 111u << 3)){
            s_motherpl->x = (UINT16) 111u << 3;
        }
    //SPAWNING NPA    
        counter_birdsky++;
        if(counter_birdsky >= 1000){
            UINT16 birdsky_x = scroll_target->x + ((UINT16) 5 << 3);
            if(s_motherpl->mirror == V_MIRROR) {birdsky_x = scroll_target->x - ((UINT16) 5 << 3);}
            spawn_npa(SpriteBirdsky, birdsky_x, ((UINT16) 4 << 3), 2);
            birdsky_x = scroll_target->x + ((UINT16) 10 << 3);
            if(s_motherpl->mirror == V_MIRROR) {birdsky_x = scroll_target->x - ((UINT16) 10 << 3);}
            spawn_npa(SpriteBirdsky, birdsky_x, ((UINT16) 3 << 3), 2);
            counter_birdsky = 0u;
        }
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //GO TO INVENTORY
        if(KEY_PRESSED(J_START)){
            ChangeState(StateInventory, s_motherpl, -1);
        }
    //CAMERA MANAGEMENT
        update_camera_position();
    //INIT SUAGULLS
        if(s_motherpl->x > 80u && s_motherpl->x < ((UINT16) ((mapwidth << 3) - 80u))
            && enemy_counter < max_seagulls_alive){
            timeout_enemy--;
            if(timeout_enemy > 150u){
                timeout_enemy = 0u;
            }
            if(timeout_enemy == 0u){
                if(s_motherpl->x < perc_40){
                    timeout_enemy = 110u;
                }else if(s_motherpl->x > perc_40 && s_motherpl->x < perc_90){
                    timeout_enemy = 55u;
                }
                enemy_wave++;
                if(enemy_wave == 5u){
                    enemy_wave = 0u;
                }
                UINT16 gullx = s_motherpl->x + 140u;
                UINT16 gully = 16u;
                switch(enemy_wave){
                    case 1u:gully = 22u;break;
                    case 2u:gully = 14u;gullx = s_motherpl->x - 80u;break;
                    case 3u:gully = 26u;break;
                    case 4u:gully = 0u;gullx = s_motherpl->x - 100u;break;
                }
                spawn_seagull_h(gullx, gully);
            }
        }
    Log(NONAME);
}

void spawn_seagull_h(UINT16 gullx, UINT16 gully) BANKED{
    Sprite* s_gull = SpriteManagerAdd(SpriteSeagullHarbor, gullx, gully);
    struct EnemyData* data_gull = (struct EnemyData*) s_gull->custom_data;
    data_gull->configured = 1;
    data_gull->vx = -2;
    if(enemy_wave == 2 || enemy_wave == 4){
        data_gull->vx = 2;
    }
    if(gully == 56u && timeout_enemy == 40){
        data_gull->vx = -2;
    }
}
