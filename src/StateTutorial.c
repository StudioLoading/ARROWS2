#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "BankManager.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "Dialogs.h"

IMPORT_MAP(border2);
IMPORT_TILES(font);
IMPORT_TILES(exzootiles);
IMPORT_MAP(tutorialmap);
IMPORT_MAP(hudpl);

extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern INT8 motherpl_hp;
extern INT8 hud_motherpl_hp;
extern Sprite* s_motherpl;
extern UINT8 init_enemy;
extern UINT8 MAX_ENEMY;
extern UINT8 mapwidth;
extern UINT8 mapheight;
extern UINT8 previous_state;
extern UINT16 motherpl_pos_x;
extern UINT16 motherpl_pos_y;
extern MirroMode motherpl_mirror; 
extern INT8 motherpl_vx;
extern UINT8 npc_spawned_zone;
extern UINT8 generic_counter;
extern struct MISSION help_cemetery_woman;
extern WHOSTALKING whostalking;
extern UINT16 timeout_enemy;
extern UINT8 enemy_counter;
extern Sprite* s_surf;
extern Sprite* inv_cursor;

UINT8 tutorial_current_step = 0;
UINT8 tutorial_counter = 0;

extern unsigned char EMPTY_STRING_21[];
extern unsigned char d1[];
extern unsigned char d2[];
extern unsigned char d3[];
extern unsigned char d4[];
extern unsigned char d5[];
extern unsigned char d6[];
extern unsigned char d7[];
extern unsigned char d8[];
extern unsigned char d9[];
extern unsigned char d10[];
extern unsigned char d11[];

const UINT8 coll_tiles_tutorial[] = {5u, 7u, 9u, 10u, 14u, 17u, 18u, 19u, 28u, 48u, 88u, 89u, 90u, 0};
const UINT8 coll_surface_tutorial[] = {1u, 27u, 0};

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void update_camera_position() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;

void show_current_step_instructions() BANKED;
void empty_instructions() BANKED;

void START(){
    LOAD_SGB_BORDER(border2);
    //SOUND
        NR52_REG = 0x80; //Enables sound, you should always setup this first
        NR51_REG = 0xFF; //Enables all channels (left and right)
        NR50_REG = 0x77; //Max volume
	//SCROLL LIMITS
        /*
        scroll_top_movement_limit = 56u;
        scroll_bottom_movement_limit = 80u;
        */
    //INIT GRAPHICS
        s_motherpl = SpriteManagerAdd(SpriteMotherpl, (UINT16) 3u << 3, (UINT16) 1u << 3);
        if(previous_state == StateInventory || previous_state == StateDialog) {
            s_motherpl->x = motherpl_pos_x;
            s_motherpl->y = motherpl_pos_y;
            s_motherpl->mirror = motherpl_mirror;
        }
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, 80u, 72u); 
        InitScroll(BANK(tutorialmap), &tutorialmap, coll_tiles_tutorial, coll_surface_tutorial);    
    //HUD
        INIT_FONT(font, PRINT_BKG);
        INIT_HUD(hudpl);
        hud_motherpl_hp = 0;
        UpdateHUD();
    //RELOAD ENEMIES
        enemy_counter = 0u;
        timeout_enemy = 0;
        ReloadEnemiesPL();
    //GET MAP DIMENSIONS
        GetMapSize(BANK(tutorialmap), &tutorialmap, &mapwidth, &mapheight);
	SHOW_SPRITES;
    //empty_instructions();
    tutorial_counter = 0;
    show_current_step_instructions();
}

void UPDATE(){
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //GO TO INVENTORY
        //if(KEY_PRESSED(J_START)){ChangeState(StateInventory, s_motherpl, -1);}
    //COUNTER
        if(tutorial_counter > 0){
            tutorial_counter--;
            if(tutorial_counter == 80){
                show_current_step_instructions();
            }
            if(tutorial_counter == 1u){
                inv_cursor = SpriteManagerAdd(SpriteStartbtn, 136u, (UINT16)100u);
            }
        }else{//tutorial_counter == 0
            if(KEY_TICKED(J_START)){
                tutorial_current_step++;
                empty_instructions();
                tutorial_counter = 120u;
                if(tutorial_current_step > 8u){
                    previous_state = StateExzoo;
                    ChangeState(StateInventory, s_motherpl, -1);
                }
            }            
            if(KEY_TICKED(J_SELECT)){
                tutorial_current_step = 0;
                empty_instructions();
                tutorial_counter = 120u;
            }
        }

    Log(NONAME);
}

