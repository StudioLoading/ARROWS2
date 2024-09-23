#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Sound.h"

#include "custom_datas.h"
//#include "Dialogs.h"


extern UINT8 J_JUMP;
extern INT8 sfx_cooldown;
extern MOTHERPL_STATE motherpl_state;

const UINT8 motherpl_anim_idle[] = {4, 1, 1, 1, 2}; //The first number indicates the number of frames
const UINT8 motherpl_anim_walk[] = {4, 3, 4, 3, 5};
const UINT8 motherpl_anim_walk_hit[] = {8, 3, 0, 4, 0, 3, 0, 5, 0};
const UINT8 motherpl_anim_jump_ascending[] = {1, 6};
const UINT8 motherpl_anim_jump_descending[] = {1, 5};
const UINT8 motherpl_anim_shoot[] = {1, 7};//7, 8, 7
const UINT8 motherpl_anim_crawlshoot[] = {3, 8, 8, 8};//9, 10, 9
const UINT8 motherpl_anim_hit[] = {2, 0, 1};
const UINT8 motherpl_anim_dead[] = {2, 0, 1};
const UINT8 motherpl_anim_crawl[] = {1, 8};
const UINT8 motherpl_anim_blocked[] = {2, 2, 1};
const UINT8 motherpl_anim_dash[] = {1, 9}; 

extern INT8 motherpl_vx;
extern INT8 motherpl_vy;
extern UINT8 motherpl_inertiax;
extern UINT8 motherpl_inertia_down;
extern UINT8 motherpl_hit;
extern UINT8 motherpl_blocked;
extern UINT8 gravity_frame_skip;
extern UINT8 motherpl_blocked_cooldown;
extern UINT8 motherpl_surfing_goton;
extern UINT8 motherpl_hit_cooldown;
extern UINT8 motherpl_dash_cooldown;
extern Sprite* s_surf;
extern struct ArrowData* surf_data;
extern INT8 motherpl_hp;

void motherplnormal_refreshAnimation() BANKED;
void motherplnormal_setanim_shoot() BANKED;
void motherplnormal_setanim_ascending() BANKED;
void motherplnormal_setanim_crawl() BANKED;
void motherplnormal_setanim_hit() BANKED;
void motherplnormal_setanim_dead() BANKED;
void motherplnormal_setanim_blocked() BANKED;
void motherplnormal_setanim_dash() BANKED;

extern void motherpl_sfx_cooldown() BANKED;
extern void motherpl_select_ticked() BANKED;
extern void motherpl_die(Sprite* s_mother) BANKED;
extern void motherpl_behave(Sprite* s_mother) BANKED;
extern void motherpl_spawnDust(Sprite* s_motherpl) BANKED;
extern void motherpl_start(Sprite* s_mother) BANKED;
extern void motherpl_changeMotherplState(Sprite* s_mother, MOTHERPL_STATE newstate) BANKED;
extern void motherpl_refresh_animation(Sprite* smother) BANKED;
extern void motherpl_jump(Sprite* s_mother) BANKED;
extern void motherpl_walk(Sprite* s_mother) BANKED;
extern void motherpl_fire(Sprite* s_mother) BANKED;
extern void motherpl_effectivevx() BANKED;
extern void motherpl_relativeposition(Sprite* s_mother) BANKED;
extern void motherpl_actualmovement(Sprite* s_mother) BANKED;
extern void motherpl_reactiontilecollision(Sprite* s_mother) BANKED;
extern void motherpl_spritecollision(Sprite* s_mother, Sprite* s_collision) BANKED;
extern void motherpl_tilescrollmonitor(Sprite* s_mother) BANKED;
extern void motherpl_getoff() BANKED;

void START(){
    motherpl_start(THIS);
}

