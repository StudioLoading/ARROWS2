#include "Banks/SetAutoBank.h"

#include "SGB.h"
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
#include "sgb_palette.h"
#include "Dialogs.h"

#define HORDE 5

IMPORT_MAP(bordermine);
IMPORT_TILES(font);
IMPORT_TILES(minetiles);
IMPORT_MAP(minemap);
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
extern INT8 motherpl_vx;
extern UINT8 npc_spawned_zone;
extern UINT8 item_spawned_cooldown;
extern struct MISSION outwalker_smith;
extern UINT8 powder_cooldown;
extern UINT8 itemspawned_powder_max;

const UINT8 coll_tiles_cave[] = {1u, 10u, 11u, 12u, 35u, 52u, 84u, 85u, 86u, 87u, 90u, 91u, 0};
const UINT8 coll_surface_cave[] = {14u, 17u, 18u, 19u, 53u, 65u, 0};

UINT8 tiles_anim_interval = 0u;
UINT16 timeout_enemy = 10u;
UINT8 timeout_cavesand = 0u;
extern UINT8 enemy_wave;
Sprite* s_superstone = 0;
UINT8 superstone_spawned = 0u;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;

void START(){
    LOAD_SGB_BORDER(bordermine);
    //SOUND
        NR52_REG = 0x80; //Enables sound, you should always setup this first
        NR51_REG = 0xFF; //Enables all channels (left and right)
        NR50_REG = 0x77; //Max volume
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 6u << 3, (UINT16) 7u << 3);
        if(previous_state == StateInventory || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
        if(outwalker_smith.mission_state == MISSION_STATE_STARTED){
            UINT8 spawn_flower_1 = outwalker_smith.current_step & 0b00000001;
            if(spawn_flower_1 == 0){
                Sprite* s_flower1 = SpriteManagerAdd(SpriteFlower, (UINT16)19u << 3, (UINT16)8u << 3);
                struct ItemSpawned* flower1_data = (struct ItemSpawned*) s_flower1->custom_data;
                flower1_data->hp = 0b00000001;
            }
        }
      	//SpriteManagerAdd(SpriteFlame, (UINT16) 16u << 3, (UINT16) 5u << 3);
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y); 
        InitScroll(BANK(minemap), &minemap, coll_tiles_cave, coll_surface_cave);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        enemy_counter = 0u;
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(minemap), &minemap, &mapwidth, &mapheight);
    if(superstone_spawned < 2){
        superstone_spawned = 0u;
    }
    tiles_anim_interval = 0u;
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
                Anim_Cave_1();
            break;
            case 12u:
                Anim_Cave_2();
            break;
            case 18u:
                Anim_Cave_3();
            break;
            case 24u:
                Anim_Cave_4();
            break;
            case 30u:
                Anim_Cave_5();
            break;
            case 36u:
                Anim_Cave_0();
                tiles_anim_interval = 0u;
            break;
        }
    //CAVESAND ANIM
        if(
            (s_motherpl->x>((UINT16)43u)<<3 && s_motherpl->x<((UINT16)63u)<<3)
            ||
            (s_motherpl->x>((UINT16)83u)<<3 && s_motherpl->x<((UINT16)103u)<<3)
            || 
            (s_motherpl->x>((UINT16)132u)<<3)
        )        
        {
            timeout_cavesand--;
            if(timeout_cavesand == 80u){
                SpriteManagerAdd(SpriteCavesand, s_motherpl->x+24u, (UINT16) 24u);
            }
            if(timeout_cavesand == 160u){
                SpriteManagerAdd(SpriteCavesand, s_motherpl->x-12u, (UINT16) 24u);
            }
            if(timeout_cavesand == 0u){
                SpriteManagerAdd(SpriteCavesand, s_motherpl->x+64u, (UINT16) 24u);
                timeout_cavesand = 240u;
            }
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
    //INIT SUPERSTONE
        if(s_motherpl->x > (110u << 3) && superstone_spawned == 0u){
            //s_superstone = SpriteManagerAdd(SpriteSuperstone, 130u << 3, 13u << 3);
            superstone_spawned = 1u;
        }
    //INIT ENEMIES
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
    
    Log(NONAME);
}

