#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "BankManager.h"
#include "Palette.h"
#include "Sound.h"
#include "Music.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Fade.h"

#include "custom_datas.h"
#include "TAnim0.h"
#include "sgb_palette.h"
#include "Dialogs.h"

#define CAMERA_DELTA_RIGHT 32
#define CAMERA_DELTA_LEFT 24

DECLARE_MUSIC(intro);
DECLARE_MUSIC(death);
DECLARE_MUSIC(tutorial);
DECLARE_MUSIC(owsw);
DECLARE_MUSIC(cemetery);
DECLARE_MUSIC(exzoo);
DECLARE_MUSIC(maze);
DECLARE_MUSIC(bosscrab);
DECLARE_MUSIC(mountain);
DECLARE_MUSIC(mine);
DECLARE_MUSIC(missionaccomplished);
DECLARE_MUSIC(letter);
DECLARE_MUSIC(endgame);

IMPORT_MAP(border);
IMPORT_MAP(border2);
IMPORT_MAP(borderdiary);
IMPORT_MAP(bordercave);
IMPORT_MAP(bordermine);
IMPORT_MAP(bordercrab);
IMPORT_MAP(bordersky);
IMPORT_MAP(bordercart);

extern struct InvItem itemEquipped;
extern struct MISSION help_cemetery_woman;
extern struct MISSION enable_hospital;
extern struct MISSION find_antidote;
extern struct MISSION hungry_people;
extern struct MISSION get_to_the_mountain;
extern struct MISSION golden_armor;
extern INT8 motherpl_hp;
extern MOTHERPL_STATE motherpl_state;
extern Sprite* s_surf;
extern UINT8 motherpl_blocked_cooldown;
extern UINT8 motherpl_hit_cooldown;
extern WHOSTALKING whostalking;
extern Sprite* s_motherow;
extern unsigned char log0[];
extern INT8 current_map;
extern UINT8 teleporting;
extern CURRENT_BORDER current_border; 
extern INT8 motherpl_vx;
extern CHAPTERS chapter;
extern UINT8 child_hooked;
extern UINT8 activate_seagulls;
extern struct PushASignData d_push_sign;
extern HOOD_TYPE hood_type;
extern INT8 credit_step;

UINT8 mine_powderspawned = 3u;
UINT8 npc_spawned_zone = 0u;
INT8 np_counter = 0u;
Sprite* s_motherpl = 0;
UINT8 init_enemy = 0u;
INT8 hud_motherpl_hp = 0;
UINT8 camera_ok = 0u;
MirroMode motherpl_mirror = NO_MIRROR; 
UINT16 motherpl_pos_x = 0u;
UINT16 motherpl_pos_y = 0u;
UINT16 motherow_pos_x = 0u;
UINT16 motherow_pos_y = 0u;
struct EtoReload e_to_reload[3];
UINT8 enemy_counter = 0u;
UINT8 mapwidth;
UINT8 mapheight;
UINT8 previous_state;
UINT8 item_spawned_cooldown = 255u;
INT8 sfx_cooldown = 0u;
UINT8 dialog_bg_cooldown = 32u;
UINT8 dialog_bg_activated = 0u;
UINT8 dialog_bg_charcounter = 0u;
UINT8 generic_counter = 0u;
UINT8 just_started = 1u;//=0 to dodge tutorial and ow_tips initial
UINT8 logtimeout = 10u;
UINT8 itemspawned_powder_max = 0;
UINT8 itemspawned_powder_counter = 0;
UINT8 enemy_wave = 0;
Sprite* s_dialog = 0;
struct NpcInfo dialog_icon = {.npcname=NONAME, .whotalks=NOBODY, .type=NOTYPE, .configured=0, .hp=0};
UINT8 child_hooked = 0u;

void UpdateHUD() BANKED;
void Log(NPCNAME npcname) BANKED;
void LogItem(INVITEMTYPE invitemtype) BANKED;
void update_camera_position() BANKED;
void update_camera_position_ow() BANKED;
void camera_tramble() BANKED;
void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos, NPCNAME npcname) BANKED;
void spawn_npa(UINT8 type, UINT16 posx, UINT16 posy, UINT8 configured) BANKED;
void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;
void manage_bgm(UINT8 new_state, UINT8 previous_state, INT8 next_map) BANKED;
void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;
void save_mother_pos(UINT8 sprite_type, UINT16 x, UINT16 y) BANKED;
void check_sgb_palette(UINT8 new_state) BANKED;
void play_music_missionaccomplished() BANKED;
void restartFromHospital() BANKED;
void manage_border(UINT8 my_next_state) BANKED;
void spawn_dialog_icon(Sprite* npc) BANKED;
void ReloadEnemiesPL() BANKED;

extern void ChangeStateThroughBetween(UINT8 new_state) BANKED;


void restartFromHospital() BANKED{
    current_map = MAP_SOUTHWEST;
    motherow_pos_x = (UINT16) 35u << 3;
    motherow_pos_y = (UINT16) 20u << 3;
    previous_state = StateOw;
    ChangeStateThroughBetween(StateHospital);
}

