#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "BankManager.h"
#include "Palette.h"
#include "Sound.h"
#include "Music.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "Dialogs.h"
#include "Dialogs2.h"

#define JUMP_TICKED_COOLDOWN 24
#define JUMP_MIN_POWER 0
#define GRAVITY 2
#define FLY_MAX 10
#define JUMP_MAX_POWER_DMG GRAVITY*9
#define JUMP_MAX_POWER_CGB GRAVITY*11
#define JUMP_TICKED_COOLDOWN 24
#define INERTIA_MAX 16
#define COOLDOWN_ATTACK 28
#define GOTON_COOLDOWN 32
#define HIT_COOLDOWN_MAX 64
#define DEAD_COOLDOWN_MAX 64
#define BLOCKED_COOLDOWN_MAX 70
#define PICKINGUP_COOLDOWN 16
#define DASH_COOLDOWN_MAX 24
#define HP_INITIAL 3


MOTHERPL_STATE motherpl_state = MOTHERPL_IDLE;
UINT8 motherpl_attack_cooldown = 0u;
UINT8 motherpl_jpower = 0u;
UINT8 jump_max_toched = 0u;
INT8 motherpl_vy = 0;
INT8 motherpl_vx = 0;
UINT8 jump_ticked_delay = 0u;
UINT8 motherpl_coll_y = 0u;
UINT8 fly_counter = 0u;
UINT8 motherpl_inertiax = 0u;
INT8 motherpl_hp = HP_INITIAL;
UINT8 motherpl_hit_cooldown = 0u;
UINT8 motherpl_blocked_cooldown = 0u;
Sprite* s_blocking = 0;
INT8 pickingup_cooldown = PICKINGUP_COOLDOWN;
Sprite* s_surf = 0;
INT8 motherpl_surf_dx = 0;
UINT8 motherpl_rabbit = 0u;
UINT8 dash_horizontal_time = 0u;
UINT8 motherpl_dash_cooldown = DASH_COOLDOWN_MAX;
UINT8 gravity_frame_skip = 0u;
UINT8 motherpl_coll_x = 0u;
UINT8 motherpl_inertia_down = 0u;
UINT8 motherpl_surfing_getoff = 0u;
UINT8 motherpl_surfing_goton = 0u;
UINT8 motherpl_hit = 0u;
UINT8 motherpl_blocked = 0u;
UINT8 motherpl_canshoot = 0u;
Sprite* s_blockcmd = 0;
struct MotherplData* motherpl_data = 0;
UINT8 jump_max_power = 0u;
struct ArrowData* surf_data = 0;

extern INT8 sfx_cooldown;
extern INT8 invcursor_posi;
extern UINT8 camera_ok;
extern UINT8 powder_cooldown;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern WHOSTALKING whostalking;
extern UINT8 child_hooked;
extern struct EnemyData* bossbat_data;
extern struct InvItem itemEquipped;
extern struct MISSION outwalker_chief;
extern struct MISSION outwalker_glass;
extern struct MISSION outwalker_smith;
extern struct MISSION enable_hospital;
extern struct MISSION get_to_the_mountain;
extern struct MISSION find_antidote;
extern struct MISSION golden_armor;
extern struct MISSION help_cemetery_woman;
extern struct MISSION mr_smee;

extern void invselectitem(INT8 max_idx) BANKED;
extern void bat_change_state(Sprite* s_bat, ENEMY_STATE e_state) BANKED;
extern void spawnItem(INVITEMTYPE itemtype, UINT16 x, UINT16 y) BANKED;
extern void UpdateHUD() BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern void spawn_dialog_icon(Sprite* npc) BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;
extern void fixInvcursor(INT8 max_idx) BANKED;
extern void pickup(struct ItemSpawned* pickedup_data) BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;
extern void play_music_reward() BANKED;
extern void changeEstate(Sprite* s_enemy, ENEMY_STATE new_e_state) BANKED;

extern void motherplnormal_refreshAnimation() BANKED;
extern void motherplnormal_setanim_shoot() BANKED;
extern void motherplnormal_setanim_ascending() BANKED;
extern void motherplnormal_setanim_crawl() BANKED;
extern void motherplnormal_setanim_hit() BANKED;
extern void motherplnormal_setanim_dead() BANKED;
extern void motherplnormal_setanim_blocked() BANKED;
extern void motherplnormal_setanim_dash() BANKED;

extern void motherplarmor_refreshAnimation() BANKED;
extern void motherplarmor_setanim_shoot() BANKED;
extern void motherplarmor_setanim_ascending() BANKED;
extern void motherplarmor_setanim_crawl() BANKED;
extern void motherplarmor_setanim_hit() BANKED;
extern void motherplarmor_setanim_dead() BANKED;
extern void motherplarmor_setanim_blocked() BANKED;
extern void motherplarmor_setanim_dash() BANKED;

