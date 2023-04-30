#include "Banks/SetAutoBank.h"

#include "savegame.h"
#include "ZGBMain.h"

extern struct MISSION missions[4];
extern struct InvItem inventory[12];
extern struct InvItem itemEquipped;
extern INT8 motherpl_hp;
extern UINT8 previous_state;
Savegame savegame;

UINT8 save_game() BANKED;
UINT8 load_game() BANKED;

UINT8 save_game() BANKED{
    ENABLE_RAM;
    savegame.hp = motherpl_hp;
    savegame.state = previous_state;
    memcpy(&savegame.item, &itemEquipped, sizeof(itemEquipped));
    memcpy(&savegame.diary, &missions, sizeof(missions));
    memcpy(&savegame.bag, &inventory, sizeof(inventory));
    DISABLE_RAM;
    return 1u;
}

UINT8 load_game() BANKED{
    ENABLE_RAM;
    motherpl_hp = savegame.hp;
    previous_state = savegame.state;
    memcpy(&itemEquipped, &savegame.item, sizeof(savegame.item));
    memcpy(&missions, &savegame.diary, sizeof(savegame.diary));
    memcpy(&inventory, savegame.bag, sizeof(savegame.bag));
    DISABLE_RAM;
    return 1u;
}