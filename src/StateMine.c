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
#include "Music.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "Dialogs.h"

#define HORDE 5

IMPORT_MAP(bordermine);
IMPORT_TILES(fontbw);
IMPORT_TILES(cavetiles);
IMPORT_MAP(cavemap);
IMPORT_MAP(hudpl);

extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern INT8 motherpl_hp;
extern INT8 motherpl_ups;
extern INT8 hud_motherpl_hp;
extern INT8 hud_motherpl_ups;
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
extern UINT8 motherpl_hit_cooldown;
extern INT8 motherpl_vx;
extern UINT8 npc_spawned_zone;
extern UINT8 item_spawned_cooldown;

const UINT8 coll_tiles_cave[] = {1u, 11u, 12u, 25u, 33u, 35u, 52u, 0};
const UINT8 coll_surface_cave[] = {14u, 17u, 18u, 19u, 24u, 53u, 65u, 0};

UINT8 tiles_anim_interval = 60u;
UINT8 timeout_enemy = 1u;
UINT8 timeout_cavesand = 0u;
UINT8 enemy_wave = 0u;
Sprite* s_superstone = 0;
UINT8 superstone_spawned = 0u;

extern void UpdateHUD() BANKED;
extern void Log() BANKED;
extern void update_camera_position() BANKED;
extern void camera_tramble() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos) BANKED;
extern void Anim_Cave_0() BANKED;
extern void Anim_Cave_1() BANKED;

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
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y); 
        InitScroll(BANK(cavemap), &cavemap, coll_tiles_cave, coll_surface_cave);    
    //HUD
        INIT_FONT(fontbw, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        enemy_counter = 0u;
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(cavemap), &cavemap, &mapwidth, &mapheight);
    if(superstone_spawned < 2){
        superstone_spawned = 0u;
    }
	SHOW_SPRITES;
}

void UPDATE(){
    //SPAWNING ITEM COOLDOWN
        if(item_spawned_cooldown > 0u){
            item_spawned_cooldown--;
        }
    //CAVE TILES ANIM
        tiles_anim_interval--;
        if(tiles_anim_interval == 0u){
            Anim_Cave_0();
            tiles_anim_interval = 16u;
        }
        if(tiles_anim_interval == 8u){
            Anim_Cave_1();
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
        if(hud_motherpl_hp != motherpl_hp || hud_motherpl_ups != motherpl_ups){
            UpdateHUD();
        }
    //GO TO INVENTORY
        if(KEY_PRESSED(J_START)){
            ChangeState(StateInventory, s_motherpl);
        }
    //CAMERA MANAGEMENT
        if(motherpl_hit_cooldown > 0 && motherpl_vx == 0){
            //CAMERA TRAMBLE
            camera_tramble();
        }else{
            //SCROLL CAMERA
            update_camera_position();
        }
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
                timeout_enemy = 255u;
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
        }
    //ENEMIES
        /*
        if(test_countdown == 0u){
            test_countdown = 255u;
            switch(init_enemy){
                case 1u:
                    SpriteManagerAdd(SpriteEnemysimplesnake, (UINT16) 12u << 3, (UINT16) 6u << 3);
                break;
                case 2u:
                    //SpriteManagerAdd(SpriteEnemysimplerat, (UINT16) 12u << 3, (UINT16) 6u << 3);
                break;
                case 3u:
                    //SpriteManagerAdd(SpriteEnemyAttackerPine, (UINT16) 12u << 3, (UINT16) 6u << 3);
                break;
                case 4u:

                break;
                case 5u:
                    SpriteManagerAdd(SpriteEnemyThrowerSpider, (UINT16) 12u << 3, (UINT16) 6u << 3);
                break;
                case 6u:
                    //SpriteManagerAdd(SpriteEnemyThrowerTarantula, (UINT16) 12u << 3, (UINT16) 6u << 3);
                break;
                case 7u:
                    init_enemy = 0;
                break;
            }
            init_enemy++;
        }
        */    
    //MANAGE NPC
    /*
        if(s_motherpl->x < ((UINT16)40u << 3)){
            if(npc_spawned_zone != 1u){
                spawn_npc(SpritePgexzoo, (UINT16) 25u << 3, 76u, WOMAN_HEAD1, WOMAN_BODY1, NO_MIRROR, EXZOO_WOMAN1);
                spawn_npc(SpritePgexzoo, (UINT16) 27u << 3, 76u, WOMAN_HEAD2, WOMAN_BODY2, V_MIRROR, EXZOO_WOMAN2);
                npc_spawned_zone = 1u;
            }
        }else if(s_motherpl->x < ((UINT16)70u << 3)){
            if(npc_spawned_zone != 2u){
                spawn_npc(SpritePgexzoo, (UINT16) 55u << 3, 76u, MAN_HEAD2, MAN_BODY2, V_MIRROR, EXZOO_MAN2);
                npc_spawned_zone = 2u;
            }
        }else if(s_motherpl->x < ((UINT16)120u << 3)){
            if(npc_spawned_zone != 3u){
                spawn_npc(SpritePgexzoo, (UINT16) 85u << 3, 76u, MAN_HEAD1, MAN_BODY1, V_MIRROR, EXZOO_MAN1);
                spawn_npc(SpritePgexzoo, (UINT16) 87u << 3, 76u, WOMAN_HEAD1, WOMAN_BODY3, NO_MIRROR, EXZOO_WOMAN3);
                npc_spawned_zone = 3u;
            }
        }
    */
    Log();
}

