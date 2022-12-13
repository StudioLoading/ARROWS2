#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"
#include "Dialogs.h"

#define GRAVITY 2
#define JUMP_MIN_POWER 0
#define JUMP_MAX_POWER GRAVITY*11
#define JUMP_TICKED_COOLDOWN 16
#define INERTIA_MAX 6
#define COOLDOWN_ATTACK 24

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 motherpl_anim_idle[] = {4, 1, 1, 1, 2}; //The first number indicates the number of frames
const UINT8 motherpl_anim_walk[] = {4, 3, 4, 3, 5};
const UINT8 motherpl_anim_jump_ascending[] = {1, 6};
const UINT8 motherpl_anim_jump_descending[] = {1, 5};
const UINT8 motherpl_anim_shoot[] = {3, 7, 8, 7};

struct MotherplData* motherpl_data = 0;
INT8 motherpl_vx = 0;
INT8 motherpl_vy = 0;
MOTHERPL_STATE motherpl_state = MOTHERPL_IDLE;
UINT8 motherpl_coll = 0u;
UINT8 motherpl_jpower = 0u;
UINT8 motherpl_inertiax = 0u;
UINT8 motherpl_inertia_down = 0u;
UINT8 motherpl_rabbit = 0u;
UINT8 motherpl_hit = 0u;
UINT8 motherpl_canshoot = 0u;
UINT8 gravity_frame_skip = 0u;
UINT8 jump_frame_skip = 0u;
UINT8 jump_ticked_delay = 0u;
UINT8 jump_max_toched = 0u;
UINT8 motherpl_attack_cooldown = 0u;
UINT8 motherpl_surfing_getoff = 0u;
Sprite* s_surf = 0;

void changeMotherplState(MOTHERPL_STATE new_state);
void changeStateFromMotherpl(UINT8 new_state);
void shoot();
void refreshAnimation();

void START(){
    motherpl_vx = 0u;
    motherpl_coll = 0u;
    motherpl_jpower = 0u;
    motherpl_inertiax = 0u;
    motherpl_inertia_down = 0u;
    motherpl_rabbit = 0u;
    gravity_frame_skip = 0u;
    jump_frame_skip = 0u;
    jump_ticked_delay = 0u;
    jump_max_toched = 0u;
    motherpl_attack_cooldown = 0u;
    SetSpriteAnim(THIS, motherpl_anim_idle, 8u);
    motherpl_data = (struct MotherplData*) THIS->custom_data;
    changeMotherplState(MOTHERPL_IDLE);
    s_surf = 0;
    motherpl_surfing_getoff = 0u;
}