void motherpl_sfx_cooldown() BANKED;
void motherpl_select_ticked() BANKED;
void motherpl_refresh_animation(Sprite* s_mother) BANKED;
void motherpl_changeMotherplState(Sprite* s_mother, MOTHERPL_STATE new_state) BANKED;
void motherpl_die(Sprite* s_mother) BANKED;
void motherpl_spawnDust(Sprite* s_motherpl) BANKED;
void motherpl_shoot(Sprite* s_mother) BANKED;
void motherpl_getoff() BANKED;
void motherpl_hitted(Sprite* s_enemy) BANKED;
void motherpl_ckautodialog(Sprite* s_mother, NPCNAME npcname) BANKED;
void motherpl_effectivevx(Sprite* s_mother) BANKED;
void motherpl_behave(Sprite* s_mother) BANKED;

void START(){

}

void motherpl_start(Sprite* s_mother) BANKED{
    switch(s_mother->type){
        case SpriteMotherpl:
        case SpriteMotherplarmor:
            motherpl_vx = 0u;
            motherpl_coll_x = 0u;
            motherpl_coll_y = 0u;
            motherpl_jpower = 0u;
            motherpl_inertiax = 0u;
            motherpl_inertia_down = 0u;
            motherpl_rabbit = 0u;
            gravity_frame_skip = 0u;
            dash_horizontal_time = 0u;
            jump_ticked_delay = 0u;
            jump_max_toched = 0u;
            motherpl_attack_cooldown = COOLDOWN_ATTACK;
            motherpl_state = MOTHERPL_IDLE;
            motherpl_refresh_animation(s_mother);
            motherpl_data = (struct MotherplData*) s_mother->custom_data;
            motherpl_changeMotherplState(s_mother, MOTHERPL_IDLE);
            s_surf = 0;
            s_blocking = 0;
            motherpl_surfing_getoff = 0u;
            motherpl_surfing_goton = 0u;
            motherpl_surf_dx = 0;
            motherpl_hit_cooldown = 0u;
            motherpl_hit = 0u;
            motherpl_blocked = 0u;
            motherpl_blocked_cooldown = 0u;
            motherpl_canshoot = 1u;
            pickingup_cooldown = PICKINGUP_COOLDOWN;
            jump_max_power = JUMP_MAX_POWER_CGB;
            if(_cpu != CGB_TYPE){
                jump_max_power = JUMP_MAX_POWER_DMG;
                OBP1_REG = PAL_DEF(0, 0, 1, 3);
                SPRITE_SET_PALETTE(s_mother,1);
            }
        break;
    }
}

void motherpl_sfx_cooldown() BANKED{
    sfx_cooldown--;
}

void motherpl_select_ticked() BANKED{
    invcursor_posi++;
    //fixInvcursor(5);
    invselectitem(5);//STATEINVENTORY
    UpdateHUD();//STATEFITTIZIO
}

void motherpl_refresh_animation(Sprite* s_mother) BANKED{
    switch(s_mother->type){
        case SpriteMotherpl:
            motherplnormal_refreshAnimation();
        break;
        case SpriteMotherplarmor:
            motherplarmor_refreshAnimation();
        break;
    }
}

void motherpl_die(Sprite* s_mother) BANKED{
    trigger_dialog(DEATH, s_mother);
}

void motherpl_spawnDust(Sprite* s_motherpl) BANKED{
    UINT16 dust_x = (UINT16) s_motherpl->x - 8u;
    if(motherpl_state == MOTHERPL_DASH){
        dust_x += 28u;
        if(s_motherpl->mirror == V_MIRROR){
            dust_x -= 58u;
        }
    } 
    Sprite* s_dust = SpriteManagerAdd(SpriteDust, dust_x, (UINT16) s_motherpl->y + 4u);
    if(s_motherpl->mirror == V_MIRROR){
        s_dust->x += 16u;
        s_dust->mirror = V_MIRROR;
    }
}

void motherpl_shoot(Sprite* s_mother) BANKED{
    if(motherpl_blocked > 0){return;}
    if(itemEquipped.quantity == 0){return;}
    switch(itemEquipped.itemtype){
        case INVITEM_ARROW_NORMAL:
        case INVITEM_ARROW_PERFO:
        case INVITEM_ARROW_BASTARD:
            if(current_state != StateTutorial){
                itemEquipped.quantity--;
                UpdateHUD();
            }
            UINT16 arrowix = s_mother->x - 2u;
            if(s_mother->mirror == NO_MIRROR){
                arrowix += 8u;
            }
            UINT16 arrowiy = s_mother->y + 6u;
            if(motherpl_state == MOTHERPL_JUMP)arrowiy = s_mother->y + 4u;
            if(motherpl_state == MOTHERPL_CRAWL)arrowiy = s_mother->y + 13u;
            Sprite* arrow = SpriteManagerAdd(SpriteArrow, arrowix, arrowiy);
            struct ArrowData* arrow_data = (struct ArrowData*) arrow->custom_data;
            arrow_data->hit = 0u;
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
            if(s_mother->mirror == NO_MIRROR){//looking right
                arrow_data->vx = arrow_vx;
            }else{
                arrow_data->vx = -arrow_vx;
            }
            arrow_data->configured = 1u;
            if(current_state != StateTutorial){
                change_quantity(itemEquipped.itemtype, -1);
            }
        break;
    }
    motherpl_attack_cooldown = COOLDOWN_ATTACK;
    motherpl_canshoot = 0u;
    if(s_mother->type == SpriteMotherplarmor){
        motherplarmor_setanim_shoot();
    }else{ motherplnormal_setanim_shoot();}
    //refreshAnimation();
}