void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED{
    whostalking = whost;
    if(whost != DEATH && whost != FISHERMAN_THERESFISH && whost != FISHERMAN_LETSGO
        && whost != FISHERMAN_FPSGATOR_COMPLETED && whost != OUTWALKER_GUARD_LANDSLIDE){
        if(hungry_people.mission_state > MISSION_STATE_DISABLED &&
            hungry_people.mission_state < MISSION_STATE_ACCOMPLISHED && whost != BRIDGE_BROKEN){
            whostalking = IMHUNGRY;
        }
    }
    ChangeState(StateDialog, s_mother, -1);
}

void play_music_missionaccomplished() BANKED{
    StopMusic;
    PlayMusic(missionaccomplished, 0);
}

void manage_bgm(UINT8 new_state, UINT8 previous_state, INT8 next_map) BANKED{
    if((previous_state == new_state && next_map == current_map) 
        || previous_state == StateInv || new_state == StateInv
        || current_state == StateShop
        || current_state == StateHospital 
        || previous_state == StateDiary || current_state == StateDiary
        || previous_state == StateDialog || current_state == StateDialog
        || previous_state == StateInv || current_state == StateInv
    ){
        return;
    }
    switch(new_state){
        case StateDialog:
            if(previous_state == StateTitle){
                PauseMusic;
            }//else{
                //ResumeMusic;
            //}
            switch(whostalking){
                case INTRO:
                    PlayMusic(letter, 1);
                break;
                case DEATH:
                    PlayMusic(death, 0);
                break;
                case FINAL:
                    PlayMusic(endgame, 0);
                break;
            }
        break;
        case StateOw:
        	StopMusic;
            if(just_started == 0){
                switch(current_map){
                    case MAP_SOUTHWEST:PlayMusic(owsw, 1);break;
                    case MAP_NORTHWEST:PlayMusic(owsw, 1);break;
                    case MAP_MAZE:PlayMusic(maze, 1);break;
                    case MAP_SOUTHEAST:PlayMusic(owsw, 1);break;
                    case MAP_EAST:PlayMusic(owsw, 1);break;
                    case MAP_ISLE:PlayMusic(tutorial, 1);break;
                }
            }            
        break;
        case StateExzoo:
            if(previous_state == StateInv){ResumeMusic;}
            else{StopMusic;PlayMusic(exzoo, 1);}
        break;
        case StateScorpions:
        case StateCemetery:
            if(previous_state == StateInv){ResumeMusic;}
            else if(previous_state != StateDialog){StopMusic;PlayMusic(cemetery, 1);}
        break;
        case StateTutorial:
            PlayMusic(tutorial, 1);
        break;
        case StateBlackiecave:
            if(previous_state == StateInv){ResumeMusic;}
            else if(previous_state != StateDialog){StopMusic;PlayMusic(intro, 1);}
        break;
        case StateBridge:
        case StateHarbor:
        case StateSilvercave:
        case StateMine:
            if(previous_state == StateInv){ResumeMusic;}
            else if(previous_state != StateDialog){StopMusic;PlayMusic(mine, 1);}//bgm_mine
        break;
        case StateHood:
        case StateOutwalkers:
            if(previous_state == StateInv){ResumeMusic;}
            else if(previous_state != StateDialog){StopMusic;PlayMusic(exzoo, 1);}
        break;
        case StateMountain:
            if(previous_state == StateInv){ResumeMusic;}
            else if(previous_state != StateDialog){StopMusic;PlayMusic(mountain, 1);}
        break;
        case StateHospital:
            if(enable_hospital.mission_state == MISSION_STATE_REWARDED
                && motherpl_hp < 5){
                play_music_missionaccomplished();
            }
        break;
        case StateCart:
            if(previous_state == StateInv){ResumeMusic;}
            else if(previous_state != StateDialog){StopMusic;PlayMusic(intro, 1);}
        break;
        case StateFps:
            if(previous_state == StateInv){ResumeMusic;}
            else {StopMusic;PlayMusic(endgame, 1);}
        break;
        case StateBosscrab:
        case StateBossMinotaur:
        case StateBossbat:
            if(previous_state == StateInv){ResumeMusic;}
            else {StopMusic;PlayMusic(bosscrab, 1);}
        break;
    }
}

void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED{
    if(sfx_cooldown == 0){
        PlayFx(c, mute_frames, s0, s1, s2, s3, s4);
        sfx_cooldown = 20u;
    }
}

void save_mother_pos(UINT8 sprite_type, UINT16 x, UINT16 y) BANKED{
    switch(sprite_type){
        case SpriteMotherpl:
        case SpriteMotherplarmor:
            motherpl_pos_x = x;
            motherpl_pos_y = y;
        break;
        case SpriteMotherow:
        case SpriteMotherowarmor:
        case SpriteLiamow:
            motherow_pos_x = x;
            motherow_pos_y = y;
        break;
    }
}

