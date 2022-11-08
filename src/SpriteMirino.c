#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"


void START(){
    struct MirinoInfo* this_mirino_info = (struct MirinoInfo*)THIS->custom_data;
    this_mirino_info->target = 0;
}

void UPDATE(){
    
    struct MirinoInfo* this_mirino_info = (struct MirinoInfo*)THIS->custom_data;
    if(this_mirino_info->on_target == 0){
        this_mirino_info->target = 0;
    }

}

void DESTROY(){
    
}