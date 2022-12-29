#ifndef ZGBMAIN_H
#define ZGBMAIN_H
#define PauseMusic hUGE_paused = 1; hUGE_mute(HT_CH_MUTE);
#define UNMUTE_ALL_CHANNELS hUGE_mute(HT_CH_PLAY)
#define ResumeMusic hUGE_paused = 0; UNMUTE_ALL_CHANNELS  

#define WALK_FX_COOLDOWN 32
/*
*/

#define STATES \
_STATE(StateCredit)\
_STATE(StateBonus)\
_STATE(StateTetra)\
_STATE(StateOverworld)\
_STATE(StateDiary)\
_STATE(StateExzoo)\
_STATE(StateFittizio)\
_STATE(StateCemetery)\
_STATE(StateBlackiecave)\
_STATE(StateCave)\
_STATE(StateInventory)\
STATE_DEF_END
/*_STATE(StateFlipper)\*/


#define SPRITES \
_SPRITE_DMG(SpriteCamerafocus, camerafocus)\
_SPRITE_DMG(SpritePunta, punta)\
_SPRITE_DMG(SpriteMirino, mirino)\
_SPRITE_DMG(SpriteTarget, target)\
_SPRITE_DMG(SpritePuff, puff)\
_SPRITE_DMG(SpriteTetradado, tetradado)\
_SPRITE_DMG(SpriteTetratacche, tetratacche)\
_SPRITE_DMG(SpriteTetracamera, camerafocus)\
_SPRITE_DMG(SpriteTetracursor, tetracursor)\
_SPRITE_DMG(SpriteMotherow, motherow)\
_SPRITE_DMG(SpriteDiarycursor, diarycursor)\
_SPRITE_DMG(SpriteInvcursor, invcursor)\
_SPRITE_DMG(SpriteMotherpl, motherpl)\
_SPRITE_DMG(SpriteEnemysimple, enemysimple)\
_SPRITE_DMG(SpriteEnemythrower, enemythrower)\
_SPRITE_DMG(SpriteEnemythrowable, enemythrowable)\
_SPRITE_DMG(SpriteArrow, arrow)\
_SPRITE_DMG(SpriteInvitem, invitem)\
SPRITE_DEF_END


/*
_SPRITE_DMG(SpritePorcupine, porcupine)\
*/
#include "ZGBMain_Init.h"

#endif

