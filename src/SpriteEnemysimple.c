#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
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


const UINT8 e_anim_hidden[] = {1, 0};
const UINT8 snake_anim_idle[] = {1, 1}; //The first number indicates the number of frames
const UINT8 snake_anim_walk[] = {4, 3, 4, 1, 2}; //The first number indicates the number of frames
const UINT8 snake_anim_hit[] = {2, 1, 3}; //The first number indicates the number of frames
const UINT8 rat_anim_idle[] = {1, 5}; //The first number indicates the number of frames
const UINT8 rat_anim_walk[] = {3, 6, 5, 7}; //The first number indicates the number of frames
const UINT8 rat_anim_hit[] = {2, 5, 0}; //The first number indicates the number of frames

UINT8 enemy_random_30_100 = 30u;

extern Sprite* s_motherpl;
extern struct MotherplData* d_motherpl;
extern UINT8 motherpl_hit;

void Estart(Sprite* s_enemy) BANKED;
void configure(Sprite* s_enemy) BANKED;
void ETurn(Sprite* s_enemy, UINT8 e_vx);
void changeEstate(Sprite* s_enemy, ENEMY_STATE new_e_state) BANKED;
UINT8 getEmaxFrameskip(ENEMY_TYPE etype);
void Econfiguration(Sprite* s_enemy) BANKED;
void Emanagement(Sprite* s_enemy) BANKED;

extern void EattackerAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;
extern void EthrowerAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;
extern void Ethrow(Sprite* s_enemy, ENEMY_STATE estate) BANKED;

void spawnItem(UINT16 x, UINT16 y, UINT8 enemy_type_dead);

void spawnItem(UINT16 x, UINT16 y, UINT8 enemy_type_dead){
    INVITEMTYPE itemtype = INVITEM_MONEY;
    UINT16 quantity = 1u;
    switch(enemy_type_dead){
        case SpriteEnemyattacker:
            quantity = 5u;
        break;
    }
    Sprite* reward = SpriteManagerAdd(SpriteItemspawned, x + 4u, y - 8u);
    struct InvItem* reward_data = (struct InvItem*) reward->custom_data;
    reward_data->itemtype = itemtype;
    reward_data->quantity = quantity;
    reward_data->equippable = 1u;
    switch(itemtype){
        case INVITEM_CROSSBOW: reward_data->equippable = 0u; break;
    }
    reward_data->configured = 1u;
}

void START(){
    Estart(THIS);
}

void UPDATE(){
    //configuration
        struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
        if(eu_info->configured == 0){
            return;
        }
        if(eu_info->configured == 1){
            Econfiguration(THIS);
        }
    //RANDOM        
    //CHECK DEATH
        if(eu_info->hp <= 0){changeEstate(THIS, ENEMY_DEAD);}
    //MANAGEMENT
        Emanagement(THIS);
}

void Emanagement(Sprite* s_enemy) BANKED{
    struct EnemyData* eu_info = (struct EnemyData*) s_enemy->custom_data;
    //ErandomManagement();
        enemy_random_30_100++;
        if(enemy_random_30_100 >= 100u){enemy_random_30_100 = 30u;}

    //Egravity();
        UINT8 e_v_coll = TranslateSprite(s_enemy, 0, E_GRAVITY << delta_time);

    //EhorizontalTileCollision(eu_info);
        if(eu_info->x_frameskip == 0 && (eu_info->e_state == ENEMY_WALK ||
            (eu_info->e_state == ENEMY_ATTACK && eu_info->type != SPIDER && eu_info->type != TARANTULA))){//x_frameskip used            
                eu_info->et_collision = TranslateSprite(s_enemy, eu_info->vx << delta_time, 0);
                if(eu_info->et_collision){
                    switch(eu_info->et_collision){
                        case 8u:
                        case 9u:
                            ETurn(s_enemy, E_VX);
                        break;
                    }
                }
                eu_info->x_frameskip = 1u;
            }else{
                UINT8 max_frameskip = getEmaxFrameskip(eu_info->type);
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
            if(CheckCollision(s_enemy, iespr)) {
                switch(iespr->type){
                    case SpriteMotherpl://io enemy ho colpito motherpl
                        if(motherpl_hit != 1u){motherpl_hit = 1u;}
                        changeEstate(s_enemy, ENEMY_WAIT);
                    break;
                    /*
                    case SpriteArrow:
                        if((s_motherpl->x < s_enemy->x && s_enemy->mirror == NO_MIRROR) || 
                            (s_motherpl->x > s_enemy->x && s_enemy->mirror == V_MIRROR)){
                                ETurn(s_enemy, eu_info->vx);
                        }
                        {
                            struct ArrowData* arrow_data = (struct ArrowData*) iespr->custom_data;
                            arrow_data->hit = 1u;
                        }
                        changeEstate(s_enemy, ENEMY_HIT);
                    break;
                    */
                    case SpriteEnemythrowable:
                        if(s_enemy->type == SpriteEnemyattacker){
                            changeEstate(s_enemy, ENEMY_HIT);
                        }
                    break;
                }
            }
        };
    //EstateBehavior(eu_info);
        switch(eu_info->e_state){
            case ENEMY_DEAD:
                eu_info->wait--;
                s_enemy->y--;
                if(eu_info->wait == 0u){
                    SpriteManagerRemoveSprite(s_enemy);
                }
                return;
            break;
            case ENEMY_HIT:
                switch(eu_info->type){
                    case SNAKE: SetSpriteAnim(s_enemy, snake_anim_hit, 16u); break;
                    case RAT: SetSpriteAnim(s_enemy, rat_anim_hit, 16u); break;
                    case PORCUPINE: EattackerAnim(s_enemy, eu_info->e_state); break;
                    case COBRA: EattackerAnim(s_enemy, eu_info->e_state); break;
                    case SPIDER: EthrowerAnim(s_enemy, eu_info->e_state); break;
                    case TARANTULA: EthrowerAnim(s_enemy, eu_info->e_state); break;
                }
            case ENEMY_WAIT:
                if(eu_info->wait){eu_info->wait--;}
                else{changeEstate(s_enemy, ENEMY_WALK);}
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
                            changeEstate(s_enemy, ENEMY_PREATTACK);
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
                            changeEstate(s_enemy, ENEMY_ATTACK);
                        break;
                        case SPIDER:
                        case TARANTULA:
                            changeEstate(s_enemy, ENEMY_THROW);
                        break;
                    }
                }
            break;
            case ENEMY_ATTACK:
                eu_info->wait--;
                if(eu_info->wait == 0u){changeEstate(s_enemy, ENEMY_WALK);}
            break;
            case ENEMY_THROW:
                eu_info->wait--;
                if(eu_info->wait == 0u){changeEstate(s_enemy, ENEMY_WAIT);}
            break;
        }
}