void motherpl_walk(Sprite* s_mother) BANKED{
    if(motherpl_state != MOTHERPL_CRAWL && motherpl_state != MOTHERPL_BLOCKED 
        && motherpl_state != MOTHERPL_PICKUP && motherpl_state != MOTHERPL_DASH ){
        if(KEY_PRESSED(J_RIGHT) || KEY_PRESSED(J_LEFT) ){
            motherpl_inertia_down = 0u;
            if(motherpl_inertiax < INERTIA_MAX){
                motherpl_inertiax++;
            }
            if(KEY_PRESSED(J_RIGHT)){motherpl_vx = 1;s_mother->mirror = NO_MIRROR;}
            else if(KEY_PRESSED(J_LEFT)){motherpl_vx = -1;s_mother->mirror = V_MIRROR;}
        }
        if(KEY_RELEASED(J_RIGHT) || KEY_RELEASED(J_LEFT)){
            motherpl_inertia_down = 1u;
            camera_ok = 0u;
        }
    }
}

void motherpl_hitted(Sprite* s_enemy) BANKED{//just raise the motherpl_hit flag to 1
    if(motherpl_hit > 0 || motherpl_hit_cooldown > 0){return;}
    motherpl_blocked = 0u;
    struct EnemyData* e_data = (struct EnemyData*) s_enemy->custom_data;
    if(e_data->hp > 0){
        if(s_enemy->type == SpriteBossscorpion 
            || s_enemy->type == SpriteBosscrab
            || s_enemy->type == SpriteBossminotaur
            || s_enemy->type == SpriteEnemyBat){
            motherpl_hit = 2u;
        }else if(s_enemy->type == SpriteBossbat){
            motherpl_hit = 1u;
        }else if(motherpl_state == MOTHERPL_DASH){
            if(e_data->e_state == ENEMY_ATTACK || s_enemy->type == SpriteSeagull || s_enemy->type == SpriteEnemyThrowerScorpion){
                motherpl_hit = 1u;
            }else if(motherpl_coll_x == 0u){
                motherpl_dash_cooldown++;
            }
        }else{
            motherpl_hit = 1u;
        }
    }
}

void motherpl_ckautodialog(Sprite* s_mother, NPCNAME npcname) BANKED{
    switch(npcname){
        case OUTWALKER_SIMON:
            if(outwalker_chief.mission_state < MISSION_STATE_ACCOMPLISHED){
                s_mother->x -= 12u;
                trigger_dialog(OUTWALKER_GUARD_NOCHIEF_NOGLASS, s_mother);
            }else if(outwalker_glass.mission_state < MISSION_STATE_ACCOMPLISHED){
                s_mother->x -= 12u;
                trigger_dialog(OUTWALKER_GUARD_NOGLASS, s_mother);
            }else if(outwalker_smith.mission_state < MISSION_STATE_ACCOMPLISHED){
                s_mother->x -= 12u;
                trigger_dialog(OUTWALKER_GUARD_NOSMITH, s_mother);
            }else if(get_to_the_mountain.mission_state == MISSION_STATE_DISABLED){
                outwalker_smith.mission_state = MISSION_STATE_REWARDED;
                get_to_the_mountain.mission_state = MISSION_STATE_ENABLED;
                SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
                s_mother->x -= 12u;
                trigger_dialog(OUTWALKER_GUARD_OK, s_mother);
                change_quantity(INVITEM_BOX, -1);
            }else if(get_to_the_mountain.mission_state >= MISSION_STATE_ACCOMPLISHED){
                //landslide!
                s_mother->x -= 12u;
                trigger_dialog(OUTWALKER_GUARD_LANDSLIDE, s_mother);
            }
        break;
        case SMEE:
            if(mr_smee.mission_state == MISSION_STATE_DISABLED){
                trigger_dialog(PIRATE_SPUGNA_0, s_mother);
            }else if(mr_smee.mission_state == MISSION_STATE_ENABLED){
                trigger_dialog(PIRATE_SPUGNA_1, s_mother);
            }
        break;
        case RICK:
            if(mr_smee.mission_state == MISSION_STATE_STARTED){
                trigger_dialog(CADAVER, s_mother);
            }
        break;
    }
}

void motherpl_getoff() BANKED{
    surf_data = 0;
    s_surf = 0;
    motherpl_surfing_getoff = 32u;
}

