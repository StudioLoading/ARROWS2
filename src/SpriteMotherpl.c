#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"
//#include "Dialogs.h"

#define GRAVITY 2
#define JUMP_MIN_POWER 0
#define JUMP_MAX_POWER GRAVITY*11
#define JUMP_TICKED_COOLDOWN 24
#define INERTIA_MAX 16
#define COOLDOWN_ATTACK 28
#define GOTON_COOLDOWN 32
#define HIT_COOLDOWN_MAX 64
#define DEAD_COOLDOWN_MAX 64
#define BLOCKED_COOLDOWN_MAX 200
#define FLY_MAX 10
#define PICKINGUP_COOLDOWN 16
#define DASH_COOLDOWN_MAX 24

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern INT8 invcursor_posi;
extern INT8 invcursor_posimax;
extern struct InvItem itemEquipped;
extern UINT8 camera_ok;
extern WHOSTALKING whostalking;

const UINT8 motherpl_anim_idle[] = {4, 1, 1, 1, 2}; //The first number indicates the number of frames
const UINT8 motherpl_anim_walk[] = {4, 3, 4, 3, 5};
const UINT8 motherpl_anim_walk_hit[] = {8, 3, 0, 4, 0, 3, 0, 5, 0};
const UINT8 motherpl_anim_jump_ascending[] = {1, 6};
const UINT8 motherpl_anim_jump_descending[] = {1, 5};
const UINT8 motherpl_anim_shoot[] = {3, 7, 7, 7};//7, 8, 7
const UINT8 motherpl_anim_crawlshoot[] = {3, 8, 8, 8};//9, 10, 9
const UINT8 motherpl_anim_hit[] = {2, 0, 1};
const UINT8 motherpl_anim_dead[] = {2, 0, 1};
const UINT8 motherpl_anim_crawl[] = {1, 8};
const UINT8 motherpl_anim_blocked[] = {2, 2, 1};
const UINT8 motherpl_anim_dash[] = {1, 9}; 

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
UINT8 motherpl_blocked = 0u;
UINT8 motherpl_canshoot = 0u;
UINT8 gravity_frame_skip = 0u;
UINT8 jump_frame_skip = 0u;
UINT8 jump_ticked_delay = 0u;
UINT8 jump_max_toched = 0u;
UINT8 motherpl_attack_cooldown = 0u;
UINT8 motherpl_blocked_cooldown = 0u;
UINT8 motherpl_surfing_getoff = 0u;
UINT8 motherpl_surfing_goton = 0u;
INT8 motherpl_surf_dx = 0;
UINT8 motherpl_hit_cooldown = 0u;
UINT8 motherpl_dash_cooldown = DASH_COOLDOWN_MAX;
Sprite* s_surf = 0;
Sprite* s_blocking = 0;
Sprite* s_blockcmd = 0;
struct ArrowData* surf_data = 0;
INT8 motherpl_hp = 4;
INT8 motherpl_ups = 3;
UINT8 fly_counter = 0u;
INT8 pickingup_cooldown = PICKINGUP_COOLDOWN;

void changeMotherplState(MOTHERPL_STATE new_state);
void shoot();
void getOff();
void refreshAnimation();
void die();
void spawnDust();

extern void UpdateHUD() BANKED;
extern void invselectitem(INT8 max_idx) BANKED;
extern void fixInvcursor(INT8 max_idx) BANKED;
extern void pickup(struct ItemSpawned* pickedup_data) BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
extern void spawn_item(INVITEMTYPE itemtype, UINT16 x, UINT16 y) BANKED;

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
    SetSpriteAnim(THIS, motherpl_anim_idle, 4u + (12 - (motherpl_hp << 1)));
    motherpl_data = (struct MotherplData*) THIS->custom_data;
    changeMotherplState(MOTHERPL_IDLE);
    s_surf = 0;
    s_blocking = 0;
    motherpl_surfing_getoff = 0u;
    motherpl_surfing_goton = 0u;
    motherpl_surf_dx = 0;
    motherpl_hit_cooldown = 0u;
    motherpl_hit = 0u;
    motherpl_blocked = 0u;
    motherpl_blocked_cooldown = 0u;
    pickingup_cooldown = PICKINGUP_COOLDOWN;
}

