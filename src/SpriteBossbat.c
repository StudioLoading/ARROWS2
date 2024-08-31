#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define BOSSBAT_MAX_COOLDOWN 160
#define BOSSBAT_MID_COOLDOWN 120
#define BOSSBAT_MIN_COOLDOWN 70
#define BOSSBAT_GRAVITY 1
#define BOSSBAT_VX_MAX 4
#define BOSSBAT_VX_MID 2
#define BOSSBAT_VX_MIN 1


const UINT8 a_bossbat_fly[] = {4, 2,3,4,5};
const UINT8 a_bossbat_falling[] = {1, 2};
const UINT8 a_bossbat_idle[] = {1, 1};
const UINT8 a_bossbat_hit[] = {2, 0,2};

UINT16 smotherpl_posx = 0u;
UINT16 smotherpl_posy = 0u;
ENEMY_STATE prev_state = ENEMY_WALK;
INT8 prev_wait = 0;
UINT8 bossbat_current_max_wait = 0u;
struct EnemyData* bossbat_data = 0;

extern Sprite* s_motherpl;
extern MOTHERPL_STATE motherpl_state;
extern void motherpl_hitted(Sprite* s_enemy) BANKED;

void bossbat_change_state(ENEMY_STATE e_state) BANKED;
void bossbat_manage_gradini() BANKED;


void START(){
    SetSpriteAnim(THIS,a_bossbat_idle, 16u);
    THIS->mirror = V_MIRROR;
    bossbat_data  = (struct EnemyData*) THIS->custom_data;
    bossbat_data->hp = 6;
    bossbat_data->wait = 80;
    bossbat_data->configured = 0;
    bossbat_data->e_state = ENEMY_IDLE;
    bossbat_data->vx = -1;
    bossbat_current_max_wait = BOSSBAT_MAX_COOLDOWN;
}

void UPDATE(){
    if(bossbat_data->hp <= 0){
        THIS->x+=3;
        THIS->y--;
        return;
    }
    if(THIS->x < 16u){
        THIS->x = 16u;
        if(bossbat_data->e_state == ENEMY_WALK && bossbat_data->vx < 0){
            bossbat_change_state(ENEMY_SLIDE_UP);
        }
    }
    //use configured flag as hit
    if(bossbat_data->configured == 1){
        bossbat_change_state(ENEMY_HIT_1);
        bossbat_data->configured = 0;
    }
    bossbat_data->wait--;
    switch(bossbat_data->e_state){
        case ENEMY_SLIDE_DOWN://discesa
            {
            INT8 vy = 2;
            bossbat_data->et_collision = TranslateSprite(THIS,bossbat_data->vx, vy);
            if(bossbat_data->et_collision){
                if(bossbat_data > 1){bossbat_change_state(ENEMY_WALK);}
                else{bossbat_change_state(ENEMY_SLIDE_UP);}
            }
            }
        break;
        case ENEMY_SLIDE_UP://ascesa
            {
            INT8 vy = -2;
            bossbat_data->et_collision = TranslateSprite(THIS,bossbat_data->vx, vy);
            switch(bossbat_data->et_collision){
                case 34u:
                case 35u:
                case 36u:
                case 37u:
                    bossbat_change_state(ENEMY_IDLE);
                break;
            }
            }
        break;
        case ENEMY_IDLE:
            if(bossbat_data->wait < 5 || bossbat_data->wait > bossbat_current_max_wait){
                bossbat_change_state(ENEMY_SLIDE_DOWN);
            }
        break;
        case ENEMY_WALK:
            {
            INT8 vy = 0;
            bossbat_data->et_collision = TranslateSprite(THIS,bossbat_data->vx, vy);
            if(bossbat_data->et_collision == 54u){bossbat_manage_gradini();}
            if(bossbat_data->wait == 0){
                bossbat_change_state(ENEMY_SLIDE_UP);
            }
            }
        break;
        case ENEMY_HIT_1:
            if(bossbat_data->wait < 4){
                bossbat_change_state(prev_state);
            }
        break;
    }
    
    //EspriteCollision(eu_info); 
        UINT8 scroll_bossbat_tile;
        Sprite* bossbat_espr;
        SPRITEMANAGER_ITERATE(scroll_bossbat_tile, bossbat_espr) {
            if(CheckCollision(THIS, bossbat_espr)) {
                switch(bossbat_espr->type){
                    case SpriteMotherpl:
                    case SpriteMotherplarmor://io enemy ho colpito motherpl
                        if(bossbat_data->e_state != ENEMY_IDLE && motherpl_state != MOTHERPL_DASH){
                            motherpl_hitted(THIS);
                        }
                    break;
                }
            }
        };
}