void empty_instructions() BANKED {
    SpriteManagerRemoveSprite(inv_cursor);
    PRINT(0, 9, EMPTY_STRING_21);
    PRINT(0, 10, EMPTY_STRING_21);
    PRINT(0, 11, EMPTY_STRING_21);
    PRINT(0, 12, EMPTY_STRING_21);
    PRINT(0, 13, EMPTY_STRING_21);
    PRINT(0, 14, EMPTY_STRING_21);
    PRINT(0, 15, EMPTY_STRING_21);
    PRINT(0, 16, EMPTY_STRING_21);
}

void show_current_step_instructions() BANKED{
    print_target = PRINT_BKG;
    switch(tutorial_current_step){
        case 0u:
            PRINT(0, 9,  "HI! MY NAME IS DESSA\0");
            PRINT(0, 10, "AND TOGETHER WE ARE \0");
            PRINT(0, 11, "GOING TO SAVE MY SON\0");
            PRINT(0, 12, EMPTY_STRING_21);
            PRINT(0, 13, "WHEN YOU ARE READY  \0");
            PRINT(0, 14, "      PRESS START   \0");
        break;
        case 1u:
            PRINT(0, 9,  "GOOD! NOW MOVE ME   \0");
            PRINT(0, 10, "LEFT AND RIGHT WITH \0");
            PRINT(0, 11, "YOUR D PAD.         \0");
            PRINT(0, 12, EMPTY_STRING_21);
            PRINT(0, 13, EMPTY_STRING_21);
        break;
        case 2u:
            PRINT(0, 9,  "GOOD! NOW MAKE ME   \0");
            PRINT(0, 10, "JUMP HIGH, UP ON    \0");
            PRINT(0, 11, "THESE BLOCKS. PRESS \0");
            PRINT(0, 12, "AND HOLD JUMP BUTTON\0");
        break;
        case 3u:
            PRINT(0, 9,  "ALRIGHT! NOW MAKE ME\0");
            PRINT(0, 10, "DASH UNDER THE LOW  \0");
            PRINT(0, 11, "EST BLOCKS. DO IT BY\0");
            PRINT(0, 12, "HOLDING DOWN AND    \0");
            PRINT(0, 13, "PRESS FIRE.         \0");
        break;
        case 4u:
            PRINT(0, 9,  "PRESS SELECT TO CHAN\0");
            PRINT(0, 10, "GE THE EQUIPPED ITEM\0");
            PRINT(0, 11, "LOOK TO THE BOTTOM  \0");
            PRINT(0, 12, "RIGHT OF THE SCREEN \0");
            PRINT(0, 13, "TO CHECK ITEM IN USE\0");
        break;
        case 5u:
            PRINT(0, 9,  "PRESS THE FIRE      \0");
            PRINT(0, 10, "BUTTON TO SHOOT ONE \0");
            PRINT(0, 11, "ARROW AT A TIME.    \0");
            PRINT(0, 12, EMPTY_STRING_21);
            PRINT(0, 13, EMPTY_STRING_21);
        break;
        case 6u:
            PRINT(0, 9,  "SHOOT AT THE BLOCKS \0");
            PRINT(0, 10, "AND SEE WHAT        \0");
            PRINT(0, 11, "HAPPENS.            \0");
            PRINT(0, 12, EMPTY_STRING_21);
            PRINT(0, 13, EMPTY_STRING_21);
        break;
        case 7u:
            PRINT(0, 9,  "AS LIAM IS I AM ABLE\0");
            PRINT(0, 10, "TO RIDE THE ARROWS I\0");
            PRINT(0, 11, "SHOOT. TRY JUMPING  \0");
            PRINT(0, 12, "OVER THEM.          \0");
            PRINT(0, 13, EMPTY_STRING_21);
        break;
        case 8u:
            PRINT(0, 9,  "ALRIGHT! I THINK WE \0");
            PRINT(0, 10, "ARE READY TO START  \0");
            PRINT(0, 11, "OUR JOURNEY. LOOK AT\0");
            PRINT(0, 12, "MY INVENTORY WITH   \0");
            PRINT(0, 13, "THE START BUTTON    \0");
        break;
    }
}