void UPDATE(){
    //SELECT
        if(KEY_TICKED(J_SELECT)){
            invcursor_posi++;
            //fixInvcursor(5);
            //if(invcursor_posi == 0){invcursor_posi = 1;}////Dodge Crossbow equip
            invselectitem(5);//STATEINVENTORY
            UpdateHUD();//STATEFITTIZIO
        }
    //BEHAVIORS
        switch(motherpl_state){
            case MOTHERPL_IDLE:
                if(motherpl_attack_cooldown == 0u){
                    SetSpriteAnim(THIS, motherpl_anim_idle, 4u + (8 - (motherpl_hp << 1)));
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
                refreshAnimation();
                if(motherpl_inertiax == 0){//|| (KEY_RELEASED(J_RIGHT) || KEY_RELEASED(J_LEFT))){
                    changeMotherplState(MOTHERPL_IDLE);
                }
            break;
            case MOTHERPL_HIT:
                motherpl_vy = GRAVITY;
                refreshAnimation();
                if(motherpl_hp <= 0){
                    changeMotherplState(MOTHERPL_DEAD);
                }
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
            case MOTHERPL_BLOCKED:
                if(KEY_TICKED(J_LEFT) || KEY_TICKED(J_RIGHT)){
                    motherpl_blocked_cooldown -= 40;
                }
                //motherpl_blocked_cooldown--;
                if(motherpl_blocked_cooldown > BLOCKED_COOLDOWN_MAX){
                    SpriteManagerRemoveSprite(s_blocking);
                    motherpl_blocked = 0u;
                    motherpl_blocked_cooldown = 16u;
                    changeMotherplState(MOTHERPL_IDLE);
                }
            break;
            case MOTHERPL_PICKUP:
                pickingup_cooldown--;
                if(pickingup_cooldown <= 0){
                    pickingup_cooldown = PICKINGUP_COOLDOWN;
                    changeMotherplState(MOTHERPL_IDLE);
                }
                return;
            break;
            case MOTHERPL_CRAWL:
                if(KEY_RELEASED(J_DOWN)){
                    camera_ok = 0u;
                }
            break;
            case MOTHERPL_CRAWL_SURF:
                if(s_surf){
                    THIS->x = s_surf->x + motherpl_surf_dx;
                    THIS->y = s_surf->y - 20u;
                    //THIS->x = s_surf->x;
                    //THIS->y = s_surf->y -16u;
                }
            break;
            case MOTHERPL_DASH:
                if(motherpl_dash_cooldown % 8 == 0){
                    spawnDust();
                }
                motherpl_dash_cooldown--;
                if(motherpl_dash_cooldown == 0){
                    motherpl_dash_cooldown = DASH_COOLDOWN_MAX;
                    changeMotherplState(MOTHERPL_IDLE);
                }
                {
                    UINT8 dash_floor = GetScrollTile((THIS->x >> 3) + 1u, (THIS->y >> 3) + 2u);
                    if(dash_floor == 48u){
                            spawn_item(INVITEM_POWDER, THIS->x, THIS->y);
                    }
                }
            break;
        }
    //BLOCK
        if(motherpl_blocked == 1u){// && motherpl_blocked_cooldown == 0u){
            changeMotherplState(MOTHERPL_BLOCKED);
        }
    //CRAWL
        if(motherpl_state != MOTHERPL_JUMP && motherpl_state != MOTHERPL_BLOCKED
         && motherpl_state != MOTHERPL_DASH){
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
        }else{
            if(motherpl_hit == 1u){
                changeMotherplState(MOTHERPL_HIT);
            }
        }
    //INPUTS
    //INPUTS DASH
        if(motherpl_state != MOTHERPL_DASH){
            if(KEY_TICKED(J_JUMP) && KEY_PRESSED(J_DOWN)){
                changeMotherplState(MOTHERPL_DASH);
                return;
            }
        }
    //INPUTS JUMP
        if((motherpl_state != MOTHERPL_BLOCKED && motherpl_state != MOTHERPL_HIT
            && motherpl_state != MOTHERPL_DASH
            ) || 
            (motherpl_state != MOTHERPL_DASH && motherpl_coll != 0u)
            ){
            if(KEY_RELEASED(J_JUMP)){
                motherpl_rabbit = 0u;
            }
            if(jump_ticked_delay == 0 && motherpl_vy == GRAVITY 
                && motherpl_jpower == JUMP_MIN_POWER){
                    //  || motherpl_coll != 0u
                //&& motherpl_state != MOTHERPL_JUMP
                if(KEY_TICKED(J_JUMP) || KEY_PRESSED(J_JUMP)){
                    if(motherpl_rabbit == 0u){
                        motherpl_rabbit = 1u;
                        changeMotherplState(MOTHERPL_JUMP);
                    }
                }else if(KEY_TICKED(J_RIGHT) || KEY_TICKED(J_LEFT)){ //motherpl_state != MOTHERPL_JUMP && 
                    changeMotherplState(MOTHERPL_WALK);
                }
            }
        }
        if(jump_ticked_delay > 0){
            jump_ticked_delay--;
        }
    //INPUT WALK
        if(motherpl_state != MOTHERPL_CRAWL && motherpl_state != MOTHERPL_BLOCKED 
            && motherpl_state != MOTHERPL_PICKUP && motherpl_state != MOTHERPL_DASH ){
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
                camera_ok = 0u;
            }
        }
    //INPUT FIRE
        if(motherpl_attack_cooldown > 0){
            motherpl_attack_cooldown--;
            if(motherpl_attack_cooldown == 0){
                refreshAnimation();
            }
        }
        if(motherpl_state != MOTHERPL_BLOCKED){
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
        //UINT8 effective_vx = motherpl_vx;
        if(motherpl_attack_cooldown > (COOLDOWN_ATTACK >> 1) 
            && motherpl_state != MOTHERPL_JUMP && motherpl_state != MOTHERPL_DASH){
            motherpl_vx = 0; 
            //effective_vx = 0;
        }
    //RELATIVE POSITION
        if(motherpl_surfing_goton == GOTON_COOLDOWN && motherpl_state != MOTHERPL_IDLE){//just got on a ride
            changeMotherplState(MOTHERPL_CRAWL_SURF);
        }
        if(motherpl_surfing_goton > 0u){motherpl_surfing_goton--;}
        if(motherpl_surfing_getoff > 0u){motherpl_surfing_getoff--;}
        if(s_surf && motherpl_vy >= 0 && motherpl_surfing_getoff == 0u){
            //THIS->x = s_surf->x + motherpl_surf_dx;
            //THIS->y = s_surf->y - 23u;
        }
    //ACTUAL MOVEMENT
        if(motherpl_state != MOTHERPL_CRAWL_SURF){      
            UINT8 t_vertical_coll = TranslateSprite(THIS, 0, motherpl_vy << delta_time);
            if(t_vertical_coll && motherpl_state == MOTHERPL_JUMP){
                //spawnDust();
                changeMotherplState(MOTHERPL_IDLE);
            }        
            if(motherpl_inertiax > 2 || motherpl_state == MOTHERPL_DASH){
                motherpl_coll = TranslateSprite(THIS, motherpl_vx << delta_time, 0);
            }
        }
    //REACTION DI TILE COLLISION
        switch(current_state){
            case StateExzoo:
                switch(motherpl_coll){
                    case 5u:
                        if(THIS->y < ((UINT16) 8u << 3)){//DO TO TETRA
                            ChangeState(StateTetra, motherpl_state);
                        }else{ //GO TO MAP
                            ChangeState(StateOverworld, motherpl_state);
                        }
                    break;
                    case 7u:
                        ChangeState(StateBonus, motherpl_state);
                    break;
                }
            break;
            case StateMine:
                switch(motherpl_coll){
                    case 11u:
                        if(motherpl_state == MOTHERPL_DASH){
                            if(THIS->mirror == NO_MIRROR){
                                THIS->x++;
                            }else{
                                THIS->x--;
                            }
                            motherpl_dash_cooldown++;
                        }
                    break;
                }
            break;
            case StateCemetery:
            break;
            case StateBlackiecave:
                switch(motherpl_coll){
                    case 34u://tiles di soffitto che quando dash non voglio incastri
                    case 35u:
                    case 36u:
                    case 37u:
                        if(motherpl_state == MOTHERPL_DASH){
                            if(THIS->mirror == NO_MIRROR){
                                THIS->x++;
                            }else{
                                THIS->x--;
                            }
                            motherpl_dash_cooldown++;
                        }
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
                    case SpritePgceme:
                    case SpritePgexzoo:
                        {
                            struct NpcInfo* npc_data = (struct NpcInfo*) implspr->custom_data;
                            whostalking = npc_data->whotalks;
                        }
                        if(KEY_RELEASED(J_FIRE)){
                            ChangeState(StateDialog, THIS);                        
                        }
                    break;
                    case SpriteArrow:
                        motherpl_blocked = 0u;
                        if(motherpl_vy > 0){
                            if((implspr->y < THIS->y+24u) && (implspr->y > THIS->y +16u)){
                                if(s_surf == 0 && motherpl_surfing_getoff == 0u){
                                    //changeMotherplState(MOTHERPL_CRAWL);
                                    changeMotherplState(MOTHERPL_CRAWL_SURF);
                                    motherpl_surfing_goton = GOTON_COOLDOWN;
                                    s_surf = implspr;
                                    surf_data =(struct ArrowData*) s_surf->custom_data;
                                    motherpl_surf_dx = THIS->x - implspr->x;
                                }
                            }
                        }
                    break;
                    case SpriteEnemysimplesnake:
                    case SpriteEnemysimplerat:
                    case SpriteEnemyAttackerCobra:
                    case SpriteEnemyAttackerPine:
                    case SpriteEnemyThrowerSpider:
                    case SpriteEnemyThrowerTarantula:
                        {
                            motherpl_blocked = 0u;
                            struct EnemyData* e_data = (struct EnemyData*) implspr->custom_data;
                            if(e_data->e_state != ENEMY_DEAD && 
                                e_data->e_state != ENEMY_HIT){
                                if(motherpl_state == MOTHERPL_DASH){
                                    if(e_data->e_state == ENEMY_ATTACK){
                                        motherpl_hit = 1u;
                                    }else if(motherpl_coll == 0u){
                                        motherpl_dash_cooldown++;
                                    }
                                }else if(e_data->e_state != ENEMY_UPSIDEDOWN 
                                        && motherpl_hit != 1u){
                                    motherpl_hit = 1u;
                                }
                            }
                        }
                    break;
                    case SpriteEnemythrowable:
                        {
                            struct ThrowableData* throwable_data = (struct ThrowableData*) s_blocking->custom_data;
                            if(throwable_data->type == WEB && motherpl_blocked == 0u 
                                && motherpl_state != MOTHERPL_DASH){
                                if(motherpl_blocked_cooldown == 0u){
                                    if(throwable_data->type == T_DESTROYED){
                                        s_blocking = implspr;
                                        changeMotherplState(MOTHERPL_BLOCKED);
                                    }
                                }
                            }
                            if(throwable_data->type == ACID){
                                motherpl_blocked = 0u;
                                if(motherpl_hit != 1u){motherpl_hit = 1u;}
                            }
                        }
                    break;
                    case SpriteItemspawned:
                        {
                            if(motherpl_state != MOTHERPL_DASH){
                                struct ItemSpawned* pickedup_data = (struct ItemSpawned*) implspr->custom_data;
                                pickup(pickedup_data);
                                SpriteManagerRemoveSprite(implspr);
                                if(motherpl_state != MOTHERPL_HIT 
                                    && motherpl_state != MOTHERPL_JUMP){
                                    changeMotherplState(MOTHERPL_PICKUP);
                                }
                            }
                        }
                    break;
                    case SpriteSuperstone:
                        if(THIS->mirror == NO_MIRROR){
                            THIS->x--;
                        }else{
                            THIS->x++;
                        }
                        motherpl_vx = 0u;
                    break;
                }            
            }
        }
    //TILE SCROLL MONITOR    
        UINT8 scroll_tile = 0u;
        UINT8 scroll_tile_overlapping = GetScrollTile((THIS->x >> 3) + 1u, (THIS->y >> 3) + 1u);
        if(THIS->mirror == NO_MIRROR){
            scroll_tile = GetScrollTile((THIS->x >> 3) + 2u, (THIS->y >> 3) + 1u);
        }else{
            scroll_tile = GetScrollTile((THIS->x >> 3) - 1u, (THIS->y >> 3) + 1u);
        }
        switch(current_state){
            case StateMine:
                if(KEY_PRESSED(J_FIRE)){
                    if(scroll_tile == 78u || scroll_tile == 79u 
                        || scroll_tile_overlapping == 78u 
                        || scroll_tile_overlapping == 79u){
                        whostalking = SUPERSTONE;
                        ChangeState(StateDialog, THIS);
                    }
                }
            break;
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
    if(motherpl_ups < 0){ChangeState(StateCredit, motherpl_state);}
    else{ChangeState(current_state, motherpl_state);}
}

void refreshAnimation(){
    switch(motherpl_state){
        case MOTHERPL_IDLE:
            SetSpriteAnim(THIS, motherpl_anim_idle, 4u + (8 - (motherpl_hp << 1) ));
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
    }
}

void spawnDust(){   
    UINT16 dust_x = (UINT16) THIS->x - 8u;
    if(motherpl_state == MOTHERPL_DASH){
        dust_x += 28u;
        if(THIS->mirror == V_MIRROR){
            dust_x -= 58u;
        }
    } 
    Sprite* s_dust = SpriteManagerAdd(SpriteDust, dust_x, (UINT16) THIS->y + 4u);
    if(THIS->mirror == V_MIRROR){
        s_dust->x += 16u;
        s_dust->mirror = V_MIRROR;
    }
}

void shoot(){
    if(motherpl_blocked > 0){return;}
    if(itemEquipped.quantity == 0){return;}
    switch(itemEquipped.itemtype){
        case INVITEM_ARROW_NORMAL:
        case INVITEM_ARROW_PERFO:
        case INVITEM_ARROW_BASTARD:
            itemEquipped.quantity--;
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
            switch(itemEquipped.itemtype){
                case INVITEM_ARROW_NORMAL:
                    arrow_data->arrow_type = ARROW_NORMAL;
                break;
                case INVITEM_ARROW_PERFO:
                    arrow_data->arrow_type = ARROW_PERF;
                break;
                case INVITEM_ARROW_BASTARD:
                    arrow_data->arrow_type = ARROW_BASTARD;
                break;
            }
            INT8 arrow_vx = 2;
            if(itemEquipped.itemtype == INVITEM_ARROW_NORMAL){
                arrow_vx = 1;
            }
            if(THIS->mirror == NO_MIRROR){//looking right
                arrow_data->vx = arrow_vx;
            }else{
                arrow_data->vx = -arrow_vx;
            }
            arrow_data->configured = 1u;
        break;
    }
    refreshAnimation();
}

void changeMotherplState(MOTHERPL_STATE new_state){
    if(motherpl_state != new_state){
        if(s_surf && motherpl_surfing_goton == 0u){
            getOff();
        }
        if(new_state != MOTHERPL_BLOCKED){
            if(s_blockcmd){
                SpriteManagerRemoveSprite(s_blockcmd);
            }
        }
        switch(new_state){
            case MOTHERPL_IDLE:
                motherpl_vx = 0;
                 if(motherpl_state == MOTHERPL_WALK){
                    spawnDust();
                }
                if(motherpl_attack_cooldown == 0u){
                    SetSpriteAnim(THIS, motherpl_anim_idle, 4u + (8 - (motherpl_hp << 1) ));
                }
                motherpl_jpower = 0;
                jump_max_toched = 0u;
                motherpl_vy = GRAVITY;
            break;
            case MOTHERPL_JUMP:
                motherpl_vy = -1;
                fly_counter = 0;                
                //jump_ticked_delay = JUMP_TICKED_COOLDOWN;
                if(motherpl_attack_cooldown == 0u){
                    SetSpriteAnim(THIS, motherpl_anim_jump_ascending, 4u);
                }
            break;
            case MOTHERPL_CRAWL:
            case MOTHERPL_CRAWL_SURF:
                if(motherpl_attack_cooldown == 0u){
                    SetSpriteAnim(THIS, motherpl_anim_crawl, 2u);
                }
                motherpl_jpower = 0;
                jump_max_toched = 0u;
                motherpl_vy = GRAVITY;
                motherpl_vx = 0;
                motherpl_inertiax = 0u;
                camera_ok = 0u;
            break;
            case MOTHERPL_WALK:
                if(motherpl_attack_cooldown == 0u){
                    refreshAnimation();
                }
                motherpl_jpower = 0;
                jump_max_toched = 0u;
            break;
            case MOTHERPL_HIT:
                motherpl_hit_cooldown = HIT_COOLDOWN_MAX;
                motherpl_hp--;
                if(s_blocking){SpriteManagerRemoveSprite(s_blocking);}
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
            case MOTHERPL_BLOCKED:
                motherpl_vx = 0;
                motherpl_blocked = 2u;
                SetSpriteAnim(THIS, motherpl_anim_blocked, 32u);
                motherpl_blocked_cooldown = BLOCKED_COOLDOWN_MAX;
                s_blockcmd = SpriteManagerAdd(SpriteRightleft, THIS->x - 2u, THIS->y - 20u);
            break;
            case MOTHERPL_PICKUP:
                motherpl_inertiax = 0u;
                pickingup_cooldown = PICKINGUP_COOLDOWN;
                SetSpriteAnim(THIS, motherpl_anim_crawl, 2u);
            break;            
            case MOTHERPL_DASH:
                SetSpriteAnim(THIS, motherpl_anim_dash, 12u);
                if(THIS->mirror == NO_MIRROR){
                    motherpl_vx = 2;
                }else{
                    motherpl_vx = -2;
                }
            break;
        }
        motherpl_state = new_state;
    }
}

void DESTROY(){}