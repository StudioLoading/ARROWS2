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
IMPORT_TILES(exzootiles);
IMPORT_MAP(exzoomap0);
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

const UINT8 coll_tiles_exzoo[] = {5u, 7u, 9u, 10u, 14u, 17u, 18u, 19u, 28u, 48u, 0};
const UINT8 coll_surface_exzoo[] = {1u, 27u, 0};


extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos, NPCNAME npcname) BANKED;


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
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y); 
        InitScroll(BANK(exzoomap0), &exzoomap0, coll_tiles_exzoo, coll_surface_exzoo);    
    //HUD
        INIT_FONT(fontbw, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(exzoomap0), &exzoomap0, &mapwidth, &mapheight);
	SHOW_SPRITES;
}

void UPDATE(){
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //GO TO INVENTORY
        if(KEY_PRESSED(J_START)){ChangeState(StateInventory, s_motherpl,-1);}
    //CAMERA MANAGEMENT
        update_camera_position();
    //MANAGE NPC
        if(s_motherpl->x < ((UINT16)40u << 3)){
            if(npc_spawned_zone != 1u){
                spawn_npc(SpritePgexzoo, (UINT16) 25u << 3, 76u, WOMAN_HEAD1, WOMAN_BODY1, NO_MIRROR, EXZOO_WOMAN1, WOMAN);
                spawn_npc(SpritePgexzoo, (UINT16) 27u << 3, 76u, WOMAN_HEAD2, WOMAN_BODY2, V_MIRROR, EXZOO_WOMAN2, WOMAN);
                npc_spawned_zone = 1u;
            }
        }else if(s_motherpl->x < ((UINT16)70u << 3)){
            if(npc_spawned_zone != 2u){
                spawn_npc(SpritePgexzoo, (UINT16) 55u << 3, 76u, MAN_HEAD2, MAN_BODY2, V_MIRROR, EXZOO_MAN2, LUKE);
                npc_spawned_zone = 2u;
            }
        }else if(s_motherpl->x < ((UINT16)120u << 3)){
            if(npc_spawned_zone != 3u){
                spawn_npc(SpritePgexzoo, (UINT16) 85u << 3, 76u, MAN_HEAD1, MAN_BODY1, V_MIRROR, EXZOO_MAN1, LEGO);
                spawn_npc(SpritePgexzoo, (UINT16) 87u << 3, 76u, WOMAN_HEAD1, WOMAN_BODY3, NO_MIRROR, EXZOO_WOMAN3, WOMAN);
                npc_spawned_zone = 3u;
            }
        }
    
    Log(NONAME);
}

