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
#define COOLDOWN_ATTACK 28
#define GOTON_COOLDOWN 32
#define HIT_COOLDOWN_MAX 64
#define DEAD_COOLDOWN_MAX 64
#define FLY_MAX 10

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern INT8 invcursor_posi;
extern INT8 invcursor_posimax;
extern struct InvItem* itemEquipped;
extern UINT8 camera_ok;

const UINT8 motherpl_anim_idle[] = {4, 1, 1, 1, 2}; //The first number indicates the number of frames
const UINT8 motherpl_anim_walk[] = {4, 3, 4, 3, 5};
const UINT8 motherpl_anim_jump_ascending[] = {1, 6};
const UINT8 motherpl_anim_jump_descending[] = {1, 5};
const UINT8 motherpl_anim_shoot[] = {3, 7, 7, 7};//7, 8, 7
const UINT8 motherpl_anim_crawlshoot[] = {3, 8, 8, 8};//9, 10, 9
const UINT8 motherpl_anim_hit[] = {2, 0, 1};
const UINT8 motherpl_anim_dead[] = {2, 0, 1};
const UINT8 motherpl_anim_crawl[] = {1, 8};

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
UINT8 motherpl_surfing_goton = 0u;
INT8 motherpl_surf_dx = 0;
UINT8 motherpl_hit_cooldown = 0u;
Sprite* s_surf = 0;
struct ArrowData* surf_data = 0;
INT8 motherpl_hp = 4;
INT8 motherpl_ups = 3;
UINT8 fly_counter = 0u;

void changeMotherplState(MOTHERPL_STATE new_state);
void changeStateFromMotherpl(UINT8 new_state);
void shoot();
void getOff();
void refreshAnimation();
void die();

extern void UpdateHUD() BANKED;
extern void invselectitem() BANKED;
extern void fixInvcursor() BANKED;



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
    motherpl_surfing_goton = 0u;
    motherpl_surf_dx = 0;
    motherpl_hit_cooldown = 0u;
    motherpl_hit = 0u;
}

