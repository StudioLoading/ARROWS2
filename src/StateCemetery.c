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
extern INT8 motherpl_ups;
extern INT8 hud_motherpl_hp;
extern INT8 hud_motherpl_ups;
extern Sprite* s_motherpl;
extern UINT8 init_enemy;
extern UINT16 motherpl_pos_x;
extern UINT16 motherpl_pos_y;
extern UINT8 enemy_counter;
extern UINT8 MAX_ENEMY;
extern UINT8 mapwidth;
extern UINT8 mapheight;
const UINT8 coll_tiles_cemetery[] = {0u, 0};
const UINT8 coll_surface_cemetery[] = {1u, 16u, 0};

extern void UpdateHUD() BANKED;
extern void Log() BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
extern void ReloadEnemiesPL() BANKED;


void START(){
    LOAD_SGB_BORDER(border);
    //SOUND
        NR52_REG = 0x80; //Enables sound, you should always setup this first
        NR51_REG = 0xFF; //Enables all channels (left and right)
        NR50_REG = 0x77; //Max volume
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //SGB PALETTE
        if(sgb_check()){
            set_sgb_palette_2();
        }
    //INIT GRAPHICS
        /*if(motherpl_pos_x != 0u && motherpl_pos_y != 0u){
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, motherpl_pos_x, motherpl_pos_y);
        }else{*/
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 10u << 3, (UINT16) 6u << 3);
        //}
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x, s_motherpl->y); 
        InitScroll(BANK(cemeterymap), &cemeterymap, coll_tiles_cemetery, coll_surface_cemetery);    
    //HUD
        INIT_FONT(fontbw, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(cemeterymap), &cemeterymap, &mapwidth, &mapheight);
}

void UPDATE(){
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp || hud_motherpl_ups != motherpl_ups){
            UpdateHUD();
        }
    //GO TO INVENTORY
        if(KEY_PRESSED(J_START)){ChangeState(StateInventory, s_motherpl);}
    //SCROLL CAMERA
        update_camera_position();
    Log();
}

