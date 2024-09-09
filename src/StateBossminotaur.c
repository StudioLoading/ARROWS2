#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "Dialogs.h"

IMPORT_MAP(bordercave);
IMPORT_TILES(font);
IMPORT_TILES(hudpltiles);
IMPORT_MAP(bossminotaurmap);
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
extern UINT8 npc_spawned_zone;
extern struct MISSION outwalker_glass;
extern WHOSTALKING whostalking;
extern UINT8 choice;
extern UINT16 timeout_enemy;
extern MOTHERPL_STATE motherpl_state;
extern UINT8 timeout_drop;

const UINT8 coll_tiles_mino[] = {2u, 6u, 10u, 21u, 27u, 29u, 32u, 35u, 0};
const UINT8 coll_surface_mino[] = {14u, 17u, 18u, 19u, 57u, 68u, 81u, 85u, 89u, 0};

extern UINT8 mother_exit_cooldown;

Sprite* s_minotaur;
extern struct EnemyData* minotaur_data;
extern struct MISSION golden_armor;
extern UINT8 tiles_anim_interval;
extern UINT8 timeout_drop;
extern CHAPTERS chapter;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        if(chapter == CHAPTER_4_SHIP){
            s_motherpl = SpriteManagerAdd(SpriteMotherplarmor, 4u, (UINT16) 8u << 3);
        }else{
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, 4u, (UINT16) 8u << 3);
        }
        if(previous_state == StateInventory
            || (previous_state == StateDialog && choice == 0u)) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
    //INIT CHAR & MAP
        s_minotaur = SpriteManagerAdd(SpriteBossminotaur, (UINT16)15u << 3, (UINT16)10u << 3);
        InitScroll(BANK(bossminotaurmap), &bossminotaurmap, coll_tiles_mino, coll_surface_mino); 
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 160u, (UINT16) 112u);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(bossminotaurmap), &bossminotaurmap, &mapwidth, &mapheight);
    timeout_enemy = 200u;
    mother_exit_cooldown = 60u;
    tiles_anim_interval = 0u;
    timeout_drop = 1u;
	SHOW_SPRITES;
    if(golden_armor.phase < 2){
        golden_armor.phase = 2;
    }
}

void UPDATE(){
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
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
    //CAMERA MANAGEMENT
        scroll_target->x = (UINT16) 80u;
        scroll_target->y = (UINT16) 90u;
    //FORCE MOTHERPL LIMITS
        if(s_motherpl->x < (UINT16)2u){
            s_motherpl->x = 2u;
            mother_exit_cooldown--;
            if(golden_armor.mission_state >= MISSION_STATE_ACCOMPLISHED){
               mother_exit_cooldown = 0; 
            }
            if(mother_exit_cooldown == 0u ){
                mother_exit_cooldown = 60u;
                previous_state = StateBossminotaur;
                ChangeState(StateSilvercave, s_motherpl, -1);
                //go back
            }
        }else if(mother_exit_cooldown != 60u){
            mother_exit_cooldown = 60u;
        }
        else if(s_motherpl->x > ((UINT16)19u << 3)){
            if(minotaur_data->configured == 2
                && minotaur_data->hp == 0 && minotaur_data->e_state == ENEMY_DEAD){
                //boss beated
            }else{s_motherpl->x = ((UINT16)19u << 3);}
        }
    //DROPS
        timeout_drop--;
        if(timeout_drop == 20u){
            SpriteManagerAdd(SpriteDrop, ((UINT16)3u << 3), ((UINT16) 5u << 3));
        }
        if(timeout_drop == 100u){
            SpriteManagerAdd(SpriteDrop, ((UINT16) 6u << 3), ((UINT16) 5u << 3));
        }
        if(timeout_drop == 0u){
            SpriteManagerAdd(SpriteDrop, ((UINT16) 16u << 3), ((UINT16) 3u << 3));
            timeout_drop = 110u;
        }
    
    Log(NONAME);
}