void UPDATE(){
    if(KEY_TICKED(J_SELECT)){
        invcursor_posi++;
        fixInvcursor();
        if(invcursor_posi == 0){invcursor_posi = 1;}////Dodge Crossbow equip
        invselectitem();//STATEINVENTORY
        UpdateHUD();//STATEFITTIZIO
    }
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
                    if(fly_counter < FLY_MAX){
                        fly_counter++;
                        motherpl_vy = 0;
                    }else if(motherpl_vy < GRAVITY){
                        motherpl_vy++;
                    }
                }
            }else{
                if (motherpl_jpower > JUMP_MIN_POWER){
                    motherpl_jpower--;
                }
                /*if(fly_counter < FLY_MAX){
                        fly_counter++;
                        motherpl_vy = 0;
                }else */
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
        case MOTHERPL_HIT:
            if(motherpl_hp > 0){//changeMotherplState(MOTHERPL_IDLE);
            }
            else{changeMotherplState(MOTHERPL_DEAD);}
        break;
        case MOTHERPL_DEAD:
            if(motherpl_hit_cooldown > 0){
                motherpl_hit_cooldown--;
                if(motherpl_hit_cooldown < (DEAD_COOLDOWN_MAX >> 1)){
                    THIS->y--;
                    if(motherpl_hit_cooldown < (DEAD_COOLDOWN_MAX >> 2)){
                        refreshAnimation();
                    }
                }
            }else{//già fatto il giro verso l' alto
                die();
            }
            return;
        break;
    }
    //CRAWL
        if(motherpl_state != MOTHERPL_JUMP){
            if(KEY_PRESSED(J_DOWN)){
                changeMotherplState(MOTHERPL_CRAWL);
            }
            if(KEY_RELEASED(J_DOWN) && motherpl_state == MOTHERPL_CRAWL){
                changeMotherplState(MOTHERPL_IDLE);
            }
        }
    //HIT
        if(motherpl_hit_cooldown > 0u){
            motherpl_hit_cooldown--;
            if(motherpl_hit_cooldown == 0u){
                motherpl_hit = 0u;
                changeMotherplState(MOTHERPL_IDLE);
            }
        }
        if(motherpl_hit == 1u){
            changeMotherplState(MOTHERPL_HIT);
        }
    //INPUTS
    //INPUTS JUMP
        if(KEY_RELEASED(J_JUMP)){
            motherpl_rabbit = 0u;
        }
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
        if(motherpl_state != MOTHERPL_CRAWL){
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
                camera_ok = 0u;
                if(motherpl_state == MOTHERPL_WALK){
                    changeMotherplState(MOTHERPL_IDLE);
                }
            }
        }
    //INPUT FIRE
        if(motherpl_attack_cooldown > 0){
            motherpl_attack_cooldown--;
            if(motherpl_attack_cooldown == 0){
                refreshAnimation();
            }
        }
        if(KEY_TICKED(J_FIRE) || KEY_PRESSED(J_FIRE)){
            if(motherpl_attack_cooldown == 0){
                SetSpriteAnim(THIS, motherpl_anim_shoot, 16u);
                if(motherpl_state == MOTHERPL_CRAWL){SetSpriteAnim(THIS, motherpl_anim_crawlshoot, 16u);}
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
    //EFFECTIVE VX
        UINT8 effective_vx = motherpl_vx;
        if(motherpl_attack_cooldown > (COOLDOWN_ATTACK >> 1) && motherpl_state != MOTHERPL_JUMP){
            effective_vx = 0;
        }
    //RELATIVE POSITION
        if(motherpl_surfing_goton == GOTON_COOLDOWN && motherpl_state != MOTHERPL_IDLE){//just got on a ride
            changeMotherplState(MOTHERPL_IDLE);
        }
        if(motherpl_surfing_goton > 0u){motherpl_surfing_goton--;}
        if(motherpl_surfing_getoff > 0u){motherpl_surfing_getoff--;}
        if(s_surf && motherpl_vy >= 0 && motherpl_surfing_getoff == 0u){
            THIS->x = s_surf->x + motherpl_surf_dx;
            THIS->y = s_surf->y - 23u;
        }
    //ACTUAL MOVEMENT
        UINT8 t_vertical_coll = TranslateSprite(THIS, 0, motherpl_vy << delta_time);
        if(t_vertical_coll && motherpl_state == MOTHERPL_JUMP){
            changeMotherplState(MOTHERPL_IDLE);
        }
        if(motherpl_inertiax > 2){
            motherpl_coll = TranslateSprite(THIS, effective_vx << delta_time, 0);
        }
    //REACTION DI TILE COLLISION
    switch(current_state){
        case StateExzoo:
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
        break;
        case StateCemetery:
        case StateBlackiecave:
            switch(motherpl_coll){
                case 8u:
                    changeStateFromMotherpl(StateOverworld);
                break;
            }
        break;
    }
    //SPRITE COLLISION
	UINT8 mpl_a_tile;
	Sprite* implspr;
	SPRITEMANAGER_ITERATE(mpl_a_tile, implspr) {
		if(CheckCollision(THIS, implspr)) {
			switch(implspr->type){
				case SpriteArrow:
                    if((implspr->y < THIS->y+24u) && (implspr->y > THIS->y +16u)){
                        if(s_surf == 0 && motherpl_surfing_getoff == 0u){
                            changeMotherplState(MOTHERPL_IDLE);
                            motherpl_surfing_goton = GOTON_COOLDOWN;
                            s_surf = implspr;
                            surf_data =(struct ArrowData*) s_surf->custom_data;
                            motherpl_surf_dx = THIS->x - implspr->x;
                        }
                    }
                break;
                case SpriteEnemysimple:
                    if(motherpl_hit != 1u){motherpl_hit = 1u;}
                break;
            }
        }
    }
    if(s_surf && surf_data->arrow_type == ARROW_DESTROYED && motherpl_surfing_goton == 0u){
        getOff();
    }
}

void getOff(){
    surf_data = 0;
    s_surf = 0;
    motherpl_surfing_getoff = 32u;
}

void die(){
    motherpl_hp = 4;
    motherpl_ups--;
    if(motherpl_ups < 0){SetState(StateCredit);}
    else{SetState(StateExzoo);}
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
        case MOTHERPL_DEAD:
            SetSpriteAnim(THIS, motherpl_anim_dead, 32u);
        break;
        case MOTHERPL_CRAWL:
            SetSpriteAnim(THIS, motherpl_anim_crawl, 2u);
        break;
    }
}