void Estart(Sprite* s_enemy) BANKED{
    SetSpriteAnim(s_enemy, e_anim_hidden, 12u);
    s_enemy->lim_x = 255u;
}

void Econfiguration(Sprite* s_enemy) BANKED{
    struct EnemyData* eu_info = (struct EnemyData*) s_enemy->custom_data;
    if(eu_info->configured == 1u && eu_info->type != UNASSIGNED){
        configure(s_enemy);
        return;
    }
}

UINT8 getEmaxFrameskip(ENEMY_TYPE etype){
    UINT8 result = 0u;
    switch(etype){
        case SNAKE:
            result = E_FRAMSKIP_SNAKE;
        break;
        case RAT:
            result = E_FRAMSKIP_RAT;
        break;
        case PORCUPINE:
            result = E_FRAMSKIP_PINE;
        break;
        case COBRA:
            result = E_FRAMSKIP_COBRA;
        break;
        case SPIDER:
            result = E_FRAMSKIP_SPIDER;
        break;
    }
    return result;
}

void ETurn(Sprite* s_enemy, UINT8 e_vx){
    struct EnemyData* e_info = (struct EnemyData*) s_enemy->custom_data;
    if(e_info->vx > 0){
        e_info->vx = -e_vx;
        s_enemy->mirror = V_MIRROR;
        s_enemy->x--;
    }else{
        e_info->vx = e_vx;
        s_enemy->mirror = NO_MIRROR;
        s_enemy->x++;
    }
    e_info->wait = 24u;
    //if(e_info->e_state != ENEMY_WALK){
        changeEstate(s_enemy, ENEMY_WAIT);
    //}
}

void configure(Sprite* s_enemy) BANKED{
    struct EnemyData* e_info = (struct EnemyData*) s_enemy->custom_data;
    switch(e_info->type){
        case SNAKE:
        case RAT:
            e_info->hp = 1;
        break;
        case PORCUPINE:
        case COBRA:
        case SPIDER:
        case TARANTULA:
            e_info->hp = 2;
        break;
    }
    e_info->x_frameskip = 1u;
    e_info->vx = E_VX;
    e_info->configured = 2u;
    changeEstate(s_enemy, ENEMY_WALK);
}

