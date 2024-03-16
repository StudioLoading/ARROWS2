#include "Banks/SetAutoBank.h"

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
#include "Dialogs.h"

IMPORT_TILES(font);
IMPORT_TILES(minetiles);
IMPORT_MAP(cartmap);
IMPORT_MAP(cartmap2);
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
extern UINT8 tiles_anim_interval;
extern UINT8 timeout_cavesand;
extern struct MISSION get_to_the_mountain;
extern struct MISSION defeat_scorpions;

const UINT8 coll_tiles_cart[] = { 1u, 7u, 3u, 12u, 14u, 17u, 18u, 19u, 21u, 22u, 27u, 28u, 32u, 36u, 40u, 44u, 
56u, 64u, 66u, 67u, 68u, 79u, 80u, 81u, 82u, 83u, 88u, 89u, 90u, 91u, 93u, 94u, 99u, 100u,
109u, 111u, 0};
const UINT8 coll_surface_cart[] = {30u, 0};

extern void UpdateHUD() BANKED;
extern void Log(NPCNAME npcname) BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ReloadEnemiesPL() BANKED;
extern void spawnItem(INVITEMTYPE itemtype, UINT16 spawn_at_x, UINT16 spawn_at_y ) BANKED;

extern UINT16 test_counter;

UINT8 current_cart_map = 0;
UINT8 items_map0 = 10;
UINT8 items_counter = 0u;
UINT8 items_map1 = 10;

void START(){
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;//56u;
        scroll_bottom_movement_limit = 80u;//80u;
    //INIT GRAPHICS
        s_motherpl = SpriteManagerAdd(SpriteCart, (UINT16) 2u << 3, (UINT16) 2u << 3);
    //INIT CHAR & MAP
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, s_motherpl->x + 20u, s_motherpl->y - 16u); 
        if(current_cart_map == 0){
            InitScroll(BANK(cartmap), &cartmap, coll_tiles_cart, coll_surface_cart);    
        }else{
            InitScroll(BANK(cartmap2), &cartmap2, coll_tiles_cart, coll_surface_cart);    
        }        
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
        GetMapSize(BANK(cartmap), &cartmap, &mapwidth, &mapheight);
	SHOW_SPRITES;
    test_counter = 0u;
    tiles_anim_interval = 0u;
    timeout_cavesand = 0u;
    current_cart_map++;
    items_counter = 0u;
}

