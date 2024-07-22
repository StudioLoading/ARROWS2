#ifndef CUSTOMDATAS_FILE
#define CUSTOMDATAS_FILE

typedef enum{
	BORDER_NONE,
	BORDER_CLASSIC,
	BORDER_DIARY,
	BORDER_CAVE,
	BORDER_MINE,
	BORDER_SKY,
	BORDER_BLACKIE,
	BORDER_BOSSCRAB,
	BORDER_THUNDER,
	BORDER_CART
}CURRENT_BORDER;

struct FlipperballInfo{
	INT8 impulse_y;
	INT8 impulse_x;
	INT8 vx;
	INT8 vy;
};

typedef enum{
	IDLE,
	JUST_THROWN,
	GOING,
	FAR,
	STRIKE
}PUNTA_STATE;

struct PuntaInfo{
	INT8 vy;
	PUNTA_STATE punta_state;
};

typedef enum{
	TARGET_UNINIT,
	TARGET_INIT_PALLONCINO,
	TARGET_IDLE_PALLONCINO,
	TARGET_INIT_AXE,
	TARGET_IDLE_AXE,
	TARGET_STROKE
}TARGET_STATE;

struct TargetInfo{
	INT8 vx;
	INT8 vy;
	INT8 enabled;
	TARGET_STATE target_state;
	UINT8 wait;
};

struct MirinoInfo{
	Sprite* target;
	INT8 on_target;
};

struct PuffInfo{
	INT8 puff_counter;
};

typedef enum{
	DADO_INVISIBLE,
	DADO_WAITING,
	ROLLING_SUPERFAST,
	ROLLING_FAST,
	ROLLING_SLOW,
	ANTICIPATION,
	DADO_FACE,
	DADO_SELECTED_PLAYER,
	DADO_SELECTED_ENEMY,
	DADO_GOING_PLAYER,
	DADO_GOING_ENEMY,
	PASSWORD
}TETRA_DADO_STATE;

typedef enum{
	FACCIA_1,
	FACCIA_2,
	FACCIA_3,
	FACCIA_4
}TETRA_DADO_FACCE;

struct TetradadoInfo{
	TETRA_DADO_STATE tetradado_state;
	TETRA_DADO_FACCE tetradado_faccia;
	UINT8 rolling_counter;
	UINT8 face_counter;
	UINT8 initial_x;
	UINT8 initial_y;
};

typedef enum{
	CURSOR_INVISIBLE,
	TRIANGLE_BLINK,
	TRIANGLE_FULL,
	TRIANGLE_EMPTY,
	HAND_OPENED,
	HAND_CLOSED
}TETRACURSOR_STATE;

struct TetracursorInfo{
	TETRACURSOR_STATE cursor_state;
};

typedef enum{
  INIT_GAME,
  START_GAME,
  RETURN_TO_PLACE,
  TURN_MAKE_DICE,
  TURN_PICK_DICE,
  TURN_GIVE_DICE,
  TURN_PICK_DRAGON,
  TURN_E_PICK_DICE,
  TURN_E_PICK_DRAGON,
  WINNER,
  LOSER
}TETRA_GAME_STATE;

typedef enum{
  TURN_PLAYER,
  TURN_ENEMY
}TETRA_TURN;

typedef enum{
	GENERIC_IDLE,
	GENERIC_WALK,
	IDLE_UP,
	IDLE_DOWN,
	IDLE_LEFT,
	IDLE_RIGHT,
	WALK_LEFT,
	WALK_RIGHT,
	WALK_UP,
	WALK_DOWN,
	JUMP,
	ATTACK,
	DIE
}FA2OW_SPRITE_STATES;

struct OwSpriteInfo{
	FA2OW_SPRITE_STATES ow_state;
	UINT8 tile_collision;
	INT8 vx;
	INT8 vy;	
};

typedef enum{
	OW_DIARY,
	OW_GAMEOPT,
	OW_GAMEOPT_SAVE,
	OW_GAMEOPT_LOAD
}HUD_OPTION;

typedef enum{
	MISSION_STATE_DISABLED,
	MISSION_STATE_ENABLED,
	MISSION_STATE_STARTED,
	MISSION_STATE_ACCOMPLISHED,
	MISSION_STATE_REWARDED
}MISSION_STATE;

typedef enum{
	FIND_BLACKIE,
	ENABLE_HOSPITAL,
	ENGAGE_SMITH,
	HELP_CEMATERY_WOMAN,
	HELP_HOSPITAL,
	OUTWALKER_CHIEF,
	OUTWALKER_GLASSES,
	OUTWALKER_SMITH,
	GET_TO_THE_MOUNTAIN,
	DEFEAT_SCORPIONS,
	FIND_ANTIDOTE,
	HUNGRY_PROPLE,
	FIX_BRIDGE,
	GOLDEN_ARMOR
}MISSION_TITLE;

