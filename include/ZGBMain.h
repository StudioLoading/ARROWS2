#ifndef ZGBMAIN_H
#define ZGBMAIN_H
#define PauseMusic hUGE_paused = 1; hUGE_mute(HT_CH_MUTE);
#define UNMUTE_ALL_CHANNELS hUGE_mute(HT_CH_PLAY)
#define ResumeMusic hUGE_paused = 0; UNMUTE_ALL_CHANNELS  

#define WALK_FX_COOLDOWN 32
/*
*/

#define STATES \
_STATE(StateFittizio)\
_STATE(StateTitlescreen)\
_STATE(StateInbetween)\
_STATE(StateCredit)\
_STATE(StateBonus)\
_STATE(StateTetra)\
_STATE(StateOverworld)\
_STATE(StateDiary)\
_STATE(StateInventory)\
_STATE(StateDialog)\
_STATE(StateExzoo)\
_STATE(StateCemetery)\
_STATE(StateBlackiecave)\
_STATE(StateCave)\
_STATE(StateSmith)\
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
_SPRITE_DMG(SpriteDust, dust)\
_SPRITE_DMG(SpriteEnemysimple, enemysimple)\
_SPRITE_DMG(SpriteEnemysimplesnake, enemysnake)\
_SPRITE_DMG(SpriteEnemysimplerat, enemyrat)\
_SPRITE_DMG(SpriteEnemyAttackerCobra, enemycobra)\
_SPRITE_DMG(SpriteEnemyAttackerPine, enemyporcupine)\
_SPRITE_DMG(SpriteEnemythrowable, enemythrowable)\
_SPRITE_DMG(SpriteEnemyThrowerSpider, enemyspider)\
_SPRITE_DMG(SpriteEnemyThrowerTarantula, enemytarantula)\
_SPRITE_DMG(SpriteArrow, arrow)\
_SPRITE_DMG(SpriteInvitem, invitem)\
_SPRITE_DMG(SpriteRightleft, rightleft)\
_SPRITE_DMG(SpriteItemspawned, itemspawned)\
_SPRITE_DMG(SpritePgceme, pgcemetery)\
_SPRITE_DMG(SpritePgexzoo, pgexzoo)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#endif

