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
#include "TAnim0.h"
#include "Dialogs.h"

IMPORT_MAP(bordercrab);
IMPORT_TILES(font);
IMPORT_TILES(hudpltiles);
IMPORT_MAP(bossscorpionmap);
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
extern struct MISSION outwalker_glass;
extern WHOSTALKING whostalking;
extern UINT8 choice;
extern UINT16 timeout_enemy;
extern MOTHERPL_STATE motherpl_state;
extern UINT8 timeout_drop;
extern CHAPTERS chapter;

const UINT8 coll_tiles_scorpio[] = {1u, 2u, 4u, 5u, 6u, 7u, 14u, 17u, 18u, 19u, 35u, 36u, 37u, 38u, 39u, 40u, 41u, 0};
const UINT8 coll_surface_scorpio[] = { 16u, 29u, 31u, 33u, 50u, 0};

extern UINT8 mother_exit_cooldown;

extern Sprite* scorpio_head;
extern Sprite* scorpio_body;
extern struct EnemyData* scorpiobody_data;
extern struct EnemyData* scorpiohead_data;
extern struct MISSION find_antidote;

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
        if(chapter == CHAPTER_4_SHIP){
            s_motherpl = SpriteManagerAdd(SpriteMotherplarmor, 10u, 46u);
        }else{
            s_motherpl = SpriteManagerAdd(SpriteMotherpl, 10u, 46u);
        }
        if(previous_state == StateInv
            || (previous_state == StateDialog && choice == 0u)) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
    //INIT CHAR & MAP
        if(find_antidote.phase < 2){
            UINT16 scorpio_x = (UINT16)14u << 3;
            UINT16 scorpio_y = (UINT16)6u << 3; 
            scorpio_body = SpriteManagerAdd(SpriteBossscorpion, scorpio_x, scorpio_y);
            scorpiobody_data = (struct EnemyData*)scorpio_body->custom_data;
            scorpiobody_data->wait = 80;
            scorpiobody_data->vx = -1;
            scorpiobody_data->x_frameskip = 2;
            scorpiobody_data->type = SCORPIO_BODY;
            scorpiobody_data->configured = 1;
            scorpio_head = SpriteManagerAdd(SpriteBossscorpionhead, scorpio_body->x-24u, scorpio_body->y);
            scorpiohead_data = (struct EnemyData*) scorpio_head->custom_data;
            scorpiohead_data->type = SCORPIO_HEAD;
            scorpiohead_data->hp = 6;
            scorpiobody_data->hp = scorpiohead_data->hp;
            scorpiohead_data->configured = 1;
        }
        InitScroll(BANK(bossscorpionmap), &bossscorpionmap, coll_tiles_scorpio, coll_surface_scorpio); 
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 88u, (UINT16) 120u);        
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(bossscorpionmap), &bossscorpionmap, &mapwidth, &mapheight);
    timeout_enemy = 200u;
    mother_exit_cooldown = 60u;
    timeout_drop = 0u;
	SHOW_SPRITES;
}

void UPDATE(){
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //CAMERA MANAGEMENT
        scroll_target->x = (UINT16) 80u;
        scroll_target->y = (UINT16) 56u;
    //FORCE MOTHERPL LIMITS
        if(s_motherpl->x < (UINT16)8u){
            s_motherpl->x = 8u;
            mother_exit_cooldown--;
            if(outwalker_glass.mission_state >= MISSION_STATE_ACCOMPLISHED){
               mother_exit_cooldown = 0; 
            }
            if(mother_exit_cooldown == 0u ){
                mother_exit_cooldown = 60u;
                previous_state = StateBossscorpion;
                ChangeState(StateOw, s_motherpl, 3);
                //go back
            }
        }else if(mother_exit_cooldown != 60u){
            mother_exit_cooldown = 60u;
        }
        if(s_motherpl->x > ((UINT16)19u << 3)){
            s_motherpl->x = ((UINT16)19u << 3);
        }
    //DROPS
        /*
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
        */
    
    Log(NONAME);
}