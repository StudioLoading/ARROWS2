#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define E_GRAVITY 4
#define E_VX 1
#define E_FRAMSKIP_SNAKE 3
#define E_FRAMSKIP_RAT 1
#define E_FRAMSKIP_PINE 3
#define E_FRAMSKIP_COBRA 1
#define E_FRAMSKIP_SPIDER 0
#define MAX_ENEMY 3


const UINT8 e_anim_hidden[] = {1, 0};

UINT8 enemy_random_30_100 = 30u;
UINT8 enemy_counter = 0u;

extern Sprite* s_motherpl;
extern struct MotherplData* d_motherpl;
extern UINT8 motherpl_hit;

void Estart() BANKED;
void configure() BANKED;
void ETurn(UINT8 e_vx);
void changeEstate(ENEMY_STATE new_e_state) BANKED;
UINT8 getEmaxFrameskip();
void Econfiguration() BANKED;
void Emanagement() BANKED;

extern void EsimpleSnakeAnim(ENEMY_STATE estate) BANKED;
extern void EsimpleRatAnim(ENEMY_STATE estate) BANKED;
extern void EattackerAnim(ENEMY_STATE estate) BANKED;
extern void EthrowerAnim(ENEMY_STATE estate) BANKED;
extern void Ethrow(ENEMY_STATE estate) BANKED;
extern void spawnItem(UINT16 x, UINT16 y, UINT8 enemy_type_dead) BANKED;


void START(){
    Estart();
}

void UPDATE(){
    //configuration
        struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
        if(eu_info->configured == 0){
            return;
        }
        if(eu_info->configured == 1){
            Econfiguration();
            return;
        }
        if(eu_info->configured != 2u){
            return;
        }
    //CHECK DEATH
        if(eu_info->hp <= 0){changeEstate(ENEMY_DEAD);}
    //MANAGEMENT
        Emanagement();
}

void Emanagement() BANKED{
    struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
    //ErandomManagement();
        enemy_random_30_100++;
        if(enemy_random_30_100 >= 100u){enemy_random_30_100 = 30u;}
    //Egravity();
        UINT8 e_v_coll = TranslateSprite(THIS, 0, E_GRAVITY << delta_time);
    //EhorizontalTileCollision(eu_info);
        if(eu_info->x_frameskip == 0 && 
            (eu_info->e_state == ENEMY_WALK ||
                (eu_info->e_state == ENEMY_ATTACK 
                && eu_info->type != SPIDER 
                && eu_info->type != TARANTULA))){//x_frameskip used            
                eu_info->et_collision = TranslateSprite(THIS, eu_info->vx << delta_time, 0);
                if(eu_info->et_collision){
                    switch(eu_info->et_collision){
                        case 8u:
                        case 9u:
                            ETurn(E_VX);
                        break;
                    }
                }
                eu_info->x_frameskip = 1u;
            }else{
                UINT8 max_frameskip = getEmaxFrameskip();
                if(eu_info->x_frameskip < max_frameskip){
                    eu_info->x_frameskip++;
                }else{
                    eu_info->x_frameskip = 0u;
                }
            }

    //EspriteCollision(eu_info); 
        UINT8 scroll_e_tile;
        Sprite* iespr;
        SPRITEMANAGER_ITERATE(scroll_e_tile, iespr) {
            if(CheckCollision(THIS, iespr)) {
                switch(iespr->type){
                    case SpriteMotherpl://io enemy ho colpito motherpl
                        if(motherpl_hit != 1u){motherpl_hit = 1u;}
                        changeEstate(ENEMY_WAIT);
                    break;
                    case SpriteArrow:
                        if((s_motherpl->x < THIS->x && THIS->mirror == NO_MIRROR) || 
                            (s_motherpl->x > THIS->x && THIS->mirror == V_MIRROR)){
                                ETurn(eu_info->vx);
                        }
                        {
                            struct ArrowData* arrow_data = (struct ArrowData*) iespr->custom_data;
                            arrow_data->hit = 1u;
                        }
                        changeEstate(ENEMY_HIT);
                    break;
                    case SpriteEnemythrowable:
                        //if(THIS->type == SpriteEnemyattacker){
                            changeEstate(ENEMY_HIT);
                        //}
                    break;
                }
            }
        };
    //EstateBehavior(eu_info);
        switch(eu_info->e_state){
            case ENEMY_DEAD:
                eu_info->wait--;
                THIS->y--;
                if(eu_info->wait == 0u){
                    enemy_counter--;
                    SpriteManagerRemoveSprite(THIS);
                }
                return;
            break;
            case ENEMY_HIT:
            case ENEMY_WAIT:
                if(eu_info->wait){eu_info->wait--;}
                else{changeEstate(ENEMY_WALK);}
                return;
            break;
            case ENEMY_WALK:
                eu_info->wait--;
                if(eu_info->wait == 0u){
                    switch(eu_info->type){
                        case PORCUPINE:
                        case COBRA:
                        case SPIDER:
                        case TARANTULA:
                            changeEstate(ENEMY_PREATTACK);
                        break;
                    }
                }
            break;
            case ENEMY_PREATTACK:
                eu_info->wait--;
                if(eu_info->wait == 0u){
                    switch(eu_info->type){
                        case PORCUPINE:
                        case COBRA:
                            changeEstate(ENEMY_ATTACK);
                        break;
                        case SPIDER:
                        case TARANTULA:
                            changeEstate(ENEMY_THROW);
                        break;
                    }
                }
            break;
            case ENEMY_ATTACK:
                eu_info->wait--;
                if(eu_info->wait == 0u){changeEstate(ENEMY_WALK);}
            break;
            case ENEMY_THROW:
                eu_info->wait--;
                if(eu_info->wait == 0u){changeEstate(ENEMY_WAIT);}
            break;
        }        
        switch(THIS->type){
            case SpriteEnemysimplesnake: EsimpleSnakeAnim(eu_info->e_state);break;
            case SpriteEnemysimplerat: EsimpleRatAnim(eu_info->e_state); break;
        }
}