void motherpl_jump(Sprite* s_mother) BANKED{
    if(KEY_RELEASED(J_JUMP)){
        motherpl_rabbit = 0u;
    }
    if((motherpl_state != MOTHERPL_BLOCKED 
        && motherpl_state != MOTHERPL_HIT
        && motherpl_state != MOTHERPL_DASH
        ) || 
        (motherpl_state != MOTHERPL_DASH && motherpl_coll_x != 0u)
        ){
        if(motherpl_coll_y && jump_ticked_delay == 0 && motherpl_vy == GRAVITY 
            && motherpl_jpower == JUMP_MIN_POWER
            && motherpl_rabbit == 0u){
                //  || motherpl_coll != 0u
            //&& motherpl_state != MOTHERPL_JUMP
            if(KEY_TICKED(J_JUMP) || KEY_PRESSED(J_JUMP)){
                if(motherpl_rabbit == 0u){
                    motherpl_rabbit = 1u;
                    motherpl_changeMotherplState(s_mother, MOTHERPL_JUMP);
                }
            }else if(motherpl_state != MOTHERPL_JUMP && motherpl_state != MOTHERPL_WALK && (KEY_TICKED(J_RIGHT) || KEY_TICKED(J_LEFT))){
                motherpl_changeMotherplState(s_mother, MOTHERPL_WALK);
            }
        }
    }
    if(jump_ticked_delay > 0){
        jump_ticked_delay--;
    }
}

void motherpl_fire(Sprite* s_mother) BANKED{
    if(motherpl_attack_cooldown > 0){
        motherpl_attack_cooldown--;
        if(motherpl_attack_cooldown == 0){
            motherpl_canshoot = 1;
            motherpl_refresh_animation(s_mother);
        }
    }
    if(motherpl_state != MOTHERPL_BLOCKED && itemEquipped.itemtype != INVITEM_MONEY){
        if(KEY_TICKED(J_FIRE) || KEY_PRESSED(J_FIRE)){
            if(motherpl_attack_cooldown == 0 && motherpl_canshoot == 1){
                motherpl_shoot(s_mother);
            }
        }
    }
}

void motherpl_effectivevx(Sprite* s_mother) BANKED{
    if(motherpl_attack_cooldown > (COOLDOWN_ATTACK >> 1) 
        && motherpl_state != MOTHERPL_JUMP && motherpl_state != MOTHERPL_DASH){
        motherpl_vx = 0; 
    }
}

void motherpl_relativeposition(Sprite* s_mother) BANKED{
    if(motherpl_surfing_goton == GOTON_COOLDOWN && motherpl_state != MOTHERPL_IDLE){//just got on a ride
        motherpl_changeMotherplState(s_mother, MOTHERPL_CRAWL_SURF);
    }
    if(motherpl_surfing_goton > 0u){motherpl_surfing_goton--;}
    if(motherpl_surfing_getoff > 0u){motherpl_surfing_getoff--;}
}

void motherpl_actualmovement(Sprite* s_mother) BANKED{
    if(motherpl_state != MOTHERPL_CRAWL_SURF && motherpl_state != MOTHERPL_BLOCKED){      
        if(dash_horizontal_time == 0){
            motherpl_coll_y = TranslateSprite(s_mother, 0, motherpl_vy << delta_time);
        }
        if(motherpl_inertiax > 2 || motherpl_state == MOTHERPL_DASH){
            motherpl_coll_x = TranslateSprite(s_mother, motherpl_vx << delta_time, 0);
        }
    }
}

void motherpl_reactiontilecollision(Sprite* s_mother) BANKED{
     switch(motherpl_coll_x){
        case 14u:
        case 17u:
        case 18u:
        case 19u:
            if(motherpl_state == MOTHERPL_DASH){
                if(s_mother->mirror == NO_MIRROR){
                    s_mother->x++;
                }else{
                    s_mother->x--;
                }
                motherpl_dash_cooldown++;
            }
        break;
    }
    switch(current_state){
        case StateExzoo:
            switch(motherpl_coll_x){
                case 5u:
                    if(s_mother->y < ((UINT16) 8u << 3)){//DO TO TETRA
                        ChangeState(StateTetra, s_mother, -1);
                    }else{ //GO TO MAP
                        ChangeState(StateOverworld, s_mother, -1);
                    }
                break;
                case 7u:
                    //ChangeState(StateBonus, s_mother, -1);
                break;
            }
        break;
        case StateMine:
            switch(motherpl_coll_x){
                case 90u:
                case 91u:
                    if(motherpl_state == MOTHERPL_DASH){
                        if(s_mother->mirror == NO_MIRROR){
                            s_mother->x++;
                        }else{
                            s_mother->x--;
                        }
                        motherpl_dash_cooldown++;
                    }
                break;
            }
        break;
        case StateCemetery:
        break;
        case StateBlackiecave:
        case StateBatcave:
        case StateBossbat:
            switch(motherpl_coll_x){
                case 34u://tiles di soffitto che quando dash non voglio incastri
                case 35u:
                case 36u:
                case 37u:
                    if(motherpl_state == MOTHERPL_DASH){
                        if(s_mother->mirror == NO_MIRROR){
                            s_mother->x++;
                        }else{
                            s_mother->x--;
                        }
                        motherpl_dash_cooldown++;
                    }
                break;
                case 54u: //GRADINI IN SALITA A SINISTRA
                    if(motherpl_state == MOTHERPL_WALK && KEY_PRESSED(J_LEFT)){
                        s_mother->y -= 8;
                        TranslateSprite(s_mother, -3, 0);
                    }
                break;
                case 55u: //GRADINI IN SALITA A DESTRA
                    if(motherpl_state == MOTHERPL_WALK && KEY_PRESSED(J_RIGHT)){
                        s_mother->y -= 8;
                        TranslateSprite(s_mother, 3, 0);
                    }
                break;
            }
        break;
    }
}