void manage_border(UINT8 my_next_state) BANKED{
    check_sgb_palette(my_next_state);
    switch(my_next_state){
        case StateCredit:
        case StateOw:
        case StateTutorial:
        case StateOutwalkers:
        case StateHospital:
        case StateTetra:
            if(current_border != BORDER_CLASSIC){
                current_border = BORDER_CLASSIC;
                LOAD_SGB_BORDER(border2);
            }
        break;
        case StateMine:
        case StateBatcave:
            if(current_border != BORDER_MINE){
                current_border = BORDER_MINE;
                LOAD_SGB_BORDER(bordermine);
            }
        break;
        case StateBlackiecave:
        case StateBossminotaur:
        case StateSilvercave:
            if(current_border != BORDER_CAVE){
                current_border = BORDER_CAVE;
                LOAD_SGB_BORDER(bordercave);
            }
        break;
        case StatePwd:
            if(current_border != BORDER_DIARY){
                current_border = BORDER_DIARY;
                LOAD_SGB_BORDER(borderdiary);
            }
        break;
        case StateBlackieroom:
            if(current_border != BORDER_BLACKIE){
                current_border = BORDER_BLACKIE;
                LOAD_SGB_BORDER(bordercave);
            }
        break;
        case StateMountain:
        case StateSky:
            if(current_border != BORDER_THUNDER){
                current_border = BORDER_THUNDER;
                LOAD_SGB_BORDER(bordersky);
            }
        break;
        case StateBosscrab:
        case StateHarbor:
        case StateBridge:
        case StateFps:
            if(current_border != BORDER_BOSSCRAB){
                current_border = BORDER_BOSSCRAB;
                LOAD_SGB_BORDER(bordercrab);
            }
        break;
        case StateCart:
            if(current_border != BORDER_CART){
                current_border = BORDER_CART;
                LOAD_SGB_BORDER(bordercart);
            }
        break;
    }
}

void check_sgb_palette(UINT8 new_state) BANKED{
    switch(new_state){
        case StateCredit:
            switch(credit_step){
                case 1://STUDIO LOADING & BN
                case 3://KICKSTARTERS
    		        set_sbg_credit0();
                break;
                case 2://MISTY HILLS & SLOOPYGOOP
                    set_sgb_palette01_MISTYHILLS();
                break;
                case 4://COMUNEKO
    		        set_sgb_palette01_COMUNEKO();
                break;
            }
        break;
        case StateBossminotaur:
        case StateSilvercave:
            set_sgb_palette01_SILVER();
            set_sgb_palette_statusbar();
        break;
        case StateCart:
            set_sgb_palette01_cart();
            set_sgb_palette_statusbar();
        break;
        case StateBosscrab:
        case StateBridge:
        case StateHarbor:
        case StateHospital:
        case StateFps:
            set_sgb_crab();
            set_sgb_palette_statusbar();
        break;
        case StateOutwalkers:
            set_sgb_palette01_OUTWALKER();
            set_sgb_palette_statusbar();
        break;
        case StateScorpions:
        case StateHood:
            set_sgb_palette01_HOOD();
            set_sgb_palette_statusbar();
        break;
        case StateExzoo:
            set_sgb_palette01_ZOO();
            set_sgb_palette_statusbar();
        break;
        case StateSky:
            set_sgb_SKY();
            set_sgb_palette_statusbar();
        break;
        case StateShop:
            set_sgb_palette01_WOLF();
            set_sgb_palette_statusbar();
        break;
        case StateBlackiecave:
        case StateBlackieroom:
            set_sgb_palette01_BLACKIECAVE();
            set_sgb_palette_statusbar();
        break;
        case StateCemetery:
            set_sgb_palette01_CEMATERYCRYPT();
            set_sgb_palette_statusbar();
        break;
        case StateTutorial:
            set_sgb_palette01_TUTORIAL();
            set_sgb_palette_statusbar();
        break;
        case StateMine:
            set_sgb_palette01_MINE();
            set_sgb_palette_statusbar();
        break;
        case StateMountain:
            set_sgb_palette01_MINE();
            set_sgb_palette_statusbar();
        break;
        case StateInv:
            set_sgb_palette_inventory();
            reset_sgb_palette_statusbar();
        break;
        case StatePwd:
        case StateDiary:
            set_sgb_palette01_WOLF();
        break;
        case StateDialog:
        case StateOw:
            if(new_state == StateOw){
                switch(current_map){
                    case MAP_SOUTHWEST:set_sgb_palette01_worldmap();break;//sw
                    case MAP_NORTHWEST:set_sgb_worldmap_nw();break;//nw
                    case MAP_MAZE:set_sgb_palette01_worldmap_maze();break;//maze
                    case MAP_SOUTHEAST:set_sgb_palette02_worldmap();break;//se
                    case MAP_EAST:set_sgb_worldmap_e();break;//e
                }
            }
            reset_sgb_palette_statusbar();
        break;
        case StateBatcave:
            set_sgb_palette01_BATCAVE();
        break;
    }
}

