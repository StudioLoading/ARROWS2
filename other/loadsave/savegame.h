//savegame.h
#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <gb/gb.h>
#include "SRAM.h"
#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"

#include "custom_datas.h"

typedef struct {
	SAVEGAME_HEADER;

    //Whatever content you want to store in external ram
    struct InvItem bag[12];
    struct InvItem item;
    struct MISSION diary[4];
    UINT8 hp;
    UINT8 state;
} Savegame;

extern Savegame savegame;

#endif