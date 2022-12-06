#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define NORMAL_FRAMESKIP 1

void START(){
    struct ArrowData* arrow_data = (struct ArrowData*) THIS->custom_data;
    arrow_data->arrow_fskip = NORMAL_FRAMESKIP;
}

void UPDATE(){
    struct ArrowData* arrow_data = (struct ArrowData*) THIS->custom_data;
    if(arrow_data->vx < 0){
        THIS->mirror = V_MIRROR;
    }else if (arrow_data->vx != 0 && THIS->mirror != NO_MIRROR){
        THIS->mirror = NO_MIRROR;
    }
    if(arrow_data->arrow_fskip){
        arrow_data->arrow_fskip--;
        if(arrow_data->arrow_fskip == 0){
            UINT8 arrow_t_coll = TranslateSprite(THIS, arrow_data->vx << delta_time, 0);
            arrow_data->arrow_fskip = NORMAL_FRAMESKIP;
            if(arrow_t_coll != 0){
                SpriteManagerRemoveSprite(THIS);
            }
        }
    }
}

void DESTROY(){}