void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED{
    /*if(current_state != StateDialog && current_state != StateCredit){
        PauseMusic;
    }*/
    if(new_state == StateInv || new_state == StateDiary){
        my_play_fx(CHANNEL_2, 60, 0xab, 0xe3, 0x37, 0x87, 0x00);//SFX_START
    }
    enemy_counter = 0;
    np_counter = 0;
    //SAVE ENEMY TO BE RELOADED
    if(new_state == StateInv){
        switch (current_state){
            case StateMine:
            case StateHood:
            case StateSilvercave:
            case StateBatcave:
            {
            UINT8 mfit_a_tile;
            Sprite* mfitspr;
            UINT8 i = 0u;
            SPRITEMANAGER_ITERATE(mfit_a_tile, mfitspr) {
                switch(mfitspr->type){
                    case SpriteEsimplerat: case SpriteEsimplesnake:
                    case SpriteEThrowerScorpion: case SpriteEThrowerSpider:
                    case SpriteEThrowerTarantula: case SpriteEBat:
                    case SpriteEAttackerCobra: case SpriteEAttackerPine:
                    case SpriteSeagull:
                        {
                        struct EnemyData* e_data = (struct EnemyData*) mfitspr->custom_data;
                        if(e_data->type > 0 && e_data->e_state != ENEMY_DEAD && i<3){
                                e_to_reload[i].type = mfitspr->type;
                                e_to_reload[i].x = mfitspr->x;
                                e_to_reload[i].y = mfitspr->y;
                                e_to_reload[i].mirror = mfitspr->mirror;
                                e_to_reload[i].e_data_hp = e_data->hp;
                                e_to_reload[i].e_data_vx = e_data->vx;
                                e_to_reload[i].e_data_wait = e_data->wait;
                                e_to_reload[i].e_data_configured = 4;
                                e_to_reload[i].e_data_type = e_data->type;
                                e_to_reload[i].e_data_et_collision = e_data->et_collision;
                                e_to_reload[i].e_data_e_state = e_data->e_state;
                                e_to_reload[i].e_data_x_frameskip = e_data->x_frameskip;
                            i++;
                        }
                        }
                    break;
                }
            };
            }
            break;
        }
    }
    npc_spawned_zone = 0u;
    if(s_mother == 0){
        if(previous_state == StateOw){s_mother = s_motherow;}
        else{s_mother = s_motherpl;}
    }
    if(new_state != current_state && next_map != current_map){
        save_mother_pos(s_mother->type, s_mother->x, s_mother->y);
    }
    //MOTHERPL POS X Y, HIDE_WIN
        if(current_state != StateInv && current_state != StateDiary
            && current_state != StateDialog && current_state != StatePwd
            && current_state != StateHospital
            && teleporting == 0){
            switch(s_mother->type){
                case SpriteMotherow:
                case SpriteMotherowarmor:
                case SpriteLiamow:
                    switch(new_state){
                        case StateOw:
                            if(current_map == MAP_SOUTHWEST && next_map == MAP_SOUTHEAST){
                                motherow_pos_x = (UINT16) 10u << 3;
                                motherow_pos_y = (UINT16) 23u << 3;
                            }else if(current_map == MAP_SOUTHEAST && next_map == MAP_SOUTHWEST){
                                motherow_pos_x = (UINT16) 44u << 3;
                                motherow_pos_y = (UINT16) 22u << 3;
                            }else if(current_map == MAP_NORTHWEST && next_map == MAP_MAZE){
                                motherow_pos_x = (UINT16) 2u << 3;
                                motherow_pos_y = 20u;
                            }else if(current_map == MAP_MAZE && next_map == MAP_NORTHWEST){
                                motherow_pos_x = (UINT16) 56u << 3;
                                motherow_pos_y = (UINT16) 42u << 3;
                            }else if(next_map == MAP_MAZE){
                                motherow_pos_x = (UINT16) 6u << 3;
                                motherow_pos_y = (UINT16) 3u << 3;
                            }else if(current_map == MAP_SOUTHEAST && next_map == MAP_SOUTHWEST){
                                motherow_pos_x = (UINT16) 43u << 3;
                                motherow_pos_y = (UINT16) 23u << 3;
                            }else if(next_map == MAP_EAST){
                                motherow_pos_x = (UINT16) 26u << 3;
                                motherow_pos_y = ((UINT16) 37u << 3) + 3;
                            }
                        break;
                        case StateBatcave:
                            motherpl_pos_x = (UINT16) 1u << 3;
                            motherpl_pos_y = (UINT16) 6u << 3;
                        break;
                        case StateBridge:
                            if(current_map == MAP_SOUTHEAST){
                                activate_seagulls = 1u;
                            }
                        break;
                    }
                break;
                case SpriteMotherpl:
                case SpriteMotherplarmor:
                    switch(current_state){
                        case StateBlackieroom:
                            if(new_state == StateBlackiecave){
                                motherpl_pos_x = (UINT16)60u << 3;
                                motherpl_pos_y = (UINT16)8u << 3;
                            }
                        break;
                        case StateBridge:
                            if(new_state == StateOw){
                                switch(next_map){
                                    case MAP_EAST: 
                                        activate_seagulls = 0u;
                                        motherow_pos_x = (UINT16) 192u;
                                        motherow_pos_y = (UINT16) 281u;
                                    break;
                                    case MAP_SOUTHEAST:
                                        motherow_pos_x = (UINT16) 30u << 3;
                                        motherow_pos_y = ((UINT16) 36u << 3) + 4u;
                                    break;
                                }
                            }
                        break;
                        case StateHood:
                            if(new_state == StateOw){
                                switch(next_map){
                                    case MAP_SOUTHWEST:
                                        switch(hood_type){
                                            case NORTH_SOUTH:
                                            motherow_pos_x = (UINT16) 19u << 3;
                                            motherow_pos_y = (UINT16) 10u << 3;
                                            break;
                                            case EAST_WEST:
                                            motherow_pos_x = (UINT16) 44u << 3;
                                            motherow_pos_y = (UINT16) 23u << 3;
                                            break;
                                        }
                                    break;
                                    case MAP_NORTHWEST:
                                        motherow_pos_x = (UINT16) 14u << 3;
                                        motherow_pos_y = (UINT16) 46u << 3;
                                    break;
                                    case MAP_SOUTHEAST:
                                        motherow_pos_x = (UINT16) 10u << 3;
                                        motherow_pos_y = (UINT16) 23u << 3;
                                    break;
                                }
                            }
                        break;
                        case StateExzoo:
                            if(new_state == StateOw){
                                //gotta do it cause of the maze
                                motherow_pos_x = ((UINT16) 14u << 3) + 4u;
                                motherow_pos_y = (UINT16) 21u << 3;
                            }
                        break;
                        case StateSky:
                            if(new_state == StateMountain){//falling from the skyss
                                motherpl_pos_x = (UINT16)47u << 3;
                                motherpl_pos_y = (UINT16)5u << 3;
                            }
                        break;
                        case StateMountain:
                            if(new_state == StateOutwalkers){
                                motherpl_pos_x = (UINT16)93u << 3;
                                motherpl_pos_y = (UINT16)11u << 3;
                            }
                        break;
                        case StateBosscrab:
                            motherpl_pos_x = (UINT16) 6u << 3;
                            motherpl_pos_y = (UINT16) 10u << 3;        
                        break;
                        case StateBossscorpion:
                            motherpl_pos_x = (UINT16) 3u << 3;
                            motherpl_pos_y = (UINT16) 12u << 3;        
                        break;
                        case StateBossminotaur:
                            if(new_state == StateSilvercave){
                                motherpl_pos_x = (UINT16) 115u << 3;
                                motherpl_pos_y = (UINT16) 12u << 3; 
                            }
                        break;
                        case StateBatcave:
                            if(new_state == StateOw){
                                motherow_pos_x = (UINT16) 12u << 3;
                                motherow_pos_y = (UINT16) 37u << 3;
                            }
                        break;
                        case StateBossbat:
                            motherpl_pos_x = (UINT16) 16 << 3;
                            motherpl_pos_y = (UINT16) 12u << 3;        
                        break;
                    }
                    motherpl_mirror = s_mother->mirror;
                break;        
            }
            HIDE_WIN;
            SetWindowY(160);
        }else if(current_state == StateDialog){
            if(next_map == MAP_ISLE){
                motherow_pos_x = (UINT16) 20u << 3;
                motherow_pos_y = (UINT16) 11u << 3;
            }
            if(new_state == StateBosscrab || new_state == StateBossscorpion){
                motherpl_pos_x = (UINT16)6u << 3;
                motherpl_pos_y = (UINT16)10u << 3;
            }
        }
        if(just_started == 1){
            motherow_pos_x = ((UINT16) 14u << 3) + 4u;
            motherow_pos_y = (UINT16) 23u << 3;
        }

    //CURRENT_MAP vs NEXT_MAP
        if(next_map != -1){
            current_map = next_map;
        }    
    d_push_sign.collided_tile = 0;
    previous_state = current_state;
    if(previous_state == StateTutorial){
        previous_state = StateExzoo;
    } 
    if(new_state != StateTitle){
	    ChangeStateThroughBetween(new_state);
    }else{
        manage_bgm(new_state, previous_state, next_map);
        SetState(new_state);
    }
}

