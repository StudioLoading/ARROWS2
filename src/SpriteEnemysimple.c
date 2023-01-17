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

extern UINT8 enemy_counter;
extern Sprite* s_motherpl;
extern struct MotherplData* d_motherpl;
extern UINT8 motherpl_hit;
extern MOTHERPL_STATE motherpl_state;
extern struct EtoReload e_to_reload[3];

void Estart() BANKED;
void configure() BANKED;
void ETurn(UINT8 e_vx);
void changeEstate(ENEMY_STATE new_e_state) BANKED;
UINT8 getEmaxFrameskip();
void Econfiguration() BANKED;
void Emanagement() BANKED;
void Edestroy() BANKED;

extern void EsimpleSnakeAnim(ENEMY_STATE estate) BANKED;
extern void EsimpleRatAnim(ENEMY_STATE estate) BANKED;
extern void EattackerPineAnim(ENEMY_STATE estate) BANKED;
extern void EattackerCobraAnim(ENEMY_STATE estate) BANKED;
extern void EthrowerSpiderAnim(ENEMY_STATE estate) BANKED;
extern void EthrowerTarantulaAnim(ENEMY_STATE estate) BANKED;
extern void EthrowWeb(ENEMY_STATE estate) BANKED;
extern void EthrowAcid(ENEMY_STATE estate) BANKED;
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
                && THIS->type != SpriteEnemyThrowerSpider 
                && eu_info->type != SpriteEnemyThrowerTarantula
                )
            )
        ){//x_frameskip used            
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
                        if(motherpl_state == MOTHERPL_DASH){
                            changeEstate(ENEMY_UPSIDEDOWN);
                        } else if(motherpl_hit != 1u){
                            motherpl_hit = 1u;
                            changeEstate(ENEMY_WAIT);
                        }
                    break;
                    case SpriteArrow:
                        if((s_motherpl->x < THIS->x && THIS->mirror == NO_MIRROR) || 
                            (s_motherpl->x > THIS->x && THIS->mirror == V_MIRROR)){
                                if(eu_info->hp > 1){
                                    ETurn(eu_info->vx);
                                }
                        }
                        {
                            struct ArrowData* arrow_data = (struct ArrowData*) iespr->custom_data;
                            arrow_data->hit = 1u;
                        }
                        changeEstate(ENEMY_HIT);
                    break;
                    case SpriteEnemythrowable:
                        if(THIS->type == SpriteEnemysimplerat || THIS->type == SpriteEnemysimplesnake){
                            changeEstate(ENEMY_HIT);
                        }
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
                    switch(THIS->type){
                        case SpriteEnemyAttackerCobra:
                        case SpriteEnemyAttackerPine:
                        case SpriteEnemyThrowerSpider:
                        case SpriteEnemyThrowerTarantula:
                            changeEstate(ENEMY_PREATTACK);
                        break;
                    }
                }
            break;
            case ENEMY_PREATTACK:
                eu_info->wait--;
                if(eu_info->wait == 0u){
                    switch(THIS->type){
                        case SpriteEnemyAttackerPine:
                        case SpriteEnemyAttackerCobra:
                            changeEstate(ENEMY_ATTACK);
                        break;
                        case SpriteEnemyThrowerSpider:
                        case SpriteEnemyThrowerTarantula:
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
            case ENEMY_UPSIDEDOWN:
                eu_info->wait--;
                if(eu_info->wait == 0u){
                    if(eu_info->vx >= 0){
                        THIS->mirror = NO_MIRROR;
                    }else{
                        THIS->mirror = V_MIRROR;
                    }
                    changeEstate(ENEMY_WAIT);
                }
                return;
            break;
        }
}

