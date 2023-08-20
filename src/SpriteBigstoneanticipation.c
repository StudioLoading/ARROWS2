#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern Sprite* s_motherpl;
extern Sprite* scroll_target;

const UINT8 a_anticipation[] = {5,1,2,3,3,0};
const UINT8 a_anticipation_blink[] = {2,3,0};

void START(){
    SetSpriteAnim(THIS,a_anticipation, 12u);
    struct EnemyData* anti_data = (struct EnemyData*) THIS->custom_data;
    anti_data->hp = 2;
    anti_data->wait = 200u;
}

void UPDATE(){
    struct EnemyData* anti_data = (struct EnemyData*) THIS->custom_data;
    if(anti_data->configured == 0){THIS->x = s_motherpl->x + 16u;}
    else{THIS->x = s_motherpl->x - 10u;}
    if(anti_data->hp >= 2){THIS->y = s_motherpl->y - 64u;}
    else{THIS->y = s_motherpl->y - 72u;}
    anti_data->wait--;
    if(anti_data->wait == 0){
        anti_data->hp--;
        switch(anti_data->hp){
            case 1:
                anti_data->wait = 48u;
                SetSpriteAnim(THIS,a_anticipation_blink, 16u);
            break;
            case 0:
                anti_data->et_collision = TranslateSprite(THIS, 0, -1 >> delta_time);
                if(anti_data->et_collision == 0){
                    Sprite* s_bigstone = SpriteManagerAdd(SpriteBigstone, THIS->x, THIS->y);
                    struct EnemyData* bigstone_data = (struct EnemyData*) s_bigstone->custom_data;
                    if(anti_data->configured == 0){bigstone_data->vx = -2;}
                    else{bigstone_data->vx = 2;}
                }
                SpriteManagerRemoveSprite(THIS);
            break;
        }
    }
}

void DESTROY(){
}