void UPDATE(){
    //UPDATE HUD for HP changings
        if(hud_motherpl_hp != motherpl_hp){
            UpdateHUD();
        }
    //TILES ANIM
        tiles_anim_interval++;
        switch(tiles_anim_interval){
            case 6u:
                Anim_Cart_1();
            break;
            case 12u:
                Anim_Cart_2();
            break;
            case 18u:
                Anim_Cart_3();
            break;
            case 24u:
                Anim_Cart_4();
            break;
            case 30u:
                Anim_Cart_5();
            break;
            case 36u:
                Anim_Cart_0();
                tiles_anim_interval = 0u;
            break;
        }
    //GO TO INVENTORY
        //if(KEY_PRESSED(J_START)){ChangeState(StateInventory, s_motherpl, -1);}
    //CAMERA MANAGEMENT
        scroll_target->x = s_motherpl->x + 60u;
        scroll_target->y = s_motherpl->y + 32u;
        if(s_motherpl->x > ((mapwidth-2) << 3)){
            if(current_cart_map == 1){//go to second cart map
                ChangeState(StateCart, s_motherpl, 1);
            }else{
                get_to_the_mountain.mission_state = MISSION_STATE_REWARDED;
                defeat_scorpions.mission_state = MISSION_STATE_STARTED;
			    SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
                ChangeState(StateOverworld, s_motherpl, 1);
            }
        }
    //FALLING SAND
        {
            timeout_cavesand--;
            switch(timeout_cavesand){
                case 20u:
                case 40u:
                case 60u:
                    SpriteManagerAdd(SpriteCavesand, s_motherpl->x+200u, s_motherpl->y - 40u);
                break;
                case 100u:
                case 120u:
                case 160u:
                    SpriteManagerAdd(SpriteCavesand, s_motherpl->x+180, s_motherpl->y - 40u);
                break;
                case 180u:
                case 200u:
                case 220u:
                    SpriteManagerAdd(SpriteCavesand, s_motherpl->x+140, s_motherpl->y - 40u);
                break;
                case 0:
                    SpriteManagerAdd(SpriteCavesand, s_motherpl->x+120u, s_motherpl->y - 40u);
                    timeout_cavesand = 240u;
                break;
            }
        }
    //SPAWNING ITEMS
        switch(current_cart_map){
            case 1u:
                if(s_motherpl->x > ((UINT16) 6u << 3) && s_motherpl->x < ((UINT16) 10u << 3) && items_counter == 0){
                    spawnItem(INVITEM_METAL, ((UINT16) 18u << 3), ((UINT16) 8u << 3));
                    items_counter++;
                }else if(s_motherpl->x > ((UINT16) 23u << 3) && s_motherpl->x < ((UINT16) 26u << 3) && items_counter == 1){
                    spawnItem(INVITEM_WOOD, ((UINT16) 31u << 3), ((UINT16) 8u << 3));
                    items_counter++;
                }else if(s_motherpl->x > ((UINT16) 34u << 3) && s_motherpl->x < ((UINT16) 41u << 3) && items_counter == 2){
                    spawnItem(INVITEM_WOOD, ((UINT16) 49u << 3), ((UINT16) 13u << 3));
                    items_counter++;
                }else if(s_motherpl->x > ((UINT16) 50u << 3) && s_motherpl->x < ((UINT16) 54u << 3) && items_counter == 3){
                    spawnItem(INVITEM_METAL, ((UINT16) 62u << 3), ((UINT16) 13u << 3));
                    items_counter++;
                }else if(s_motherpl->x > ((UINT16) 66u << 3) && s_motherpl->x < ((UINT16) 70u << 3) && items_counter == 3){
                    spawnItem(INVITEM_METAL, ((UINT16) 78u << 3), ((UINT16) 14u << 3));
                    items_counter++;
                }else if(s_motherpl->x > ((UINT16) 66u << 3) && s_motherpl->x < ((UINT16) 70u << 3) && items_counter == 3){
                    spawnItem(INVITEM_METAL, ((UINT16) 78u << 3), ((UINT16) 14u << 3));
                    items_counter++;
                }else if(s_motherpl->x > ((UINT16) 160u << 3) && s_motherpl->x < ((UINT16) 164u << 3) && items_counter == 3){
                    spawnItem(INVITEM_WOOD, ((UINT16) 172u << 3), ((UINT16) 14u << 3));
                    items_counter++;
                }
            break;
            case 2u:
                if(s_motherpl->x > ((UINT16) 122u << 3) && s_motherpl->x < ((UINT16) 126u << 3) && items_counter == 0){
                    spawnItem(INVITEM_METAL, ((UINT16) 132u << 3), ((UINT16) 16u << 3));
                    items_counter++;
                }else if(s_motherpl->x > ((UINT16) 130u << 3) && s_motherpl->x < ((UINT16) 132u << 3) && items_counter == 0){
                    spawnItem(INVITEM_WOOD, ((UINT16) 140u << 3), ((UINT16) 14u << 3));
                    items_counter++;
                }else if(s_motherpl->x > ((UINT16) 145u << 3) && s_motherpl->x < ((UINT16) 152u << 3) && items_counter == 0){
                    spawnItem(INVITEM_METAL, ((UINT16) 160u << 3), ((UINT16) 20u << 3));
                    items_counter++;
                }else if(s_motherpl->x > ((UINT16) 155u << 3) && s_motherpl->x < ((UINT16) 162u << 3) && items_counter == 0){
                    spawnItem(INVITEM_WOOD, ((UINT16) 170u << 3), ((UINT16) 20u << 3));
                    items_counter++;
                }else if(s_motherpl->x > ((UINT16) 165u << 3) && s_motherpl->x < ((UINT16) 172u << 3) && items_counter == 0){
                    spawnItem(INVITEM_WOOD, ((UINT16) 180u << 3), ((UINT16) 20u << 3));
                    items_counter++;
                }
            break;
        }
    //BOLTS
        /*
        if(enemy_counter < 2 && s_motherpl->y > 40){
            timeout_enemy++;
            UINT8 timeout_enemy_max = 90u;
            if(s_motherpl->x > ((UINT16) 90u << 3)){
                timeout_enemy_max = 52u;
            }
            if(timeout_enemy == timeout_enemy_max){
                test_counter++;                
                timeout_enemy = 0;
                INT8 delta_for_running = 0;
                if(motherpl_vx != 0){
                    if(motherpl_vx > 0){delta_for_running = 10;}
                    else{delta_for_running = -10;}
                }
                if(s_surf != 0){
                    delta_for_running = +60;
                }
                UINT16 bolt_x = delta_for_running;
                UINT16 bolt_y = s_motherpl->y;
                switch(test_counter){
                    case 6:
                        test_counter = 0;
                        bolt_x = s_motherpl->x + 80u;
                    break;
                    case 5:bolt_x = s_motherpl->x-4u;break;
                    case 4:bolt_x = s_motherpl->x + 40u;break;
                    case 3:bolt_x = s_motherpl->x - 30u;break;
                    case 2:bolt_x = s_motherpl->x + 30u;break;
                    case 1:bolt_x = s_motherpl->x +10u;break;
                }
                SpriteManagerAdd(SpriteBoltground, (UINT16) bolt_x, (UINT16)bolt_y);
            }
        }
        */
    
    Log(NONAME);
}