void changeEstate(Sprite* s_enemy, ENEMY_STATE new_e_state) BANKED{
    struct EnemyData* e_info = (struct EnemyData*) s_enemy->custom_data;
    if(new_e_state == ENEMY_HIT && e_info->e_state == ENEMY_ATTACK){
        return;//Invulnerabilità durante l' attacco!
    }
    if(e_info->e_state != new_e_state && e_info->e_state != ENEMY_DEAD){
        switch(new_e_state){
            case ENEMY_WALK:
                switch(e_info->type){
                    case SNAKE: SetSpriteAnim(s_enemy, snake_anim_walk, 12u);break;
                    case RAT: SetSpriteAnim(s_enemy, rat_anim_walk, 12u); break;
                    case PORCUPINE:
                        e_info->x_frameskip = E_FRAMSKIP_PINE;
                        e_info->wait = enemy_random_30_100 + 60u;
                        EattackerAnim(s_enemy, new_e_state);
                    break;
                    case COBRA:
                        e_info->x_frameskip = E_FRAMSKIP_COBRA;
                        e_info->wait = enemy_random_30_100 + 100u;
                        EattackerAnim(s_enemy, new_e_state);
                    break;
                    case SPIDER:
                        e_info->x_frameskip = E_FRAMSKIP_SPIDER;
                        e_info->wait = enemy_random_30_100 + 100u;
                        EthrowerAnim(s_enemy, new_e_state);
                    break;
                    case TARANTULA:
                        e_info->x_frameskip = E_FRAMSKIP_SPIDER;
                        e_info->wait = enemy_random_30_100 + 100u;
                        EthrowerAnim(s_enemy, new_e_state);
                    break;
                }
            break;
            case ENEMY_IDLE:
                switch(e_info->type){
                    case SNAKE: SetSpriteAnim(s_enemy, snake_anim_walk, 12u); break;
                    case RAT:SetSpriteAnim(s_enemy, rat_anim_walk, 12u); break;
                    case PORCUPINE: EattackerAnim(s_enemy, new_e_state); break;
                    case COBRA: EattackerAnim(s_enemy, new_e_state); break;
                    case SPIDER: EthrowerAnim(s_enemy, new_e_state); break;
                    case TARANTULA: EthrowerAnim(s_enemy, new_e_state); break;
                }
            break;
            case ENEMY_WAIT:
                switch(e_info->type){
                    case SNAKE: SetSpriteAnim(s_enemy, snake_anim_idle, 16u); break;
                    case RAT: SetSpriteAnim(s_enemy, rat_anim_idle, 16u); break;
                    case PORCUPINE: EattackerAnim(s_enemy, new_e_state); break;
                    case COBRA: EattackerAnim(s_enemy, new_e_state); break;
                    case SPIDER: EthrowerAnim(s_enemy, new_e_state); break;
                    case TARANTULA: EthrowerAnim(s_enemy, new_e_state); break;
                }
                e_info->wait = 40u;
            break;
            case ENEMY_HIT: 
                switch(e_info->type){
                    case SNAKE: SetSpriteAnim(s_enemy, snake_anim_hit, 16u); break;
                    case RAT: SetSpriteAnim(s_enemy, rat_anim_hit, 16u); break;
                    case PORCUPINE: EattackerAnim(s_enemy, new_e_state); break;
                    case COBRA: EattackerAnim(s_enemy, new_e_state); break;
                    case SPIDER: EthrowerAnim(s_enemy, new_e_state); break;
                    case TARANTULA: EthrowerAnim(s_enemy, new_e_state); break;
                }
                e_info->hp--;
                e_info->wait = 56u;
                if(e_info->hp <= 0u){
                    changeEstate(s_enemy, ENEMY_DEAD);
                    return;
                }
            break;
            case ENEMY_DEAD:
                spawnItem(s_enemy->x,  s_enemy->y, s_enemy->type);
                e_info->wait = 24u;
                switch(e_info->type){
                    case SNAKE: SetSpriteAnim(s_enemy, snake_anim_hit, 32u); break;
                    case RAT: SetSpriteAnim(s_enemy, rat_anim_hit, 32u); break;
                    case PORCUPINE: EattackerAnim(s_enemy, new_e_state); break;
                    case COBRA: EattackerAnim(s_enemy, new_e_state); break;
                    case SPIDER: EthrowerAnim(s_enemy, new_e_state); break;
                    case TARANTULA: EthrowerAnim(s_enemy, new_e_state); break;
                }
            break;
            case ENEMY_PREATTACK:
                e_info->wait = 40u;
                switch(e_info->type){
                    case PORCUPINE: EattackerAnim(s_enemy, new_e_state); break;
                    case COBRA: EattackerAnim(s_enemy, new_e_state); break;
                    case SPIDER: EthrowerAnim(s_enemy, new_e_state); break;
                    case TARANTULA: EthrowerAnim(s_enemy, new_e_state);break;
                }
            break;
            case ENEMY_ATTACK:
                if(s_enemy->mirror == NO_MIRROR){e_info->vx = 2*E_VX;}
                else{e_info->vx = 2* (-E_VX);}
                e_info->wait = enemy_random_30_100;
                if(e_info->type == PORCUPINE){EattackerAnim(s_enemy, new_e_state);}
                if(e_info->type == COBRA){EattackerAnim(s_enemy, new_e_state);}
                e_info->x_frameskip = 0u;
            break;
            case ENEMY_THROW:
                Ethrow(s_enemy, e_info->e_state);
            break;
        }
        e_info->e_state = new_e_state;
    }
}

void DESTROY(){
}