struct MISSION{
	MISSION_TITLE mission_title;
	MISSION_STATE mission_state;
	UINT8 current_step;
	UINT8 reward_quantity;
	UINT8 phase;
};

typedef enum{
	MOTHERPL_IDLE,
	MOTHERPL_JUMP,
	MOTHERPL_WALK,
	MOTHERPL_HIT,
	MOTHERPL_DEAD,
	MOTHERPL_CRAWL,
	MOTHERPL_CRAWL_SURF,
	MOTHERPL_BLOCKED,
	MOTHERPL_PICKUP,
	MOTHERPL_DASH,
	MOTHERPL_ASCENDING
}MOTHERPL_STATE;

struct MotherplData{
	INT8 accel_y;
	MOTHERPL_STATE mpl_state;
};

typedef enum{
	UNASSIGNED,
	SNAKE,
	RAT,
	PORCUPINE,
	COBRA,
	SPIDER,
	TARANTULA,
	SCORPIO_HEAD,
	SCORPIO_BODY,
	GULL
}ENEMY_TYPE;

typedef enum{
	T_UNASSIGNED,
	WEB,
	ACID,
	PROJECTILE,
	T_DESTROYING,
	T_DESTROYED
}ENEMYTHROWABLE_TYPE;

typedef enum{
	ENEMY_IDLE,
	ENEMY_JUMP,
	ENEMY_WALK,
	ENEMY_WAIT,
	ENEMY_HIT_1,
	ENEMY_HIT_2,
	ENEMY_DEAD,
	ENEMY_TREMBLING,
	ENEMY_PREATTACK,
	ENEMY_ATTACK,
	ENEMY_THROW,
	ENEMY_UPSIDEDOWN,
	ENEMY_JUMP_DESCENDING,
	ENEMY_SLIDE_DOWN,
	ENEMY_SLIDE_DOWN2,
	ENEMY_SLIDE_UP,
	ENEMY_SLIDE_UP2,
	ENEMY_RUN
}ENEMY_STATE;

struct EtoReload{
    UINT8 type;
	UINT16 x;
	UINT16 y;
	UINT8 alive;
};

struct PushASignData{
	UINT16 collided_tile;
};

struct EnemyData{
	INT8 hp;
	INT8 vx;
	UINT8 wait;
	UINT8 configured;
	//0 da configurare
	//1 in configurazione
	//2 non ancora spawnato item
	//3 già spawnato item
	ENEMY_TYPE type;
	UINT8 et_collision;
	ENEMY_STATE e_state;
	UINT8 x_frameskip;
};

struct ThrowableData{
	INT8 hp;
	INT8 vx;
	INT8 vy;
	UINT8 wait;
	UINT8 x_frameskip;
	ENEMYTHROWABLE_TYPE type;
	UINT8 configured;
};

typedef enum{
	ARROW_UNASSIGNED,
	ARROW_NORMAL,
	ARROW_PERF,
	ARROW_BASTARD,
	ARROW_DESTROYED
}ARROW_TYPE;

struct ArrowData{
	INT8 vx;
	UINT8 configured;
	UINT8 arrow_fskipx;
	ARROW_TYPE arrow_type;
	UINT8 arrow_fskipx_max;
	UINT8 hit;
};

typedef enum{
	INVITEM_MONEY, //COINS
	INVITEM_HEARTS, //FULLHP
	INVITEM_METAL, 
	INVITEM_HEART, //HP+1
	INVITEM_WOOD,//IMPORTANT: da qui in su devono esser quelli spawnati, c'è un random su StateFittizio!
	INVITEM_UNASSIGNED,
	INVITEM_ARROW_NORMAL,
	INVITEM_ARROW_PERFO,
	INVITEM_ARROW_BASTARD,
	INVITEM_BOMB,
	INVITEM_POWDER,
	INVITEM_SILVER,
	INVITEM_DISAPPEARING,
	INVITEM_LETTER,
	INVITEM_PASS,
	INVITEM_GLASSES,
	INVITEM_FLOWER,
	INVITEM_BOX,
	INVITEM_SCORPIONTAIL,
	INVITEM_HERB,
	INVITEM_MAP,
	INVITEM_SILVERSKULL,
	INVITEM_ARMOR
}INVITEMTYPE;


