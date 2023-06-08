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
IMPORT_TILES(cemeterytiles);
IMPORT_TILES(hudpltiles);
IMPORT_MAP(cemeterymap);
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
extern struct MISSION missions[4];

const UINT8 coll_tiles_cemetery[] = {0u, 0};
const UINT8 coll_surface_cemetery[] = {1u, 16u, 0};

extern void UpdateHUD() BANKED;
extern void Log() BANKED;
extern void update_camera_position() BANKED;
extern void camera_tramble() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos) BANKED;


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
        s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 4u << 3, (UINT16) 10u << 3);
        if(previous_state == StateInventory || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x, s_motherpl->y); 
        InitScroll(BANK(cemeterymap), &cemeterymap, coll_tiles_cemetery, coll_surface_cemetery);    
    //HUD
        INIT_FONT(fontbw, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(cemeterymap), &cemeterymap, &mapwidth, &mapheight);
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
        if(s_motherpl->x < ((UINT16)35u << 3)){
            if(npc_spawned_zone != 1u){
                spawn_npc(SpritePgceme, (UINT16) 18u << 3, 80u, WOMAN_HEAD1, WOMAN_BODY2, V_MIRROR, CEMETERY_WOMAN2);
                if(missions[2].mission_state == MISSION_STATE_ENABLED){
                    spawn_npc(SpritePgceme, (UINT16) 27u << 3, 81u, WOMAN_HEAD1, MAN_BODY1, V_MIRROR, CRYING_MOTHER);
                }else{
                    spawn_npc(SpritePgceme, (UINT16) 28u << 3, 76u, WOMAN_HEAD1, WOMAN_BODY1, NO_MIRROR, CEMETERY_WOMAN1);
                }
                npc_spawned_zone = 1u;
            }
        }else if(s_motherpl->x < ((UINT16)60u << 3)){
            if(npc_spawned_zone != 2u){
                if(missions[1].mission_state == MISSION_STATE_DISABLED){
                    spawn_npc(SpritePgceme, (UINT16) 45u << 3, 80u, MAN_HEAD1, MAN_BODY1, V_MIRROR, SMITH);
                }
                spawn_npc(SpritePgceme, (UINT16) 52u << 3, 68u, WOMAN_HEAD1, WOMAN_BODY2, NO_MIRROR, CEMETERY_WOMAN2);
                
                npc_spawned_zone = 2u;
            }
        }
    
    Log();
}

