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

IMPORT_MAP(bordercave);
IMPORT_TILES(fontbw);
IMPORT_TILES(blackiecavetiles);
IMPORT_TILES(hudpltiles);
IMPORT_MAP(blackiecavemap);
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
extern struct MISSION find_blackie;
extern WHOSTALKING whostalking;

const UINT8 coll_tiles_blackiecave[] = {1u, 2u, 4u, 5u, 6u, 7u, 14u, 17u, 18u, 19u, 35u, 36u, 37u, 38u, 39u, 40u, 41u, 0};
const UINT8 coll_surface_blackiecave[] = { 16u, 29u, 31u, 33u, 0};

UINT8 wolf_spawned = 0u;
UINT8 timeout_drop = 0u;
extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void camera_tramble() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
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
        s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 4u << 3, (UINT16) 20u << 3);
        if(previous_state == StateInventory || previous_state == StateDialog
            || previous_state == StateBlackieroom) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
        if(s_motherpl->x > (UINT16)62u << 3){
            s_motherpl->x = (UINT16)60u << 3;
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x, s_motherpl->y); 
        InitScroll(BANK(blackiecavemap), &blackiecavemap, coll_tiles_blackiecave, coll_surface_blackiecave);    
    //HUD
        INIT_FONT(fontbw, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(blackiecavemap), &blackiecavemap, &mapwidth, &mapheight);
    wolf_spawned = 0u;
    timeout_drop = 0u;
	SHOW_SPRITES;
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
        if(s_motherpl->x > ((UINT16)56u << 3) && find_blackie.current_step < 2){
            if(wolf_spawned == 0u){
                wolf_spawned = 1u;
                SpriteManagerAdd(SpriteWolf, (UINT16)70u << 3, (UINT16) 76u);
            }else if(wolf_spawned > 0u){
                if(s_motherpl->x > ((UINT16)62u << 3)){
                    //trigger dialog
                    whostalking = WOLF01;
                    ChangeState(StateDialog, s_motherpl);
                }
            }
        }
     //DROPS ANIM
        if(s_motherpl->x<((UINT16)50u)<<3){
            timeout_drop--;
            if(timeout_drop == 80u){
                SpriteManagerAdd(SpriteDrop, s_motherpl->x+24u, (UINT16) 44u);
            }
            if(timeout_drop == 160u){
                SpriteManagerAdd(SpriteDrop, s_motherpl->x-12u, (UINT16) 44u);
            }
            if(timeout_drop == 0u){
                SpriteManagerAdd(SpriteDrop, s_motherpl->x+64u, (UINT16) 44u);
                timeout_drop = 240u;
            }
        }

    
    Log(NONAME);
}