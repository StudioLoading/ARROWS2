#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 spider_anim_idle[] = {3, 1, 1, 2}; //The first number indicates the number of frames
const UINT8 spider_anim_walk[] = {7, 1, 2, 6, 3, 1, 2, 5, 4};//The first number indicates the number of frames
const UINT8 spider_anim_hit[] = {2, 1, 0};//{2, 1, 3}; //The first number indicates the number of frames

extern void Estart() BANKED;
extern void configure(struct EnemyData* e_info) BANKED;
extern void ETurn(struct EnemyData* e_info, UINT8 e_vx) BANKED;
extern void changeEstate(struct EnemyData* e_info, ENEMY_STATE new_e_state) BANKED;
extern UINT8 getEmaxFrameskip(ENEMY_TYPE etype) BANKED;
extern void ErandomManagement() BANKED;
extern void Econfiguration(struct EnemyData* eu_info) BANKED;
extern void Egravity() BANKED;
extern void EhorizontalTileCollision(struct EnemyData* eu_info) BANKED;
extern void EspriteCollision(struct EnemyData* eu_info) BANKED;
extern void EstateBehavior(struct EnemyData* eu_info) BANKED;

void START(){
    Estart();
}

void UPDATE(){
    //configuration
        struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
        if(eu_info->configured == 0){
            return;
        }
        Econfiguration(eu_info);
    //CHECK DEATH
        if(eu_info->hp <= 0){changeEstate(eu_info, ENEMY_DEAD);}
    //GRAVITY
        Egravity();
    //TODO check vertical collision
    //HORIZONTAL MAP COLLISION: BACK & FORTH LOGIC
        EhorizontalTileCollision(eu_info);
    //SPRITE COLLISION
        EspriteCollision(eu_info);    
    //STATE BEHAVIOR
        EstateBehavior(eu_info);
}

void DESTROY(){}