void Estart() BANKED{
    if(enemy_counter == MAX_ENEMY){
        SpriteManagerRemoveSprite(THIS);
        return;
    }
    enemy_counter++;
    SetSpriteAnim(THIS, e_anim_hidden, 12u);
    THIS->lim_x = 255u;
    struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
    eu_info->configured = 1u;
}

void Econfiguration() BANKED{
    struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
    if(eu_info->configured == 1u){
        configure();
        return;
    }
}

UINT8 getEmaxFrameskip(){
    UINT8 result = 0u;
    switch(THIS->type){
        case SpriteEnemysimplesnake:
        case SpriteEnemysimplerat:
            result = E_FRAMSKIP_SNAKE;
        break;
        /*
        case PORCUPINE:
            result = E_FRAMSKIP_PINE;
        break;
        case COBRA:
            result = E_FRAMSKIP_COBRA;
        break;
        case SPIDER:
            result = E_FRAMSKIP_SPIDER;
        break;*/
    }
    return result;
}

void ETurn(UINT8 e_vx){
    struct EnemyData* e_info = (struct EnemyData*) THIS->custom_data;
    if(e_info->vx > 0){
        e_info->vx = -e_vx;
        THIS->mirror = V_MIRROR;
        THIS->x--;
    }else{
        e_info->vx = e_vx;
        THIS->mirror = NO_MIRROR;
        THIS->x++;
    }
    e_info->wait = 24u;
    //if(e_info->e_state != ENEMY_WALK){
        changeEstate(ENEMY_WAIT);
    //}
}

void configure() BANKED{
    struct EnemyData* e_info = (struct EnemyData*) THIS->custom_data;
    switch(THIS->type){
        case SpriteEnemysimplesnake:
        case SpriteEnemysimplerat:
            e_info->hp = 1;
        break;
        /*case PORCUPINE:
        case COBRA:
        case SPIDER:
        case TARANTULA:
            e_info->hp = 2;
        break;*/
    }
    e_info->x_frameskip = 1u;
    e_info->vx = E_VX;
    e_info->configured = 2u;
    changeEstate(ENEMY_WALK);
}

void changeEstate(ENEMY_STATE new_e_state) BANKED{
    struct EnemyData* e_info = (struct EnemyData*) THIS->custom_data;
    if(new_e_state == ENEMY_HIT && e_info->e_state == ENEMY_ATTACK){
        return;//Invulnerabilità durante l' attacco!
    }
    if(e_info->e_state != new_e_state && e_info->e_state != ENEMY_DEAD){
        switch(new_e_state){
            case ENEMY_WALK:
                    /*
                    case PORCUPINE:
                        e_info->x_frameskip = E_FRAMSKIP_PINE;
                        e_info->wait = enemy_random_30_100 + 60u;
                    break;
                    case COBRA:
                        e_info->x_frameskip = E_FRAMSKIP_COBRA;
                        e_info->wait = enemy_random_30_100 + 100u;
                    break;
                    case SPIDER:
                        e_info->x_frameskip = E_FRAMSKIP_SPIDER;
                        e_info->wait = enemy_random_30_100 + 100u;
                    break;
                    case TARANTULA:
                        e_info->x_frameskip = E_FRAMSKIP_SPIDER;
                        e_info->wait = enemy_random_30_100 + 100u;
                    break;
                    */
            break;
            case ENEMY_IDLE:
            break;
            case ENEMY_WAIT:
                e_info->wait = 40u;
            break;
            case ENEMY_HIT:
                e_info->hp--;
                e_info->wait = 56u;
                if(e_info->hp <= 0u){
                    changeEstate(ENEMY_DEAD);
                    return;
                }
            break;
            case ENEMY_DEAD:
                //spawnItem(THIS->x, THIS->y, THIS->type);
                e_info->wait = 24u;
            break;
            case ENEMY_PREATTACK:
                e_info->wait = 40u;
            break;
            case ENEMY_ATTACK:
                if(THIS->mirror == NO_MIRROR){e_info->vx = 2*E_VX;}
                else{e_info->vx = 2* (-E_VX);}
                e_info->wait = enemy_random_30_100;
                e_info->x_frameskip = 0u;
            break;
            case ENEMY_THROW:
                Ethrow(e_info->e_state);
            break;
        }
        //UPDATE ANIMATION
        switch(THIS->type){
            case SpriteEnemysimplesnake: EsimpleSnakeAnim(new_e_state);  break;
            case SpriteEnemysimplerat: EsimpleRatAnim(new_e_state);  break;
        }
        e_info->e_state = new_e_state;
    }
}

void DESTROY(){
}