void motherpl_spritecollision(Sprite* s_mother, Sprite* s_collision) BANKED{
    switch(s_collision->type){
        case SpriteBlackie:
            motherpl_canshoot = 0u;
            {
                struct NpcInfo* npc_data = (struct NpcInfo*) s_collision->custom_data;
                Log(npc_data->npcname);
                if(KEY_RELEASED(J_FIRE)){
                    trigger_dialog(npc_data->whotalks, s_mother);
                }
            }
        break;
        case SpritePgCadaver:
        case SpritePgoutwalker:
        case SpritePgceme:
        case SpritePgexzoo:
        case SpritePgPirate:
            motherpl_canshoot = 0u;
            {
                struct NpcInfo* npc_data = (struct NpcInfo*) s_collision->custom_data;
                //whostalking = ;
                Log(npc_data->npcname);
                motherpl_ckautodialog(s_mother, npc_data->npcname);
                spawn_dialog_icon(s_collision);
                if(KEY_RELEASED(J_FIRE)){
                    trigger_dialog(npc_data->whotalks, s_mother);
                }
            }
        case SpriteBlackiechild:
            if(find_antidote.mission_state == MISSION_STATE_ACCOMPLISHED
            && find_antidote.phase == 5){
                trigger_dialog(BLACKIE_DEAD_CHILD, s_mother);
            }
        break;
        break;
        case SpriteArrow:
            motherpl_blocked = 0u;
            if(motherpl_vy > 0){
                if((s_collision->y < s_mother->y+24u) && (s_collision->y > s_mother->y +16u)){
                    if(s_surf == 0 && motherpl_surfing_getoff == 0u){
                        //changeMotherplState(MOTHERPL_CRAWL);
                        motherpl_changeMotherplState(s_mother, MOTHERPL_CRAWL_SURF);
                        motherpl_surfing_goton = GOTON_COOLDOWN;
                        s_surf = s_collision;
                        surf_data =(struct ArrowData*) s_surf->custom_data;
                        motherpl_surf_dx = s_mother->x - s_collision->x;
                    }
                }
            }
        break;
        case SpriteEnemysimplesnake:
        case SpriteEnemysimplerat:
            if(s_mother->type == SpriteMotherplarmor){
                changeEstate(s_collision, ENEMY_HIT_1);
            }
        case SpriteEnemyAttackerCobra:
        case SpriteEnemyAttackerPine:
        case SpriteEnemyThrowerSpider:
        case SpriteEnemyThrowerScorpion:
        case SpriteBigstone:
        case SpriteBolt:
        case SpriteSeagull:
        case SpriteBosscrab:
        case SpriteBossscorpion:
            motherpl_hitted(s_collision);
        break;
        case SpriteBossbat:
            {
            if(bossbat_data->e_state == ENEMY_HIT_1){return;}
            if(motherpl_state == MOTHERPL_DASH){
                if(bossbat_data->e_state == ENEMY_SLIDE_DOWN){
                    if(motherpl_dash_cooldown == 0){motherpl_dash_cooldown = 1;}
                    bossbat_data->configured = 1;
                }else{
                    motherpl_hitted(s_collision);
                }
            }else{motherpl_hitted(s_collision);}
            }
        break;
        case SpriteBossminotaur:
            if(golden_armor.phase >= 3){
                motherpl_canshoot = 0u;
                if(KEY_TICKED(J_FIRE)){
                    trigger_dialog(MINOTAUR_DEFEAT, s_mother);
                    golden_armor.phase = 4;
                }
            }
        break;
        case SpriteItemspawned:
            {
                if(motherpl_state != MOTHERPL_DASH){
                    struct ItemSpawned* pickedup_data = (struct ItemSpawned*) s_collision->custom_data;
                    pickup(pickedup_data);
                    SpriteManagerRemoveSprite(s_collision);
                    if(pickedup_data->itemtype == INVITEM_SILVER){
                        enable_hospital.current_step = 2;
                    }
                    if(motherpl_state != MOTHERPL_HIT 
                        && motherpl_state != MOTHERPL_JUMP){
                        motherpl_changeMotherplState(s_mother, MOTHERPL_PICKUP);
                    }
                }
            }
        break;
        case SpriteSuperstone:
            if(s_mother->mirror == NO_MIRROR){
                s_mother->x--;
            }else{
                s_mother->x++;
            }
            motherpl_vx = 0u;
        break;
        case SpriteGlasses:
            {
            struct ItemSpawned* pickedup_data = (struct ItemSpawned*) s_collision->custom_data;
            pickup(pickedup_data);
            play_music_reward();
            SpriteManagerRemoveSprite(s_collision);
            outwalker_glass.mission_state = MISSION_STATE_ACCOMPLISHED;
            outwalker_glass.current_step = 2;
            SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
            }
        break;
        case SpriteFlower:
            {
            struct ItemSpawned* pickedup_data = (struct ItemSpawned*) s_collision->custom_data;
            UINT8 flower_hp = pickedup_data->hp;
            outwalker_smith.current_step = outwalker_smith.current_step | flower_hp;
            pickup(pickedup_data);
            SpriteManagerRemoveSprite(s_collision);
            }
        break;
        case SpriteLeaf:
            if(motherpl_state == MOTHERPL_DASH){
                SpriteManagerRemoveSprite(s_collision);
            }
        break;
        case SpriteScorpiontail:
            {
            struct ItemSpawned* pickedup_data = (struct ItemSpawned*) s_collision->custom_data;
            pickup(pickedup_data);
            play_music_reward();
            SpriteManagerRemoveSprite(s_collision);
            find_antidote.phase = 2;
            find_antidote.current_step = 0;
            SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
            }
        break;
        case SpriteSkull:
            {
            struct EnemyData* skull_data = (struct EnemyData*) s_collision->custom_data;
            if(skull_data->vx == 0){
                struct ItemSpawned pickedup_gold_data = {.itemtype = INVITEM_MONEY, .quantity = 30u, .equippable = 1u};
                pickup(&pickedup_gold_data);
                struct ItemSpawned pickedup_silver_data = {.itemtype = INVITEM_SILVERSKULL, .quantity = 1, .equippable = 0u};
                pickup(&pickedup_silver_data);
                play_music_reward();
                SpriteManagerRemoveSprite(s_collision);
                golden_armor.mission_state = MISSION_STATE_ACCOMPLISHED;
                golden_armor.phase = 3;
                golden_armor.current_step = 0;
                SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
            }
            }
        break;
    } 
}

