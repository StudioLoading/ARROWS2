#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_gocursor[] = {7, 0, 1, 2, 3, 4, 5, 6 };

void START(){
    SetSpriteAnim(THIS,a_gocursor, 16u);
}

void UPDATE(){
}

void DESTROY(){
}