void bossbat_manage_gradini() BANKED{
    if(bossbat_data->e_state == ENEMY_WALK && bossbat_data->vx < 0){
        THIS->y -= 2;
    }
}

void bossbat_change_state(ENEMY_STATE e_state) BANKED{
    smotherpl_posx = s_motherpl->x;
    smotherpl_posy = s_motherpl->y;
    switch(e_state){
        case ENEMY_HIT_1:
            if(bossbat_data->e_state != ENEMY_IDLE && bossbat_data->e_state != ENEMY_HIT_1){
                prev_wait = bossbat_data->wait;
                prev_state = bossbat_data->e_state;
                bossbat_data->hp--;
                bossbat_data->wait = 120u;
                THIS->anim_frame = 0;
                SetSpriteAnim(THIS, a_bossbat_hit, 24u);
            }
        break;
        case ENEMY_WALK:
            switch(bossbat_data->hp){
                case 6:
                case 5:
                    bossbat_data->wait = 40u;
                break;
            }
            if(bossbat_data->e_state == ENEMY_HIT_1){
                bossbat_data->wait = prev_wait;
            }
            SetSpriteAnim(THIS, a_bossbat_fly, 16u);    
        break;
        case ENEMY_SLIDE_DOWN:
            bossbat_data->wait = bossbat_current_max_wait >> 1;
            SetSpriteAnim(THIS, a_bossbat_falling, 16u);
            SpriteManagerAdd(SpriteCavesand, THIS->x + 8u, THIS->y + 8u); 
            THIS->y +=6;
            if(smotherpl_posx > THIS->x && bossbat_data->vx < 0){
                THIS->mirror = NO_MIRROR;
                bossbat_data->vx = -bossbat_data->vx;
            }else if(smotherpl_posx < THIS->x && bossbat_data->vx > 0){
                THIS->mirror = V_MIRROR;
                bossbat_data->vx = -bossbat_data->vx;

            }
            if(bossbat_data->e_state == ENEMY_HIT_1){
                bossbat_data->wait = prev_wait;
            }
        break;
        case ENEMY_SLIDE_UP:
            bossbat_data->wait = BOSSBAT_MAX_COOLDOWN;
            SetSpriteAnim(THIS, a_bossbat_fly, 24u);
            THIS->anim_speed = 56u; 
            if(bossbat_data->e_state == ENEMY_HIT_1){
                bossbat_data->wait = prev_wait;
            }
        break;
        case ENEMY_IDLE:
            SetSpriteAnim(THIS, a_bossbat_idle, 1u);
            THIS->y -= 3;
            bossbat_data->vx = -bossbat_data->vx;
            switch(bossbat_data->hp){
                case 1:
                case 2:
                    if(bossbat_data->vx > 0){bossbat_data->vx = BOSSBAT_VX_MAX;}
                    else{bossbat_data->vx = -BOSSBAT_VX_MAX;}
                    bossbat_current_max_wait = BOSSBAT_MIN_COOLDOWN;
                break;
                case 3:
                case 4:
                case 5:
                    if(bossbat_data->vx > 0){bossbat_data->vx = BOSSBAT_VX_MID;}
                    else{bossbat_data->vx = -BOSSBAT_VX_MID;}
                    bossbat_current_max_wait = BOSSBAT_MID_COOLDOWN;
                break;
            }    
            THIS->mirror = NO_MIRROR;
            if(bossbat_data->vx < 0){
                THIS->mirror = V_MIRROR;
            }
        break;
        case ENEMY_DEAD:
            bossbat_data->hp = 0;
        break;
    }
    bossbat_data->e_state = e_state;
}

void DESTROY(){
    bossbat_data->wait = 100u;
    bossbat_change_state(ENEMY_DEAD);
}