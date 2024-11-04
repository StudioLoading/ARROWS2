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
#include "TAnim0.h"
#include "Dialogs.h"

#define MAX_CLOUD_TIMEOUT 200

IMPORT_TILES(font);
IMPORT_TILES(cemeterytiles);
IMPORT_TILES(hudpltiles);
IMPORT_MAP(cemeterymap);
IMPORT_MAP(hudpl);


extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
extern INT8 motherpl_hp;
extern INT8 hud_motherpl_hp;
extern Sprite* s_motherpl;
extern UINT8 init_enemy;
extern UINT8 mapwidth;
extern UINT8 mapheight;
extern UINT8 previous_state;
extern UINT16 motherpl_pos_x;
extern UINT16 motherpl_pos_y;
extern MirroMode motherpl_mirror; 
extern UINT8 npc_spawned_zone;
extern struct MISSION engage_smith;
extern struct MISSION help_cemetery_woman;
extern struct MISSION outwalker_smith;
extern CHAPTERS chapter;

const UINT8 coll_tiles_cemetery[] = {0u, 0};
const UINT8 coll_surface_cemetery[] = {1u, 16u, 0};
UINT8 cloud_timeout = 0;
UINT8 configured_loop = 0;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos, NPCNAME npcname) BANKED;
extern void spawn_npa(UINT8 type, UINT16 posx, UINT16 posy, UINT8 configured) BANKED;


void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        if(chapter == CHAPTER_4_SHIP){
            s_motherpl = SpriteManagerAdd(SpriteMotherplarmor, (UINT16) 4u << 3, (UINT16) 10u << 3);
        }else{
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 4u << 3, (UINT16) 10u << 3);
        }
        if(previous_state == StateInv || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
        if(outwalker_smith.mission_state == MISSION_STATE_STARTED){
            UINT8 spawn_flower_4 = outwalker_smith.current_step & 0b00001000;
            if(spawn_flower_4 == 0){
                Sprite* s_flower4 = SpriteManagerAdd(SpriteFlower, (UINT16)31u << 3, (UINT16)12u << 3);
                struct ItemSpawned* flower4_data = (struct ItemSpawned*) s_flower4->custom_data;
                flower4_data->hp = 0b00001000;
            }
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x, s_motherpl->y); 
        InitScroll(BANK(cemeterymap), &cemeterymap, coll_tiles_cemetery, coll_surface_cemetery);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(cemeterymap), &cemeterymap, &mapwidth, &mapheight);
        cloud_timeout = MAX_CLOUD_TIMEOUT;
        configured_loop = 0;
	SHOW_SPRITES;
}

void UPDATE(){
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //GO TO INVENTORY
        if(KEY_PRESSED(J_START)){ChangeState(StateInv, s_motherpl, -1);}
    //CAMERA MANAGEMENT
        update_camera_position();
    //MANAGE NPC
        if(s_motherpl->x < ((UINT16)35u << 3)){
            if(npc_spawned_zone != 1u){
                spawn_npc(SpritePgceme, (UINT16) 18u << 3, 80u, WOMAN_HEAD1, WOMAN_BODY2, V_MIRROR, CEMETERY_WOMAN2, WOMAN);
                if(help_cemetery_woman.mission_state == MISSION_STATE_ENABLED){
                    spawn_npc(SpritePgceme, (UINT16) 27u << 3, 81u, WOMAN_HEAD1, MAN_BODY1, V_MIRROR, CRYING_MOTHER, MARGARET);
                }else{
                    spawn_npc(SpritePgceme, (UINT16) 28u << 3, 76u, WOMAN_HEAD1, WOMAN_BODY1, NO_MIRROR, CEMETERY_WOMAN1, WOMAN);
                }
                spawn_npa(SpriteBirdsky, (UINT16) 8 << 3, (UINT16) 6u << 3, 1); 
                spawn_npa(SpriteBirdsky, ((UINT16) 8 << 3) - 4u, ((UINT16) 6u << 3) -2u, 1);  
                spawn_npa(SpriteBirdsky, (UINT16) 7u << 3, (UINT16) 7u << 3, 2); 
                npc_spawned_zone = 1u;
            }
        }else if(s_motherpl->x < ((UINT16)60u << 3)){
            if(npc_spawned_zone != 2u){
                if(engage_smith.current_step == 0){
                    spawn_npc(SpritePgceme, (UINT16) 45u << 3, 80u, MAN_HEAD1, MAN_BODY1, V_MIRROR, WHOST_SHOP_SMITH, JOHN);
                }
                spawn_npa(SpriteBirdsky, (UINT16) 40 << 3, (UINT16) 5u << 3, 1); 
                spawn_npa(SpriteBirdsky, ((UINT16) 41 << 3) - 4u, ((UINT16) 6u << 3) -2u, 1);  
                spawn_npa(SpriteBirdsky, (UINT16) 44u << 3, (UINT16) 5u << 3, 2); 
                spawn_npc(SpritePgceme, (UINT16) 52u << 3, 68u, WOMAN_HEAD1, WOMAN_BODY2, NO_MIRROR, CEMETERY_WOMAN2, WOMAN);
                npc_spawned_zone = 2u;
            }
        }
        cloud_timeout--;
        if(cloud_timeout == 0){
            cloud_timeout = MAX_CLOUD_TIMEOUT;
            if(s_motherpl->x < ((UINT16) 8u << 3)){
            }
            configured_loop++;
            if(configured_loop == 5){
                configured_loop = 1;
            }
        } 
    Log(NONAME);
}