void UpdateHUD() BANKED{
    UINT8 idx_leftheart = 5;
    UINT8 idx_rightheart = 5;
    INT8 tmp_hp = motherpl_hp;
    //EQUIPPED ITEM
        switch(itemEquipped.itemtype){
            case INVITEM_MONEY:
                UPDATE_HUD_TILE(16,1,0);
                UPDATE_HUD_TILE(17,1,21);
                UPDATE_HUD_TILE(18,1,0);
            break;
	        case INVITEM_ARROW_NORMAL:
                UPDATE_HUD_TILE(16,1,6);
                UPDATE_HUD_TILE(17,1,5);
                UPDATE_HUD_TILE(18,1,17);
            break;
	        case INVITEM_ARROW_PERFO:
                UPDATE_HUD_TILE(16,1,6);
                UPDATE_HUD_TILE(17,1,5);
                UPDATE_HUD_TILE(18,1,18);
            break;
	        case INVITEM_ARROW_BASTARD:
                UPDATE_HUD_TILE(16,1,6);
                UPDATE_HUD_TILE(17,1,5);
                UPDATE_HUD_TILE(18,1,19);
            break;
	        case INVITEM_BOMB:
                UPDATE_HUD_TILE(16,1,6);
                UPDATE_HUD_TILE(17,1,5);
                UPDATE_HUD_TILE(18,1,20);
            break;
        }
        print_target = PRINT_WIN;
        if(itemEquipped.quantity < 10){ PRINT(16,2,"00%i", itemEquipped.quantity); }
        else if(itemEquipped.quantity < 100){ PRINT(16,2,"0%i", itemEquipped.quantity);}
        else {PRINT(16,2,"%i", itemEquipped.quantity);}
    //HP
        hud_motherpl_hp = motherpl_hp;
        for(idx_leftheart=5; idx_leftheart<15 ;idx_leftheart+=2){
            if(tmp_hp > 0){
                UPDATE_HUD_TILE(idx_leftheart,1,7);
                UPDATE_HUD_TILE(idx_leftheart,2,8);
                idx_rightheart++;
                UPDATE_HUD_TILE(idx_rightheart,1,9);
                UPDATE_HUD_TILE(idx_rightheart,2,10);
            }else{
                UPDATE_HUD_TILE(idx_leftheart,1,1);
                UPDATE_HUD_TILE(idx_leftheart,2,2);
                idx_rightheart++;
                UPDATE_HUD_TILE(idx_rightheart,1,3);
                UPDATE_HUD_TILE(idx_rightheart,2,4);
            }
            tmp_hp--;
            idx_rightheart++;
        }

}

