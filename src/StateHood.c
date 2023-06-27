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

IMPORT_MAP(border);
IMPORT_TILES(fontbw);
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
extern UINT8 motherpl_hit_cooldown;
extern INT8 motherpl_vx;
extern UINT8 npc_spawned_zone;
extern UINT8 generic_counter;
extern struct MISSION missions[4];
extern WHOSTALKING whostalking;
extern UINT16 timeout_enemy;
extern UINT8 enemy_counter;

const UINT8 coll_tiles_hood[] = {1u, 10u, 14u, 17u, 18u, 19u, 0};
const UINT8 coll_surface_hood[] = {0};
Sprite* s_child;
UINT16 spawn_child_cooldown = 100u;

extern void UpdateHUD() BANKED;
extern void Log() BANKED;
extern void update_camera_position() BANKED;
extern void camera_tramble() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos) BANKED;

UINT16 test_counter = 120u;

void START(){
    LOAD_SGB_BORDER(border);
    //SOUND
        NR52_REG = 0x80; //Enables sound, you should always setup this first
        NR51_REG = 0xFF; //Enables all channels (left and right)
        NR50_REG = 0x77; //Max volume
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 10u << 3, (UINT16) 9u << 3);
        if(previous_state == StateInventory || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
        if(missions[2].current_step == 2u){
            s_child = SpriteManagerAdd(SpriteChild, (UINT16)(s_motherpl->x + 12u), (UINT16)84u);
            missions[2].current_step = 3u;
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y); 
        InitScroll(BANK(hoodswnwmap), &hoodswnwmap, coll_tiles_hood, coll_surface_hood);    
    //HUD
        INIT_FONT(fontbw, PRINT_BKG);
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
}

void UPDATE(){
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //GO TO INVENTORY
        if(KEY_PRESSED(J_START)){ChangeState(StateInventory, s_motherpl);}
    //CAMERA MANAGEMENT
        if(motherpl_hit_cooldown > 0){//&& motherpl_vx == 0){
            //CAMERA TRAMBLE
            camera_tramble();
        }else{
            //SCROLL CAMERA
            update_camera_position();
        }
    
    //MANAGE NPC
        /*if(test_counter > 0){
            test_counter--;
            if(test_counter == 0){
                SpriteManagerAdd(SpriteHurricane, s_motherpl->x + 20u, s_motherpl->y - 8u);
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
        if(s_motherpl->x > (UINT16)30u && s_motherpl->x < ((mapwidth << 3) - 80u)){
            switch(missions[2].current_step){
                case 3u:
                    if(missions[2].current_step == 3u && enemy_counter < 2){
                        timeout_enemy--;
                        if(timeout_enemy == 200u){
                            SpriteManagerAdd(SpriteEnemyAttackerPine, (UINT16)(s_motherpl->x - 120u), (UINT16) 6u << 3);
                        }
                        if(timeout_enemy == 0u){
                            timeout_enemy = 600u;
                            SpriteManagerAdd(SpriteEnemysimplesnake, (UINT16)(s_motherpl->x - 80u), (UINT16) 6u << 3);
                        }
                    }
                break;
                case 1u:
                    if(CheckCollision(s_motherpl, s_child)){
                        missions[2].current_step = 2u;
                        whostalking = CHILD;
                        ChangeState(StateDialog, s_motherpl);
                    }
                break;
                case 0u:
                    if(s_motherpl->x > ((UINT16) 60u << 3) && s_motherpl->x < ((UINT16) 62u << 3)){
                        spawn_child_cooldown--;
                        if(spawn_child_cooldown == 0){
                            s_child = SpriteManagerAdd(SpriteChild, (UINT16)(s_motherpl->x + 24u), (UINT16) 84u);
                            missions[2].current_step = 1u;
                        }
                    }
                break;
            }            
        }
    
    Log();
}

