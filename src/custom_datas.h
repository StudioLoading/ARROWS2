#ifndef CUSTOMDATAS_FILE
#define CUSTOMDATAS_FILE

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
	FIND_METAL_SPECIAL,
	ENGAGE_SMITH,
	HELP_CEMATERY_WOMAN,
	HELP_HOSPITAL
}MISSION_TITLE;

struct MISSION{
	MISSION_TITLE mission_title;
	MISSION_STATE mission_state;
	UINT8 current_step;
	UINT8 reward_quantity;
	UINT8 goal;//0 object, 1 person
	UINT8 sprite_goal_type;
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
	MOTHERPL_DASH
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
	TARANTULA
}ENEMY_TYPE;

typedef enum{
	T_UNASSIGNED,
	WEB,
	ACID,
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
	ENEMY_PREATTACK,
	ENEMY_ATTACK,
	ENEMY_THROW,
	ENEMY_UPSIDEDOWN
}ENEMY_STATE;

struct EtoReload{
    UINT8 type;
	UINT16 x;
	UINT16 y;
	UINT8 alive;
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
	INVITEM_HEART, //HP
	INVITEM_WOOD,//IMPORTANT: da qui in su devono esser quelli spawnati, c'è un random su StateFittizio!
	INVITEM_UNASSIGNED,
	INVITEM_ARROW_NORMAL,
	INVITEM_ARROW_PERFO,
	INVITEM_ARROW_BASTARD,
	INVITEM_BOMB,
	INVITEM_POWDER,
	INVITEM_METAL_SPECIAL,
	INVITEM_DISAPPEARING,
	INVITEM_LETTER
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
    SMITH,
	SUPERSTONE,
	SMITH_DISABLED,
	SMITH_CHECKING_WOODANDMETAL,
	SMITH_CHECKING_NOWOODANDMETAL,
	SMITH_GIVING_ARROWS,
	BOUNCINGBLOCK,
	WOLF01,
	BLACKIE,
	HOSPITAL_DISABLED,
	HOSPITAL_ENABLING,
	HOSPITAL_CURE,
	CRYING_MOTHER,
	CHILD,
	CHILDS_SAVED
}WHOSTALKING;

typedef enum{
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

struct NpcInfo{
	UINT8 none1;
	UINT8 none2;
	UINT8 none3;
	UINT8 none4;
	UINT8 none5;
    WHOSTALKING whotalks;
	NPCTYPE type;
    UINT8 configured;
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
	UINT8 type:3;
	UINT8 distance;
	UINT8 step;
	INT8 vx;
	INT8 vy;
};

#endif