void LogItem(INVITEMTYPE invitemtype) BANKED{
    //if(logtimeout > 0){logtimeout--;}
    //if(logtimeout == 0u){
        logtimeout = 90u;
    //}
    GetLocalizedLogItem_EN(invitemtype);
    PRINT(0,0,log0);   
}

void Log(NPCNAME npcname) BANKED{    
    /*switch(motherpl_state){
        case MOTHERPL_IDLE: PRINT(0, 0, "IDLE"); break;
        case MOTHERPL_JUMP: PRINT(0, 0, "JUMP"); break;
        case MOTHERPL_WALK: PRINT(0, 0, "WALK"); break;
        case MOTHERPL_HIT: PRINT(0, 0, " HIT"); break;
        case MOTHERPL_DEAD: PRINT(0, 0, "DEAD"); break;
        case MOTHERPL_CRAWL: PRINT(0, 0, "CRAW"); break;
        case MOTHERPL_CRAWL_SURF: PRINT(0, 0, "CRAS"); break;
        case MOTHERPL_DASH: PRINT(0, 0, "DASH"); break;
    }
        if(s_surf){
            PRINT(5, 0, "SURF%i",motherpl_surf_dx);
        }
        */
        /*else{
            if(hurricane_info->step < 10){
                PRINT(5, 2, "00%u",hurricane_info->step);   
            }else if(hurricane_info->step < 100){
                PRINT(5, 2, "0%u",hurricane_info->step);
            }else{
                PRINT(5, 2, "%u",hurricane_info->step);
            }
        }*/    
        //PRINT(10, 3, "AR:%u%u", arrows_onscreen, 5u);
        //PRINT(16, 0, "!LOG");
    if(logtimeout > 0){logtimeout--;}
    else{GetLocalizedLog_EN();}
    if(npcname != NONAME){
        if(logtimeout == 0u){logtimeout = 60u;}
        GetLocalizedLogName_EN(npcname);
    }
    PRINT(0,0,log0);   
}

void camera_tramble() BANKED{
    UINT8 cooldown_shifted = 1;
    if(motherpl_hit_cooldown > 50){
        cooldown_shifted = motherpl_hit_cooldown % 4;
    }
    else if(motherpl_hit_cooldown > 17){
        cooldown_shifted = motherpl_hit_cooldown % 6;
    } else {
        cooldown_shifted = motherpl_hit_cooldown % 8;
    }
    if(cooldown_shifted == 0){
        if(scroll_target->y > s_motherpl->y){
            scroll_target->y = ((UINT16)s_motherpl->y) - 6u;
        }else{
            scroll_target->y = ((UINT16)s_motherpl->y) + 20u;
        }
    }
}