void UPDATE(){
    switch(motherpl_state){
        case MOTHERPL_IDLE:
            if(motherpl_attack_cooldown == 0u){
                SetSpriteAnim(THIS, motherpl_anim_idle, 8u);
            }
            motherpl_jpower = 0;
            jump_max_toched = 0u;
            motherpl_vy = GRAVITY;
            if(motherpl_vx != 0){
                changeMotherplState(MOTHERPL_WALK);
            }
        break;
        case MOTHERPL_JUMP:               
            if(motherpl_coll && motherpl_vy > 0){//IF ON SURFACE, NO MORE JUMP
                changeMotherplState(MOTHERPL_IDLE);
            }
            if(KEY_RELEASED(J_JUMP)){
                motherpl_jpower = JUMP_MIN_POWER;
                jump_max_toched = 0u;
                motherpl_vy = GRAVITY;
                jump_ticked_delay = JUMP_TICKED_COOLDOWN;
            }else if(KEY_PRESSED(J_JUMP) && jump_ticked_delay == 0){
                if(motherpl_jpower < JUMP_MAX_POWER){
                    motherpl_jpower++;
                    if(motherpl_jpower < (JUMP_MAX_POWER / 3)){
                        motherpl_vy = -3;
                    }else if(motherpl_jpower < (JUMP_MAX_POWER / 2)){
                        motherpl_vy = -2;
                    }else{
                        motherpl_vy = -1;
                    }
                }       
                if(motherpl_jpower == JUMP_MAX_POWER){
                    if(motherpl_vy < GRAVITY){
                        motherpl_vy++;
                    }
                }
            }else{
                if (motherpl_jpower > JUMP_MIN_POWER){
                    motherpl_jpower--;
                }
                if(motherpl_vy < GRAVITY){
                    motherpl_vy++;
                }
            }
            if(motherpl_vy > 0){
                SetSpriteAnim(THIS, motherpl_anim_jump_descending, 4u);
            }
        break;
        case MOTHERPL_WALK:
            if(motherpl_inertiax == 0 || (KEY_RELEASED(J_RIGHT) || KEY_RELEASED(J_LEFT))){
                changeMotherplState(MOTHERPL_IDLE);
            }
        break;
    }
    if(KEY_RELEASED(J_JUMP)){
        motherpl_rabbit = 0u;
    }
    //INPUTS
    //INPUTS JUMP
        if(jump_ticked_delay == 0 && motherpl_vy == GRAVITY && motherpl_jpower == JUMP_MIN_POWER){
            if(KEY_TICKED(J_JUMP) || KEY_PRESSED(J_JUMP)){
                if(motherpl_rabbit == 0u){
                    motherpl_rabbit = 1u;
                    changeMotherplState(MOTHERPL_JUMP);
                }
            }else if((KEY_TICKED(J_RIGHT) || KEY_TICKED(J_LEFT))){ //motherpl_state != MOTHERPL_JUMP && 
                changeMotherplState(MOTHERPL_WALK);
            }
        }
        if(jump_ticked_delay > 0){
            jump_ticked_delay--;
        }
    //INPUT WALK
        if(KEY_PRESSED(J_RIGHT) || KEY_PRESSED(J_LEFT) ){
            motherpl_inertia_down = 0u;
            if(motherpl_inertiax < INERTIA_MAX){
                motherpl_inertiax++;
            }
            if(KEY_PRESSED(J_RIGHT)){motherpl_vx = 1;THIS->mirror = NO_MIRROR;}
            else if(KEY_PRESSED(J_LEFT)){motherpl_vx = -1;THIS->mirror = V_MIRROR;}
        }
        if(KEY_RELEASED(J_RIGHT) || KEY_RELEASED(J_LEFT)){
            motherpl_inertia_down = 1u;
            motherpl_vx = 0;
            if(motherpl_state == MOTHERPL_WALK){
                changeMotherplState(MOTHERPL_IDLE);
            }
        }
    //INPUT FIRE
        if(motherpl_attack_cooldown > 0){
            motherpl_attack_cooldown--;
            if(motherpl_attack_cooldown == 0){
                refreshAnimation();
            }
        }
        if(KEY_TICKED(J_FIRE)){
            if(motherpl_attack_cooldown == 0){
                SetSpriteAnim(THIS, motherpl_anim_shoot, 16u);
                motherpl_attack_cooldown = COOLDOWN_ATTACK;
                motherpl_canshoot = 1u;
            }
        }
        if(motherpl_attack_cooldown <= (COOLDOWN_ATTACK >> 1)
            && motherpl_canshoot == 1u){
            shoot();
            motherpl_canshoot = 0u;
        }
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
    UINT8 effective_vx = motherpl_vx;
    if(motherpl_attack_cooldown > (COOLDOWN_ATTACK >> 1) && motherpl_state != MOTHERPL_JUMP){
        effective_vx = 0;
    }
    //RELATIVE POSITION
    if(motherpl_surfing_getoff > 0u){motherpl_surfing_getoff--;}
    if(s_surf && motherpl_vy >= 0 && motherpl_surfing_getoff == 0u){
        THIS->x = s_surf->x;
        THIS->y = s_surf->y - 23u;
    }
    //ACTUAL MOVEMENT
    if(motherpl_inertiax > 2){
        motherpl_coll = TranslateSprite(THIS, effective_vx << delta_time, motherpl_vy << delta_time);
    }else{
        motherpl_coll = TranslateSprite(THIS, 0, motherpl_vy << delta_time);
    }
    //REACTION DI TILE COLLISION
    switch(motherpl_coll){
        case 5u:
            if(THIS->y < ((UINT16) 8u << 3)){//DO TO TETRA
                changeStateFromMotherpl(StateTetra);
            }else{ //GO TO MAP
                changeStateFromMotherpl(StateOverworld);
            }
        break;
        case 7u:
            changeStateFromMotherpl(StateBonus);
        break;
    }

    
	UINT8 mpl_a_tile;
	Sprite* implspr;
	SPRITEMANAGER_ITERATE(mpl_a_tile, implspr) {
		if(CheckCollision(THIS, implspr)) {
			switch(implspr->type){
				case SpriteArrow:
                    if((implspr->y < THIS->y+24u) && (implspr->y > THIS->y +16u)){
                        if(s_surf == 0 && motherpl_surfing_getoff == 0u){
                            s_surf = implspr;
                        }
                    }
                break;
            }
        }
    }
}

void refreshAnimation(){
    switch(motherpl_state){
        case MOTHERPL_IDLE:
            SetSpriteAnim(THIS, motherpl_anim_idle, 8u);
        break;
        case MOTHERPL_WALK:
            SetSpriteAnim(THIS, motherpl_anim_walk, 12u);
        break;
        case MOTHERPL_JUMP:
            SetSpriteAnim(THIS, motherpl_anim_jump_ascending, 4u);
        break;
    }
}

void shoot(){
    UINT16 arrowix = THIS->x;
    if(THIS->mirror == NO_MIRROR){
        arrowix += 4u;
    }
    UINT16 arrowiy = THIS->y + 4u;
    Sprite* arrow = SpriteManagerAdd(SpriteArrow, arrowix, arrowiy);
    struct ArrowData* arrow_data = (struct ArrowData*) arrow->custom_data;
    if(THIS->mirror == NO_MIRROR){//looking right
        arrow_data->vx = 1;
    }else{
        arrow_data->vx = -1;
    }
}

void changeStateFromMotherpl(UINT8 new_state){
		SetWindowY(160);
        SetState(new_state);
}

void changeMotherplState(MOTHERPL_STATE new_state){
    if(motherpl_state != new_state){
        if(s_surf){
            s_surf = 0;
            motherpl_surfing_getoff = 62u;
        }
        switch(new_state){
            case MOTHERPL_IDLE:
                if(motherpl_attack_cooldown == 0u){
                    SetSpriteAnim(THIS, motherpl_anim_idle, 8u);
                }
                motherpl_jpower = 0;
                jump_max_toched = 0u;
                motherpl_vy = GRAVITY;
            break;
            case MOTHERPL_JUMP:
                motherpl_vy = -1;
                if(motherpl_attack_cooldown == 0u){
                    SetSpriteAnim(THIS, motherpl_anim_jump_ascending, 4u);
                }
                //jump_ticked_delay = JUMP_TICKED_COOLDOWN;
            break;
            case MOTHERPL_WALK:
                if(motherpl_attack_cooldown == 0u){
                    SetSpriteAnim(THIS, motherpl_anim_walk, 12u);
                }
                motherpl_jpower = 0;
                jump_max_toched = 0u;
            break;
        }
        motherpl_state = new_state;
    }
}

void DESTROY(){}