void Estart() BANKED{
    THIS->lim_x = 255u;
    THIS->lim_y = 255u;
    UINT8 i = 0u;    
    for(i = 0u; i < 3u; ++i){
        UINT8 j = 0u;    
        for(j = 0u; j < 3u; ++j){
            if(e_to_reload[j].type == THIS->type && e_to_reload[j].alive == 1u){
                i = 3u;
            }
        }
        if(e_to_reload[i].alive == 0u && i < 3u){
            e_to_reload[i].x = THIS->x;
            e_to_reload[i].y = THIS->y;
            e_to_reload[i].type = THIS->type;
            e_to_reload[i].alive = 1u;
        }
    }
    enemy_counter++;
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
        case SpriteEnemyAttackerCobra:
            result = E_FRAMSKIP_COBRA;
        break;
        case SpriteEnemyAttackerPine:
            result = E_FRAMSKIP_PINE;
        break;
        case SpriteEnemyThrowerSpider:
        case SpriteEnemyThrowerTarantula:
            result = E_FRAMSKIP_SPIDER;
        break;
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
    changeEstate(ENEMY_WAIT);
}

void configure() BANKED{
    struct EnemyData* e_info = (struct EnemyData*) THIS->custom_data;
    switch(THIS->type){
        case SpriteEnemysimplesnake:
        case SpriteEnemysimplerat:
        case SpriteEnemyAttackerCobra:
        case SpriteEnemyAttackerPine:
        case SpriteEnemyThrowerSpider:
        case SpriteEnemyThrowerTarantula:
            e_info->hp = 1;
        break;
    }
    e_info->x_frameskip = 1u;
    e_info->vx = E_VX;
    e_info->configured = 2u;
    e_info->e_state =  ENEMY_IDLE;
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
                switch(THIS->type){
                    case SpriteEnemyAttackerCobra:
                        e_info->x_frameskip = E_FRAMSKIP_COBRA;
                        e_info->wait = enemy_random_30_100 + 100u;
                    break;
                    case SpriteEnemyAttackerPine:                    
                        e_info->x_frameskip = E_FRAMSKIP_PINE;
                        e_info->wait = enemy_random_30_100 + 60u;
                    break;
                    case SpriteEnemyThrowerSpider:
                        e_info->x_frameskip = E_FRAMSKIP_SPIDER;
                        e_info->wait = enemy_random_30_100 + 100u;
                    break;
                    case SpriteEnemyThrowerTarantula:
                        e_info->x_frameskip = E_FRAMSKIP_SPIDER;
                        e_info->wait = enemy_random_30_100 + 100u;
                    break;
                }
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
                spawnItem(THIS->x, THIS->y, THIS->type);
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
                switch(THIS->type){
                    case SpriteEnemyThrowerSpider: EthrowWeb(e_info->e_state); break;
                    case SpriteEnemyThrowerTarantula: EthrowAcid(e_info->e_state); break;
                }
            break;
            case ENEMY_UPSIDEDOWN:
                e_info->wait = 200u;
                if(e_info->vx < 0){
                    THIS->mirror = HV_MIRROR;
                }else{
                    THIS->mirror = H_MIRROR;
                }
                TranslateSprite(THIS, 0, -10 << delta_time);
            break;
        }
        //UPDATE ANIMATION
        switch(THIS->type){
            case SpriteEnemysimplesnake: EsimpleSnakeAnim(new_e_state);  break;
            case SpriteEnemysimplerat: EsimpleRatAnim(new_e_state);  break;
            case SpriteEnemyAttackerCobra: EattackerCobraAnim(new_e_state); break;
            case SpriteEnemyAttackerPine: EattackerPineAnim(new_e_state); break;
            case SpriteEnemyThrowerSpider: EthrowerSpiderAnim(new_e_state); break;
            case SpriteEnemyThrowerTarantula: EthrowerTarantulaAnim(new_e_state); break;
        }
        e_info->e_state = new_e_state;
    }
}

void Edestroy(){
    UINT8 i = 0u;
    for(i = 0; i < 3; ++i){
        if(e_to_reload[i].type == THIS->type){
            e_to_reload[i].alive = 0u;
            return;
        }
    }
}

void DESTROY(){
}