void update_camera_position() BANKED{
    //FIXED CAMERA
        switch(current_state){
            case StateBossminotaur:
            case StateBlackieroom:
                if(scroll_target->x != (UINT16) 80u || 
                    scroll_target->y != (UINT16) 56u){
                    scroll_target->x = (UINT16) 80u;
                    scroll_target->y = (UINT16) 56u;
                }
                return;
            break;
            case StateBossbat:
                scroll_target->x = ((UINT16) 12u << 3);
                scroll_target->y = ((UINT16) 10u << 3);
            break;
        }
    //LIMITS
        //HORIZONTAL
            //LEFT
            if(s_motherpl->x < (UINT16)8u){
                s_motherpl->x = 8u;
                INT8 next_ = -1;
                switch(current_state){
                    case StateHood:
                        if(child_hooked == 1){
                        // && help_cemetery_woman.current_step == 2){
                            help_cemetery_woman.current_step = 4;
                        }
                    case StateExzoo:
                        next_ = MAP_SOUTHWEST;
                        ChangeState(StateOw, s_motherpl, MAP_SOUTHWEST);
                    break;
                    case StateSky:
                        ChangeState(StateMountain, s_motherpl, -1);
                    break;
                    case StateMountain:
                        ChangeState(StateOutwalkers, s_motherpl, -1);
                    break;
                    case StateSilvercave:
                        next_ = MAP_EAST;
                        ChangeState(StateOw, s_motherpl, MAP_EAST);
                    break;
                    case StateBridge:
                        next_ = MAP_SOUTHEAST;
                    default:
                        ChangeState(StateOw, s_motherpl, next_);
                    break;
                }
            }
            //RIGHT
            if(s_motherpl->x > (((UINT16)mapwidth) << 3) - 16u){
                s_motherpl->x = (((UINT16)mapwidth) << 3) - 16u;
                switch(current_state){
                    case StateHood:
                        if(help_cemetery_woman.current_step < 3u){
                        }else{//exiting hoods to the north
                            motherow_pos_x = 0u;
                            motherow_pos_y = 0u;
                            switch(hood_type){
                                case NORTH_SOUTH:
                                    ChangeState(StateOw, s_motherpl, 1);
                                break;
                                case EAST_WEST:
                                    ChangeState(StateOw, s_motherpl, 3);
                                break;
                            }
                        }
                    break;
                    case StateMountain:
                        ChangeState(StateSky, s_motherpl, -1);
                    break;
                    case StateBridge:
                        golden_armor.current_step = 2;
                        ChangeState(StateOw, s_motherpl, 4);
                    break;
                    case StateOutwalkers:
                        if(get_to_the_mountain.mission_state >= MISSION_STATE_ACCOMPLISHED){
                            trigger_dialog(OUTWALKER_GUARD_LANDSLIDE, s_motherpl);
                        }else{
                            ChangeState(StateMountain, s_motherpl, -1);
                        }
                    break;
                    case StateSky:
                        trigger_dialog(IBEX_GIVE_MISSION, s_motherpl);
                    break;
                    case StateSilvercave:
                        s_motherpl->x -= 8u;
                        if(golden_armor.mission_state < MISSION_STATE_ACCOMPLISHED){
                            trigger_dialog(MINOTAUR_ENTRANCE, s_motherpl);
                        }else{
                            ChangeState(StateBossminotaur, s_motherpl, -1);
                        }
                    break;
                    case StateBatcave:
                            motherow_pos_x = 10u;
                            motherow_pos_y = 37u;
                            ChangeState(StateOw, s_motherpl, 3);
                    break;
                    default:
                        ChangeState(StateOw, s_motherpl, -1);
                    break;
                }
            }
        //VERTICAL
            if(s_motherpl->y > (((UINT16) mapheight) << 3) && current_state != StateCart){
                s_motherpl->y = ((UINT16) mapheight) - 32u;
            }
            if(s_motherpl->y > 60000u){
                s_motherpl->y = 32u;
            }  
    //SCROLL TARGET Y    
        if(motherpl_hit_cooldown > 0){
            //CAMERA TRAMBLE
            camera_tramble();
        }else{
            if(scroll_target->y != (s_motherpl->y + 16u)){
                scroll_target->y = s_motherpl->y + 16u;
            }
        }
    //BLOCK 
        if(motherpl_state == MOTHERPL_BLOCKED ){return;}
        if(motherpl_blocked_cooldown > 0u){motherpl_blocked_cooldown--;return;}
        //in ogni caso non uscire dai margini
        if(s_surf){
            switch(s_motherpl->mirror){
                case NO_MIRROR:
                    scroll_target->x = s_motherpl->x + CAMERA_DELTA_RIGHT;
                break;
                case V_MIRROR:
                    scroll_target->x = s_motherpl->x - CAMERA_DELTA_LEFT;
                break;
            }
            return;
        }
        if((KEY_PRESSED(J_DOWN) && motherpl_state != MOTHERPL_JUMP) 
            || motherpl_state == MOTHERPL_PICKUP){
                camera_ok = 0u;
        }
        if(KEY_PRESSED(J_RIGHT) || KEY_PRESSED(J_LEFT) || motherpl_state == MOTHERPL_DASH
            || current_state == StateCart){
            if(camera_ok == 1u){
                switch(s_motherpl->mirror){
                    case NO_MIRROR:
                        scroll_target->x = s_motherpl->x + CAMERA_DELTA_RIGHT;
                    break;
                    case V_MIRROR:
                        scroll_target->x = s_motherpl->x - CAMERA_DELTA_LEFT;
                    break;
                }
            }else{
                switch(s_motherpl->mirror){
                    case NO_MIRROR://going right
                        if (scroll_target->x < (s_motherpl->x + CAMERA_DELTA_RIGHT)){
                            if(motherpl_vx > 0 || KEY_PRESSED(J_RIGHT)){
                                scroll_target->x+=3;
                            }
                        }else if (!KEY_PRESSED(J_LEFT) 
                            && !KEY_PRESSED(J_DOWN)){
                            camera_ok = 1u;
                        }
                    break;
                    case V_MIRROR://going left
                        if(scroll_target->x > (s_motherpl->x - CAMERA_DELTA_LEFT)){
                            if(motherpl_vx < 0 || KEY_PRESSED(J_LEFT)){
                                scroll_target->x-=3;
                            }
                        }else if (!KEY_PRESSED(J_RIGHT) && !KEY_PRESSED(J_DOWN)){
                            camera_ok = 1u;
                        }
                    break;
                }
            }
        }
}

