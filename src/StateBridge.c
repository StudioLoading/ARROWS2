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

#define HORDE 5

IMPORT_MAP(bordermine);
IMPORT_TILES(font);
IMPORT_TILES(bridgetiles);
IMPORT_MAP(bridgemap);
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
extern struct MISSION outwalker_smith;
extern struct MISSION enable_hospital;
extern UINT8 powder_cooldown;
extern UINT8 itemspawned_powder_max;
extern UINT16 counter_birdsky;
extern UINT8 counter_fish;

const UINT8 coll_tiles_bridge[] = {1u, 63u, 82u, 83u,
                                91u, 92u, 93u, 95u, 96u, 98u, 100u, 0};
const UINT8 coll_surface_bridge[] = {75u, 86u, 89u, 90u, 0};

extern UINT8 tiles_anim_interval;
extern UINT16 timeout_enemy;
extern UINT8 timeout_cavesand;
extern UINT8 enemy_wave;

UINT16 perc_10;
UINT16 perc_40; 
UINT16 perc_90;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void spawnItem(INVITEMTYPE itemtype, UINT16 spawn_at_x, UINT16 spawn_at_y ) BANKED;
extern void spawn_npa(UINT8 type, UINT16 posx, UINT16 posy, UINT8 configured) BANKED;

void spawn_seagull(UINT16 gull_x, UINT16 gull_y) BANKED;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 10u << 3, (UINT16) 6u << 3);
        if(previous_state == StateInventory || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
        //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y); 
        InitScroll(BANK(bridgemap), &bridgemap, coll_tiles_bridge, coll_surface_bridge);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        enemy_counter = 0u;
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(bridgemap), &bridgemap, &mapwidth, &mapheight);
    tiles_anim_interval = 0u;
    powder_cooldown = 60u;
    itemspawned_powder_max = 4;
    tiles_anim_interval = 0u;
    timeout_enemy = 10u;
    timeout_cavesand = 0u;
    counter_birdsky = 0u;
    counter_fish = 0u;
    perc_10 = (mapwidth/10) << 3;
    perc_40 = perc_10 << 2; 
    perc_90 = (mapwidth << 3) - perc_10;
    enemy_wave = 0u;
    SHOW_SPRITES;
}

void UPDATE(){
    //SPAWNING NPA    
        counter_birdsky++;
        if(counter_birdsky >= 1000){
            spawn_npa(SpriteBirdsky, scroll_target->x + ((UINT16) 5 << 3), ((UINT16) 1 << 3), 2);
            spawn_npa(SpriteBirdsky, scroll_target->x + ((UINT16) 10 << 3), ((UINT16) 0 << 3), 2);
            counter_birdsky = 0u;
        }
        counter_fish++;
        switch(counter_fish){
            case 90:
            case 120:
                spawn_npa(SpriteFish, scroll_target->x + ((UINT16) 5 << 3), ((UINT16) 14 << 3), 2);
                spawn_npa(SpriteFish, scroll_target->x - ((UINT16) 1 << 3)+1, ((UINT16) 16 << 3) -2, 4);
                spawn_npa(SpriteFish, scroll_target->x + ((UINT16) 8 << 3), ((UINT16) 14 << 3) + 2, 1);
            break;
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
    //INIT ENEMIES
        if(s_motherpl->x > 80u && s_motherpl->x < ((UINT16) ((mapwidth << 3) - 80u))){
            timeout_enemy--;
            if(timeout_enemy > 150u){
                timeout_enemy = 0u;
            }
            if(timeout_enemy == 0u){
                if(s_motherpl->x < perc_40){
                    timeout_enemy = 80u;
                }else if(s_motherpl->x > perc_40 && s_motherpl->x < perc_90){
                    timeout_enemy = 40u;
                }
                enemy_wave++;
                if(enemy_wave == 5u){
                    enemy_wave = 0u;
                }
                UINT16 gullx = s_motherpl->x + 140u;
                UINT16 gully = 16u;
                switch(enemy_wave){
                    case 1u:gully = 52u;break;
                    case 2u:gully = 44u;break;
                    case 3u:gully = 56u;break;
                    case 4u:gully = 60u;break;
                }
                spawn_seagull(gullx, gully);
            }
        }
    Log(NONAME);
}

void spawn_seagull(UINT16 gullx, UINT16 gully) BANKED{
    Sprite* s_gull = SpriteManagerAdd(SpriteSeagull, gullx, gully);
    struct EnemyData* data_gull = (struct EnemyData*) s_gull->custom_data;
    data_gull->configured = 1;
    data_gull->vx = -1;
    if(gully == 56u && timeout_enemy == 40){
        data_gull->vx = -2;
    }
}
