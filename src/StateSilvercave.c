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

#define HORDE_COOLDOWN_MAX 800
#define HORDE_COUNTER_MAX 2
#define HORDE_STEP_MAX 4
 

IMPORT_MAP(bordermine);
IMPORT_TILES(font);
IMPORT_TILES(mapsilvert);
IMPORT_MAP(mapsilver);
IMPORT_MAP(hudpl);

extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
extern INT8 motherpl_hp;
extern INT8 hud_motherpl_hp;
extern Sprite* s_motherpl;
extern UINT8 enemy_counter;
extern UINT8 mapwidth;
extern UINT8 mapheight;
extern UINT8 previous_state;
extern UINT16 motherpl_pos_x;
extern UINT16 motherpl_pos_y;
extern MirroMode motherpl_mirror; 
extern UINT8 item_spawned_cooldown;

const UINT8 coll_tiles_silvercave[] = {2u, 6u, 10u, 21u, 22u,
27u, 35u, 93u, 0};
const UINT8 coll_surface_silvercave[] = {14u, 17u, 18u, 19u, 29u, 57u, 68u, 81u,
82u, 85u, 86u, 89u, 0};

extern UINT8 tiles_anim_interval;
extern UINT16 timeout_enemy;
extern UINT8 timeout_drop;
extern UINT8 horde_step;
extern UINT8 horde_counter;
extern UINT16 horde_cooldown;
extern CHAPTERS chapter;

UINT8 bastards_spawned = 0u;

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
        if(chapter == CHAPTER_4_SHIP){
            s_motherpl = SpriteManagerAdd(SpriteMotherplarmor, (UINT16) 7u << 3, (UINT16) 9u << 3);
        }else{
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 7u << 3, (UINT16) 9u << 3);
        }
        if(previous_state == StateInventory || previous_state == StateDialog || 
            previous_state == StateBossminotaur) {
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
    timeout_enemy = 120u;
    horde_step = 0;
    horde_counter = 0;
    horde_cooldown = HORDE_COOLDOWN_MAX;
    SHOW_SPRITES;
}

void UPDATE(){
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
    //SPAWN ENEMIES
        if(horde_counter == HORDE_COUNTER_MAX){
            horde_cooldown = HORDE_COOLDOWN_MAX;
            horde_counter = 0;
            horde_step++;
        }
        if(horde_cooldown > 0){horde_cooldown--;}
        if(horde_cooldown <= 0 && horde_step < HORDE_STEP_MAX){
            if(s_motherpl->x > ((UINT16) 22u << 3) && 
                s_motherpl->x < ((mapwidth << 3) - 64) && enemy_counter < 2 &&
                !(s_motherpl->x > ((UINT16) 82u << 3) && s_motherpl->y < ((UINT16) 20u << 3))
            ){
                timeout_enemy--;
                if(timeout_enemy == 0u){
                    timeout_enemy = 200u;                       
                    UINT16 spawn_posx = spawn_posx = s_motherpl->x - 32u;
                    if((s_motherpl->x | 0x0001 ) == 0){spawn_posx = s_motherpl->x + 42u;}
                    UINT16 spawn_posy = (UINT16) 25u << 3;
                    if(s_motherpl->y < ((UINT16) 20u << 3)){
                        spawn_posy = (UINT16) 9u << 3;
                    }
                    //up left => spiderwall    
                    if(s_motherpl->y < ((UINT16) 24u << 3) && 
                        s_motherpl->x < ((UINT16) 83u << 3)){
                        timeout_enemy = 200u;
                        spawn_posy = s_motherpl->y - 24u;
                        SpriteManagerAdd(SpriteSpider, spawn_posx, spawn_posy);
                    }else{
                        timeout_enemy = 120u;
                        SpriteManagerAdd(SpriteEnemyThrowerSpider, spawn_posx, spawn_posy);                    
                    }
                    horde_counter++;
                }
            }
        }
    //METAL SPECIAL
        if(s_motherpl->x > ((UINT16) 110u << 3) && s_motherpl->y < ((UINT16) 8u << 3)){
            if(bastards_spawned == 0u){
                spawnItem(INVITEM_ARROW_BASTARD, ((UINT16) 113u << 3), ((UINT16) 1u << 3));
                bastards_spawned = 1u;
            }
        }
    Log(NONAME);
}

