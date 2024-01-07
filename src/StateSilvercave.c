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
IMPORT_TILES(mapsilvert);
IMPORT_MAP(mapsilver);
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

const UINT8 coll_tiles_silvercave[] = {2u, 6u, 7u, 8u, 10u, 11u, 12u,
27u, 29u, 35u, 0};
const UINT8 coll_surface_silvercave[] = {14u, 17u, 18u, 19u, 0};

extern UINT8 tiles_anim_interval;
extern UINT16 timeout_enemy;
extern UINT8 enemy_wave;
extern UINT8 timeout_drop;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void spawnItem(INVITEMTYPE itemtype, UINT16 spawn_at_x, UINT16 spawn_at_y ) BANKED;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 9u << 3, (UINT16) 9u << 3);
        if(previous_state == StateInventory || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y); 
        InitScroll(BANK(mapsilver), &mapsilver, coll_tiles_silvercave, coll_surface_silvercave);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        enemy_counter = 0u;
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(mapsilver), &mapsilver, &mapwidth, &mapheight);
    tiles_anim_interval = 0u;
    timeout_drop = 0;
    powder_cooldown = 60u;
    itemspawned_powder_max = 4;
    SHOW_SPRITES;
}

void UPDATE(){
    //COOLDOWNS
        if(powder_cooldown > 0){powder_cooldown--;}
    //SPAWNING ITEM COOLDOWN
        if(item_spawned_cooldown > 0u){
            item_spawned_cooldown--;
        }
    //CAVE TILES ANIM
        tiles_anim_interval++;
        switch(tiles_anim_interval){
            case 6u:
                Anim_Silver_1();
            break;
            case 12u:
                Anim_Silver_2();
            break;
            case 18u:
                Anim_Silver_3();
            break;
            case 24u:
                Anim_Silver_4();
            break;
            case 30u:
                Anim_Silver_5();
            break;
            case 36u:
                Anim_Silver_0();
                tiles_anim_interval = 0u;
            break;
        }
    //CAVESAND ANIM
        timeout_drop--;
        if(timeout_drop == 80u){
            SpriteManagerAdd(SpriteDrop, s_motherpl->x+24u, s_motherpl->y-80u);
        }
        if(timeout_drop == 160u){
            SpriteManagerAdd(SpriteDrop, s_motherpl->x-12u, s_motherpl->y-80u);
        }
        if(timeout_drop == 0u){
            SpriteManagerAdd(SpriteDrop, s_motherpl->x+64u, s_motherpl->y-80u);
            timeout_drop = 240u;
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
    /*
        UINT16 spawn_posx = 0u;
        if(s_motherpl->x > ((UINT16)50u)<<3 && 
            s_motherpl->x < ((UINT16)93u)<<3){
            spawn_posx = 73u;
        }
        if(s_motherpl->x > ((UINT16)154u)<<3 && 
            s_motherpl->x < ((UINT16)184u)<<3){
            spawn_posx = 169u;
        }
        if(spawn_posx == 0u && enemy_wave > 0){
            enemy_wave = 0u;
        }
        if(s_motherpl && spawn_posx && enemy_wave < HORDE){
            timeout_enemy--;
            if(timeout_enemy == 0u){
                timeout_enemy = 200u;
                switch(init_enemy){
                    case 1u:
                    case 2u:
                    case 3u:
                        SpriteManagerAdd(SpriteEnemysimplesnake, (UINT16) spawn_posx << 3, (UINT16) 6u << 3);
                        enemy_wave++;
                    break;
                    case 4u:
                        SpriteManagerAdd(SpriteEnemysimplerat, (UINT16) spawn_posx << 3, (UINT16) 6u << 3);
                        enemy_wave++;
                    break;
                    case 5u:
                        init_enemy = 0;
                    break;
                }
                init_enemy++;
            }
        }else if (timeout_enemy != 10u){
            timeout_enemy = 10u;
        }
    */
    //METAL SPECIAL
        if(s_motherpl->x > ((UINT16) 190u << 3)){
            if(enable_hospital.mission_state == MISSION_STATE_ENABLED && enable_hospital.current_step == 0){
                spawnItem(INVITEM_METAL_SPECIAL, ((UINT16) 195u << 3), ((UINT16) 7u << 3));
                enable_hospital.current_step = 1;
            }
        }
    Log(NONAME);
}