void UPDATE(){
    //SFX_COOLDOWN
        if(sfx_cooldown > 0) {motherpl_sfx_cooldown();}
    //SELECT
        if(KEY_TICKED(J_SELECT)){
           motherpl_select_ticked();
        }
    //BEHAVIORS
        motherpl_behave(THIS);
    //BLOCK
        if(motherpl_blocked == 1u){// && motherpl_blocked_cooldown == 0u){
            motherpl_changeMotherplState(THIS, MOTHERPL_BLOCKED);
        }
    //CRAWL
        if(motherpl_state != MOTHERPL_JUMP && motherpl_state != MOTHERPL_BLOCKED
         && motherpl_state != MOTHERPL_DASH){
            if(KEY_PRESSED(J_DOWN)){
                motherpl_changeMotherplState(THIS, MOTHERPL_CRAWL);
            }
            if(KEY_RELEASED(J_DOWN) && motherpl_state == MOTHERPL_CRAWL){
                motherpl_changeMotherplState(THIS, MOTHERPL_IDLE);
            }
        }
    //HIT
        if(motherpl_hit_cooldown > 0u && motherpl_state != MOTHERPL_DEAD){
            motherpl_hit_cooldown--;
            if(motherpl_hit_cooldown == 0u){
                if(KEY_PRESSED(J_RIGHT) || KEY_PRESSED(J_LEFT)){
                    motherpl_changeMotherplState(THIS, MOTHERPL_WALK);
                }else{
                    motherpl_changeMotherplState(THIS, MOTHERPL_IDLE);
                }
            }
        }else{
            if(motherpl_hit > 0){
                motherpl_changeMotherplState(THIS, MOTHERPL_HIT);
            }
        }
    //INPUTS
    //INPUTS DASH
        if(motherpl_state != MOTHERPL_DASH){
            if(KEY_TICKED(J_JUMP) && KEY_PRESSED(J_DOWN)){
                motherpl_changeMotherplState(THIS, MOTHERPL_DASH);
                return;
            }
        }
    //INPUTS JUMP
        motherpl_jump(THIS);
    //INPUT WALK
        motherpl_walk(THIS);
    //INPUT FIRE
        motherpl_fire(THIS);
    //GRAVITY FRAME SKIP
        if(gravity_frame_skip == 0u){
            gravity_frame_skip = 1u;        
        }else{
            gravity_frame_skip = 0u;
        }
    //INERTIA ON X AXIS
        if(motherpl_inertia_down == 1u){
            if(motherpl_inertiax > 0){
                motherpl_inertiax--;
            }
        }
        if(motherpl_inertiax > 0 && !KEY_PRESSED(J_LEFT) && !KEY_PRESSED(J_RIGHT)){
            motherpl_inertiax--;
        }
    //EFFECTIVE VX
        motherpl_effectivevx();
    //RELATIVE POSITION
        motherpl_relativeposition(THIS);
    //ACTUAL MOVEMENT
        motherpl_actualmovement(THIS);
    //REACTION DI TILE COLLISION
        motherpl_reactiontilecollision(THIS);
    //SPRITE COLLISION
        UINT8 mpl_a_tile;
        Sprite* implspr;
        SPRITEMANAGER_ITERATE(mpl_a_tile, implspr) {
            if(CheckCollision(THIS, implspr)) {
                motherpl_spritecollision(THIS, implspr);           
            }
        }
    //TILE SCROLL MONITOR    
        motherpl_tilescrollmonitor(THIS);
    if(s_surf && surf_data->arrow_type == ARROW_DESTROYED && motherpl_surfing_goton == 0u){
        motherpl_getoff();
    }
}

void motherplnormal_setanim_shoot() BANKED{
    SetSpriteAnim(THIS, motherpl_anim_shoot, 4u);
    if(motherpl_state == MOTHERPL_CRAWL){SetSpriteAnim(THIS, motherpl_anim_crawlshoot, 16u);}
}

void motherplnormal_setanim_ascending() BANKED{
    SetSpriteAnim(THIS, motherpl_anim_jump_ascending, 4u);
}

void motherplnormal_setanim_crawl() BANKED{
    SetSpriteAnim(THIS, motherpl_anim_crawl, 2u);
}

void motherplnormal_setanim_hit() BANKED{
    SetSpriteAnim(THIS, motherpl_anim_hit, 64u);
}

void motherplnormal_setanim_dead() BANKED{
    SetSpriteAnim(THIS, motherpl_anim_dead, 32u);
}

void motherplnormal_setanim_blocked() BANKED{
    SetSpriteAnim(THIS, motherpl_anim_blocked, 32u);
}

void motherplnormal_setanim_dash() BANKED{
    SetSpriteAnim(THIS, motherpl_anim_dash, 12u);
}

void motherplnormal_refreshAnimation() BANKED{
    switch(motherpl_state){
        case MOTHERPL_IDLE:
            SetSpriteAnim(THIS, motherpl_anim_idle, 4u + (8 - (motherpl_hp << 1) ));
        break;
        case MOTHERPL_WALK:
            SetSpriteAnim(THIS, motherpl_anim_walk, 12u);
        break;
        case MOTHERPL_JUMP:
            if(motherpl_vy > 0){
                SetSpriteAnim(THIS, motherpl_anim_jump_descending, 4u);
            }else{
                SetSpriteAnim(THIS, motherpl_anim_jump_ascending, 4u);
            }
        break;
        case MOTHERPL_DEAD:
            SetSpriteAnim(THIS, motherpl_anim_dead, 32u);
        break;
        case MOTHERPL_CRAWL:
            SetSpriteAnim(THIS, motherpl_anim_crawl, 2u);
        break;
        case MOTHERPL_BLOCKED:
            SetSpriteAnim(THIS, motherpl_anim_blocked, 32u);
        break;
        case MOTHERPL_HIT:        
            if(motherpl_vx != 0 ){
                SetSpriteAnim(THIS, motherpl_anim_walk_hit, 24u);
            }else{
                SetSpriteAnim(THIS, motherpl_anim_hit, 24u);
            }
        break;
        case MOTHERPL_DASH:motherplnormal_setanim_dash();break;
    }
}

void DESTROY(){}