void motherpl_tilescrollmonitor(Sprite* s_mother) BANKED{
    UINT8 scroll_tile = 0u;
    UINT8 scroll_tile_overlapping = GetScrollTile((s_mother->x >> 3) + 1u, (s_mother->y >> 3) + 1u);
    if(s_mother->mirror == NO_MIRROR){
        scroll_tile = GetScrollTile((s_mother->x >> 3) + 2u, (s_mother->y >> 3) + 1u);
    }else{
        scroll_tile = GetScrollTile((s_mother->x >> 3) - 1u, (s_mother->y >> 3) + 1u);
    }
    switch(current_state){
        case StateMine:
            if(KEY_PRESSED(J_FIRE)){
                if(scroll_tile == 78u || scroll_tile == 79u 
                    || scroll_tile_overlapping == 78u 
                    || scroll_tile_overlapping == 79u){
                    trigger_dialog(SUPERSTONE, s_mother);
                }
            }
        break;
        case StateBlackiecave:
            if(KEY_PRESSED(J_FIRE)){
                if(scroll_tile == 18u){
                    trigger_dialog(BOUNCINGBLOCK, s_mother);
                }
            }
        break;
    }
}

void motherpl_changeMotherplState(Sprite* s_mother, MOTHERPL_STATE new_state) BANKED{
    if(motherpl_state != new_state){
        if(s_surf && motherpl_surfing_goton == 0u){
            motherpl_getoff();
        }
        if(new_state != MOTHERPL_BLOCKED){
            if(s_blockcmd){
                SpriteManagerRemoveSprite(s_blockcmd);
            }
        }
        if(new_state != MOTHERPL_DASH){dash_horizontal_time = 0u;}
        switch(new_state){
            case MOTHERPL_IDLE:
                motherpl_vx = 0;
                 if(motherpl_state == MOTHERPL_WALK){
                    motherpl_spawnDust(s_mother);
                }
                if(motherpl_attack_cooldown == 0u){
                    motherpl_refresh_animation(s_mother);
                    //SetSpriteAnim(s_mother, motherpl_anim_idle, 4u + (8 - (motherpl_hp << 1) ));
                }
                motherpl_jpower = 0;
                jump_max_toched = 0u;
                motherpl_vy = GRAVITY;
            break;
            case MOTHERPL_JUMP:
                motherpl_vy = -1;
                fly_counter = 0;
                //my_play_fx(CHANNEL_1, 60, 0x76, 0x7a, 0xe1, 0x5a, 0x86);//SFX_JUMP
                //jump_ticked_delay = JUMP_TICKED_COOLDOWN;
                if(motherpl_attack_cooldown == 0u){
                    if(s_mother->type == SpriteMotherplarmor){
                        motherplarmor_setanim_ascending();
                    }else{ motherplnormal_setanim_ascending();}
                }
            break;
            case MOTHERPL_CRAWL:
            case MOTHERPL_CRAWL_SURF:
                //if(motherpl_attack_cooldown == 0u){
                    if(s_mother->type == SpriteMotherplarmor){
                        motherplarmor_setanim_crawl();
                    }else{ motherplnormal_setanim_crawl(); }
                //}
                motherpl_jpower = 0;
                jump_max_toched = 0u;
                motherpl_vy = GRAVITY;
                motherpl_vx = 0;
                motherpl_inertiax = 0u;
                camera_ok = 0u;
            break;
            case MOTHERPL_WALK:
                if(motherpl_attack_cooldown == 0u){
                   motherpl_refresh_animation(s_mother);
                }
                motherpl_jpower = 0;
                jump_max_toched = 0u;
            break;
            case MOTHERPL_HIT:
                my_play_fx(CHANNEL_2, 60, 0xce, 0x53, 0xb2, 0x83, 0x00);//SFX_HIT
                motherpl_hit_cooldown = HIT_COOLDOWN_MAX;
                motherpl_hp -= motherpl_hit;
                motherpl_hit = 0;
                if(motherpl_hp <= 0 && motherpl_state != MOTHERPL_DEAD){
                    motherpl_changeMotherplState(s_mother, MOTHERPL_DEAD);
                    return;
                }
                //TODO START remove me invulnerability!
                //if(motherpl_hp <= 0){motherpl_hp = 1;}
                //TODO END remove me
                if(s_mother->mirror == NO_MIRROR){    
                    SpriteManagerAdd(SpritePuff, s_mother->x + 2, s_mother->y + 4);
                    s_mother->x-=6;
                }else{
                    Sprite* hit_puff = SpriteManagerAdd(SpritePuff, s_mother->x - 4, s_mother->y + 4);
                    hit_puff->mirror = s_mother->mirror;
                    s_mother->x+=6;
                }
                if(s_blocking){SpriteManagerRemoveSprite(s_blocking);}
                if(motherpl_hp > 0){
                    if(s_mother->type == SpriteMotherplarmor){
                        motherplarmor_setanim_hit();
                    }else{motherplnormal_setanim_hit();}
                }
            break;
            case MOTHERPL_DEAD:
                motherpl_vx = 0;
                motherpl_vy = 0;
                motherpl_hp = 0;
                    if(s_mother->type == SpriteMotherplarmor){
                        motherplarmor_setanim_dead();
                    }else{motherplnormal_setanim_dead();}
                motherpl_hit_cooldown = DEAD_COOLDOWN_MAX;                
            break;
            case MOTHERPL_BLOCKED:
                motherpl_vx = 0;
                motherpl_blocked = 2u;
                    if(s_mother->type == SpriteMotherplarmor){
                        motherplarmor_setanim_blocked();
                    }else{motherplnormal_setanim_blocked();}
                motherpl_blocked_cooldown = BLOCKED_COOLDOWN_MAX;
                s_blockcmd = SpriteManagerAdd(SpriteRightleft, s_mother->x - 2u, s_mother->y - 20u);
            break;
            case MOTHERPL_PICKUP:
                motherpl_inertiax = 0u;
                pickingup_cooldown = PICKINGUP_COOLDOWN;
                    if(s_mother->type == SpriteMotherplarmor){
                        motherplarmor_setanim_crawl();
                    }else{motherplnormal_setanim_crawl();}
            break;            
            case MOTHERPL_DASH:
                dash_horizontal_time = 12u;
                if(motherpl_state == MOTHERPL_JUMP){s_mother->y -= 4u;}
                if(s_mother->mirror == NO_MIRROR){
                    motherpl_vx = 2;
                }else{
                    motherpl_vx = -2;
                }
            break;
        }
        motherpl_state = new_state;
        motherpl_refresh_animation(s_mother);
    }
}