void shoot(){
    if(itemEquipped->quantity == 0){return;}
    switch(itemEquipped->itemtype){
        case INVITEM_ARROW_NORMAL:
        case INVITEM_ARROW_PERFO:
        case INVITEM_ARROW_BASTARD:
            itemEquipped->quantity--;
            UpdateHUD();
            UINT16 arrowix = THIS->x;
            if(THIS->mirror == NO_MIRROR){
                arrowix += 4u;
            }
            UINT16 arrowiy = THIS->y + 6u;
            if(motherpl_state == MOTHERPL_JUMP)arrowiy = THIS->y + 4u;
            if(motherpl_state == MOTHERPL_CRAWL)arrowiy = THIS->y + 13u;
            Sprite* arrow = SpriteManagerAdd(SpriteArrow, arrowix, arrowiy);
            struct ArrowData* arrow_data = (struct ArrowData*) arrow->custom_data;
            switch(itemEquipped->itemtype){
                case INVITEM_ARROW_NORMAL:
                    arrow_data->arrow_type = ARROW_NORMAL;
                    arrow_data->arrow_fskipx_max = 0;
                case INVITEM_ARROW_PERFO:
                case INVITEM_ARROW_BASTARD:
                break;
            }
            if(THIS->mirror == NO_MIRROR){//looking right
                arrow_data->vx = 2;
            }else{
                arrow_data->vx = -2;
            }
            arrow_data->vy = 0;
            if(KEY_PRESSED(J_UP)){
                arrow_data->vy = -2;
            }
        break;
    }
    refreshAnimation();
}

void changeStateFromMotherpl(UINT8 new_state){
		SetWindowY(160);
        SetState(new_state);
}

void changeMotherplState(MOTHERPL_STATE new_state){
    if(motherpl_state != new_state){
        if(s_surf && motherpl_surfing_goton == 0u){
            getOff();
        }
        if(new_state == MOTHERPL_HIT && motherpl_hit_cooldown > 0u){
            return;
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
                fly_counter = 0;
                motherpl_vy = -1;
                if(motherpl_attack_cooldown == 0u){
                    SetSpriteAnim(THIS, motherpl_anim_jump_ascending, 4u);
                }
            break;
            case MOTHERPL_CRAWL:
                if(motherpl_attack_cooldown == 0u){
                    SetSpriteAnim(THIS, motherpl_anim_crawl, 2u);
                }
                motherpl_jpower = 0;
                jump_max_toched = 0u;
                motherpl_vy = GRAVITY;
                motherpl_vx = 0;
            break;
            case MOTHERPL_WALK:
                if(motherpl_attack_cooldown == 0u){
                    SetSpriteAnim(THIS, motherpl_anim_walk, 12u);
                }
                motherpl_jpower = 0;
                jump_max_toched = 0u;
            break;
            case MOTHERPL_HIT:
                motherpl_hp--;
                motherpl_hit_cooldown = HIT_COOLDOWN_MAX;
                if(motherpl_hp > 0){
                    SetSpriteAnim(THIS, motherpl_anim_hit, 32u);
                }else{
                    changeMotherplState(MOTHERPL_DEAD);
                    return;
                }
            break;
            case MOTHERPL_DEAD:
                SetSpriteAnim(THIS, motherpl_anim_dead, 32u);
                motherpl_hit_cooldown = DEAD_COOLDOWN_MAX;                
            break;
        }
        motherpl_state = new_state;
    }
}

void DESTROY(){}