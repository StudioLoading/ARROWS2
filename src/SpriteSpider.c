#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "SpriteManager.h"
#include "Scroll.h"

#include "custom_datas.h"

//PER I SCORPIONI VERI PRENDERE IL SpriteEThrowerSpider.c come template

const UINT8 a_spider[] = {8,0,1,2,3,4,3,2,1}; //The first number indicates the number of frames

void START() {
	SetSpriteAnim(THIS, a_spider, 12u);
    struct EnemyData* spider_data = (struct EnemyData*) THIS->custom_data;
    spider_data->x_frameskip =4;
    spider_data->wait = 0;
}

void UPDATE(){
    struct EnemyData* spider_data = (struct EnemyData*) THIS->custom_data;
    spider_data->wait++;
    if(spider_data->wait >= spider_data->x_frameskip){
        spider_data->wait = 0;
        UINT8 v_coll = TranslateSprite(THIS, 0, 1 << delta_time);
        if(v_coll != 0){
            SpriteManagerRemoveSprite(THIS);
        }
    }
}

void DESTROY(){
    SpriteManagerAdd(SpriteEThrowerSpider, THIS->x, THIS->y - 16u);
}