void motherpl_behave(Sprite* s_mother) BANKED{
    if(motherpl_state == MOTHERPL_DEAD || motherpl_hp <= 0){
        motherpl_vx = 0;
        motherpl_vy = 0;
        if(motherpl_hit_cooldown > 0){
            motherpl_hit_cooldown--;
            if(motherpl_hit_cooldown < (DEAD_COOLDOWN_MAX >> 1)){
                s_mother->y--;
                if(motherpl_hit_cooldown < (DEAD_COOLDOWN_MAX >> 2)){
                    motherpl_refresh_animation(s_mother);
                }
            }
        }else{//già fatto il giro verso l' alto
            motherpl_die(s_mother);
        }
        return;
    }
    switch(motherpl_state){
        case MOTHERPL_IDLE:
            /*
            if(motherpl_attack_cooldown == 0u){
                motherpl_refresh_animation(s_mother);
            }
            */
            motherpl_jpower = 0;
            jump_max_toched = 0u;
            motherpl_vy = GRAVITY;
            if(motherpl_vx != 0){
                motherpl_changeMotherplState(s_mother, MOTHERPL_WALK);
            }
        break;
        case MOTHERPL_JUMP:
            if(KEY_TICKED(J_JUMP)){
                jump_ticked_delay = JUMP_TICKED_COOLDOWN;
            }
            if(motherpl_coll_y){
                if(motherpl_jpower == 0){
                    if(jump_ticked_delay == 0){
                        motherpl_changeMotherplState(s_mother, MOTHERPL_PICKUP);
                    }else{
                        motherpl_changeMotherplState(s_mother, MOTHERPL_IDLE);
                    }
                }else{
                    motherpl_changeMotherplState(s_mother, MOTHERPL_IDLE);
                }
            }
            if(KEY_RELEASED(J_JUMP)){
                motherpl_jpower = JUMP_MIN_POWER;
                jump_max_toched = 0u;
                motherpl_vy = GRAVITY;
                jump_ticked_delay = JUMP_TICKED_COOLDOWN;
            }else if(KEY_PRESSED(J_JUMP) && jump_ticked_delay == 0){
                if(motherpl_jpower < jump_max_power){
                    motherpl_jpower++;
                    if(motherpl_jpower < (jump_max_power / 3)){
                        motherpl_vy = -3;
                    }else if(motherpl_jpower < (jump_max_power / 2)){
                        motherpl_vy = -2;
                    }else{
                        motherpl_vy = -1;
                    }
                }else if(motherpl_jpower == jump_max_power){
                    if(fly_counter < FLY_MAX){
                        fly_counter++;
                        motherpl_vy = 0;
                    }else if(motherpl_vy < GRAVITY){
                        motherpl_vy++;
                    }
                }
            }else{
                if (!KEY_PRESSED(J_JUMP) && motherpl_jpower > JUMP_MIN_POWER){
                    motherpl_jpower--;
                }
                if(motherpl_vy < GRAVITY){
                    motherpl_vy++;
                }
            }
        break;
        case MOTHERPL_WALK:
            motherpl_refresh_animation(s_mother);
            if(motherpl_inertiax == 0){//|| (KEY_RELEASED(J_RIGHT) || KEY_RELEASED(J_LEFT))){
                motherpl_changeMotherplState(s_mother, MOTHERPL_IDLE);
            }
        break;
        case MOTHERPL_HIT:
            motherpl_vy = GRAVITY;
            motherpl_refresh_animation(s_mother);
            if(motherpl_hp <= 0){
                motherpl_changeMotherplState(s_mother, MOTHERPL_DEAD);
            }
        break;
        case MOTHERPL_BLOCKED:
            if(KEY_TICKED(J_LEFT) || KEY_TICKED(J_RIGHT)){
                motherpl_blocked_cooldown += 10;
                if(motherpl_blocked_cooldown >= 80){//got away!
                    if(s_blocking->type == SpriteEnemyBat){
                        s_blocking->y -= 24u;
                        bat_change_state(s_blocking, ENEMY_SLIDE_UP);
                    }
                    motherpl_blocked = 0u;
                    motherpl_blocked_cooldown = BLOCKED_COOLDOWN_MAX;
                    motherpl_changeMotherplState(s_mother, MOTHERPL_IDLE);                 
                }
            }
            motherpl_blocked_cooldown--;
            if(motherpl_blocked_cooldown < 12 || s_blocking == 0){
                if(s_blocking->type == SpriteEnemyBat){
                    motherpl_hitted(s_blocking);
                }
                motherpl_blocked = 0u;
                motherpl_blocked_cooldown = BLOCKED_COOLDOWN_MAX;
                SpriteManagerRemoveSprite(s_blocking);
            }
        break;
        case MOTHERPL_PICKUP:
            pickingup_cooldown--;
            if(pickingup_cooldown <= 0){
                pickingup_cooldown = PICKINGUP_COOLDOWN;
                motherpl_changeMotherplState(s_mother, MOTHERPL_IDLE);
            }
            return;
        break;
        case MOTHERPL_CRAWL:
            if(KEY_RELEASED(J_DOWN)){
                camera_ok = 0u;
            }
        break;
        case MOTHERPL_CRAWL_SURF:
            if(s_surf != 0){
                s_mother->x = s_surf->x + 2u;//motherpl_surf_dx;
                s_mother->y = s_surf->y - 20u;
            }else{
                motherpl_changeMotherplState(s_mother, MOTHERPL_IDLE);
            }
            if(KEY_TICKED(J_JUMP)){
                if(motherpl_rabbit == 0u){
                    motherpl_rabbit = 1u;
                    motherpl_changeMotherplState(s_mother, MOTHERPL_JUMP);
                }
            }
        break;
        case MOTHERPL_DASH:
            if(dash_horizontal_time > 0){dash_horizontal_time--;}
            if(motherpl_vy < GRAVITY){
                motherpl_vy++;
            }
            motherpl_dash_cooldown--;
            if(motherpl_dash_cooldown == 0){
                motherpl_dash_cooldown = DASH_COOLDOWN_MAX;
                if(KEY_PRESSED(J_RIGHT) || KEY_PRESSED(J_LEFT)){
                    motherpl_changeMotherplState(s_mother, MOTHERPL_WALK);
                }else{
                    motherpl_changeMotherplState(s_mother, MOTHERPL_IDLE);
                }
            }
            {//TODO check, tile 48u in lvl nonMine potrebbe essere altro
                UINT8 dash_floor = GetScrollTile((s_mother->x >> 3) + 1u, (s_mother->y >> 3) + 2u);
                if(current_state == StateMine || current_state == StateBatcave){
                    if(dash_floor == 48u && powder_cooldown == 0){
                        powder_cooldown = 60u;
                        spawnItem(INVITEM_POWDER, s_mother->x, s_mother->y);
                    }
                }
                if(dash_floor != 0 && motherpl_dash_cooldown % 8 == 0 && current_state != StateBossbat){
                    motherpl_spawnDust(s_mother);
                }
            }
        break;
    }
}

void UPDATE(){

}