void ReloadEnemiesPL() BANKED{    
    init_enemy = 0u;
    enemy_counter = 0u;
    UINT8 i = 0u;
    if(previous_state == StateInv || previous_state == StateDialog){
        for(i = 0u; i < 3u && e_to_reload->e_data_hp > 0; ++i){
            Sprite* s_enemy = SpriteManagerAdd(e_to_reload[i].type, e_to_reload[i].x, e_to_reload[i].y);
            struct EnemyData* s_enemy_data = (struct EnemyData*) s_enemy->custom_data;
            s_enemy_data->hp = e_to_reload[i].e_data_hp;
            s_enemy_data->vx = e_to_reload[i].e_data_vx;
            s_enemy_data->wait = e_to_reload[i].e_data_wait;
            s_enemy_data->configured = e_to_reload[i].e_data_configured;
            s_enemy_data->type = e_to_reload[i].e_data_type;
            s_enemy_data->et_collision = e_to_reload[i].e_data_et_collision;
            s_enemy_data->e_state = e_to_reload[i].e_data_e_state;
            s_enemy_data->x_frameskip = e_to_reload[i].e_data_x_frameskip;
            s_enemy->mirror = e_to_reload[i].mirror;
            INT16 delta_x = s_enemy->x - s_motherpl->x;
            if(delta_x > 80 || delta_x < -80){
                SpriteManagerRemoveSprite(s_enemy);
            }else{
                enemy_counter++;
            }
            e_to_reload[i].type = 0;
            e_to_reload[i].x = 0;
            e_to_reload[i].y = 0;
            e_to_reload[i].e_data_hp=0;
            e_to_reload[i].e_data_vx=0;
            e_to_reload[i].e_data_wait=0;
            e_to_reload[i].e_data_configured=0;
            e_to_reload[i].e_data_type=0;
            e_to_reload[i].e_data_et_collision=0;
            e_to_reload[i].e_data_e_state=0;
            e_to_reload[i].e_data_x_frameskip=0;
        }
    }
}

//spawn non playable character
void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos, NPCNAME npcname) BANKED{
    if(type == SpritePgCadaver){
        Sprite* s_body = SpriteManagerAdd(type, posx, posy);
        s_body->mirror = mirror;
        struct NpcInfo* body_data = (struct NpcInfo*) s_body->custom_data;
        body_data->type = body;
        body_data->whotalks = whos;
        body_data->npcname = npcname;
        return;
    }
    if(np_counter > 6){
        return;
    }
    if(chapter == CHAPTER_2_PLAGUE && find_antidote.mission_state < MISSION_STATE_REWARDED){
        if(npcname != OUTWALKER_JESSICA && whos != JESSICA_PLANTS){
            return;
        }
    }
    np_counter++;
    Sprite* s_head = SpriteManagerAdd(type, posx, posy);
    s_head->mirror = mirror;
    struct NpcInfo* head_data = (struct NpcInfo*) s_head->custom_data;
    head_data->type = head;
    head_data->whotalks = whos;
    head_data->npcname = npcname;
    Sprite* s_body = SpriteManagerAdd(type, posx, posy+16u);
    s_body->mirror = mirror;
    struct NpcInfo* body_data = (struct NpcInfo*) s_body->custom_data;
    body_data->type = body;
    body_data->whotalks = whos;
    body_data->npcname = npcname;
    head_data->configured = 1u;
    body_data->configured = 1u;
}

//spanw non playable animal
void spawn_npa(UINT8 type, UINT16 posx, UINT16 posy, UINT8 configured) BANKED{
    if(np_counter > 6){
        return;
    }
    np_counter++;
    Sprite* s_birdsky = SpriteManagerAdd(type, posx, posy);    
    struct EnemyData* birdsky_info = (struct EnemyData*) s_birdsky->custom_data;
    birdsky_info->configured = configured;
}

void spawn_dialog_icon(Sprite* npc) BANKED{
    struct NpcInfo* npc_data = (struct NpcInfo*) npc->custom_data;
    switch(npc_data->type){
        case WOMAN_BODY1: case WOMAN_BODY2: case WOMAN_BODY3: 
        case MAN_BODY1: case MAN_BODY2: case PIRATE_CAPTAIN:
	    case PIRATE_PANZONE: case PIRATE_SPUGNA: case PIRATE_MARINAIO:
	    case PIRATE_WALKER:
            if(dialog_icon.hp == 0){
                s_dialog = SpriteManagerAdd(SpriteInvcursor, npc->x - 4, npc->y - 24);
                struct ItemSpawned* incursor_data = (struct ItemSpawned*)s_dialog->custom_data;
                incursor_data->configured = 1;
                dialog_icon.hp = 40;
            } 
        break;
    }
}

void START(){}

void UPDATE(){}