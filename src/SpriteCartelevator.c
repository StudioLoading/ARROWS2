#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_cartelevator[] = {4, 0, 1, 0, 2};

void START(){
    SetSpriteAnim(THIS,a_cartelevator, 32u);
    struct EnemyData* elev_data = (struct EnemyData*) THIS->custom_data; 
    elev_data->e_state = ENEMY_SLIDE_UP;
    elev_data->wait = 0u;
}

void UPDATE(){
    struct EnemyData* elev_data = (struct EnemyData*) THIS->custom_data; 
    switch(elev_data->e_state){
        case ENEMY_SLIDE_UP:
            THIS->y--;
        break;
        case ENEMY_SLIDE_DOWN:
            elev_data->wait++;
            if(elev_data->wait == 2){
                elev_data->wait = 0;
                THIS->y++;
            }
        break;
    }
}

void DESTROY(){
}
