#include "Banks/SetAutoBank.h"

#include "Palette.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_owcrab[] = {4,0,1,0,2}; //The first number indicates the number of frames

void START(){
	THIS->lim_x = 200u;
	THIS->lim_y = 200u;
    SetSpriteAnim(THIS, a_owcrab, 12u);
    OBP1_REG = PAL_DEF(0, 0, 1, 3);
    SPRITE_SET_PALETTE(THIS,1);
    struct EnemyData* crabow_data = (struct EnemyData*)THIS->custom_data;
    crabow_data->vx = 0;
    crabow_data->configured = 0;
}

void UPDATE(){
    struct EnemyData* crabow_data = (struct EnemyData*)THIS->custom_data;
    if(crabow_data->configured == 2){
        if(crabow_data->vx != 0){
            UINT8 owcrab_t_coll = TranslateSprite(THIS, 0, -1 << delta_time);
            if(owcrab_t_coll == 81u || owcrab_t_coll == 80){
                SpriteManagerRemoveSprite(THIS);
            }
        }
    }
}

void DESTROY(){
}