typedef enum{
	NOBODY,
	INTRO,
    DESSA,
	DEATH,
    EXZOO_WOMAN1,
    EXZOO_WOMAN2,
    EXZOO_WOMAN3,
    EXZOO_MAN1,
    EXZOO_MAN2,
    CEMETERY_WOMAN1,
    CEMETERY_WOMAN2,
    WHOST_SHOP_SMITH,
    WHOST_SHOP_CARPENTER,
	SUPERSTONE,
	SMITH_DISABLED,
	SMITH_CHECKING_WOODANDMETAL,
	SMITH_CHECKING_NOWOODANDMETAL,
	SMITH_GIVING_ARROWS,
	SMITH_FLOWERS_ASKED,
	SMITH_FLOWERS_MISSING,
	SMITH_FLOWERS_THANKYOU,
	SMITH_NEED_GOLD_AND_SILVER,
	SMITH_FORGE_ARMOR,
	CARPENTER_DISABLED,
	CARPENTER_CHECKING_NOWOODANDMETAL,
	CARPENTER_GIVING_ARROWS,
	CARPENTER_NEED_MATERIALS,
	CARPENTER_FIX_BRIDGE,
	BOUNCINGBLOCK,
	WOLF01,
	BLACKIE,
	HOSPITAL_DISABLED,
	HOSPITAL_ENABLING,
	HOSPITAL_CURE,
	HOSPITAL_FINE,
	HOSPITAL_CURE_FROM_DEATH,
	HOSPITAL_GAMEOVER,
	HOSPITAL_HEAL_1,
	HOSPITAL_GET_ANTIDOTE,
	HOSPITAL_THANKS_FOR_TAIL,
	HOSPITAL_GO_FOR_HERBS,
	HOSPITAL_ANTIDOTE_BUILT,
	CRYING_MOTHER,
	CHILD,
	CHILDS_SAVED,
	MAZE_CANT_GO,
	OUTWALKER_NO_ENTER,
	POLICE_0_GET_PASS,
	POLICE_0_STILL_NOT_FOUND,
	POLICE_0_WONT_TALK,
	POLICE_0_FIGHTING,
	POLICE_0_NOGUARDS,
	OUTWALKER_MAN1,
	OUTWALKER_MAN2,
	OUTWALKER_WOMAN1,
	OUTWALKER_WOMAN2,
	JESSICA_PLANTS,
	OUTWALKER_GLASS,
	OUTWALKER_GUARD_NOCHIEF_NOGLASS,
	OUTWALKER_GUARD_NOGLASS,
	OUTWALKER_GUARD_NOSMITH,
	OUTWALKER_GUARD_OK,
	OUTWALKER_CHIEF_FOUND,
	OUTWALKER_GUARD_LANDSLIDE,
	BOSS_CRAB_FIGHT,
	IBEX_GIVE_MISSION,
	IBEX_GIVE_HERBS,
	BLACKIE_DEAD_CHILD,
	IMHUNGRY,
	FISHERMAN_THERESFISH,
	FISHERMAN_LETSGO,
	FISHERMAN_FPSGATOR_COMPLETED,
	ITEMDETAIL_MAP,
	BRIDGE_BROKEN,
	MINOTAUR_ENTRANCE,
	MINOTAUR_DEFEAT
}WHOSTALKING;

typedef enum{
	NOTYPE,
    WOMAN_HEAD1,
    WOMAN_HEAD2,
    WOMAN_BODY1,
    WOMAN_BODY2,
    WOMAN_BODY3,
    MAN_HEAD1,
    MAN_HEAD2,
    MAN_HEAD3,
    MAN_BODY1,
    MAN_BODY2
}NPCTYPE;

typedef enum{
	NONAME,
	JOHN,
	LUKE,
	MARGARET,
	WOMAN,
	LEGO,
	CHILD_TOOFAR,
	MAN,
	WOLF_BLACKIE,
	WOLF_BLACKIE_CHILD,
	OUTWALKER_JERRY,
	OUTWALKER_JASON,
	OUTWALKER_ANNETTE,
	OUTWALKER_JESSICA,
	OUTWALKER_JACK,
	OUTWALKER_SIMON,
	CRAB,
	SCORPION
}NPCNAME;

struct NpcInfo{
	NPCNAME npcname;
    WHOSTALKING whotalks;
	NPCTYPE type;
    UINT8 configured;
	INT8 hp;
	UINT8 none2;
	UINT8 none3;
	UINT8 none4;
};

struct InvItem{
	INVITEMTYPE itemtype;
	UINT16 quantity;
	UINT8 equippable;
};

struct ItemSpawned{
	INVITEMTYPE itemtype;
	UINT8 quantity;
	UINT8 configured;
	UINT8 hp;
	INT8 vy;
	INT8 vx;
	UINT8 frmskip;
	UINT8 equippable;
};

struct InvcursorInfo{
	UINT8 switch_animation;
	UINT8 square_or_arrow;
};

struct PlatformInfo {
	UINT8 configured;
	UINT8 distance;
	UINT8 step;
	INT8 vx;
	INT8 vy;
	INT8 clockwise;
	UINT8 plat_counter;
	ENEMY_STATE p_state;
};

struct TeleportInfo{
	UINT16 dest_x;
	UINT16 dest_y;
};

typedef enum{
	SHOP_SMITH,
	SHOP_CARPENTER
}SHOP;
#endif

