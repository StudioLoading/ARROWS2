#ifndef ZGBMAIN_H
#define ZGBMAIN_H
#define PauseMusic hUGE_paused = 1; hUGE_mute(HT_CH_MUTE);
#define UNMUTE_ALL_CHANNELS hUGE_mute(HT_CH_PLAY)
#define ResumeMusic hUGE_paused = 0; UNMUTE_ALL_CHANNELS

#define STATES \
_STATE(StateUtils)\
_STATE(StateUtilsOw)\
_STATE(StateTitle)\
_STATE(StateMowUt)\
_STATE(StateMplUt)\
_STATE(StateTutorial)\
_STATE(StateInbetween)\
_STATE(StateCredit)\
_STATE(StateTetra)\
_STATE(StateOw)\
_STATE(StateDiary)\
_STATE(StateInv)\
_STATE(StateDialog)\
_STATE(StateExzoo)\
_STATE(StateCemetery)\
_STATE(StateBlackiecave)\
_STATE(StateMine)\
_STATE(StateShop)\
_STATE(StateBlackieroom)\
_STATE(StateHospital)\
_STATE(StateHood)\
_STATE(StatePwd)\
_STATE(StateOutwalkers)\
_STATE(StateBosscrab)\
_STATE(StateMountain)\
_STATE(StateSky)\
_STATE(StateCart)\
_STATE(StateScorpions)\
_STATE(StateBossscorpion)\
_STATE(StateFps)\
_STATE(StateBridge)\
_STATE(StateSilvercave)\
_STATE(StateBossminotaur)\
_STATE(StateBatcave)\
_STATE(StateHarbor)\
_STATE(StateBossbat)\
STATE_DEF_END
/*
_STATE(StateHospital)\
_STATE(StateFlipper)\
*/


#define SPRITES \
_SPRITE_DMG(SpriteCamerafocus, camerafocus)\
_SPRITE_DMG(SpritePunta, punta)\
_SPRITE_DMG(SpriteMirino, mirino)\
_SPRITE_DMG(SpritePuff, puff)\
_SPRITE_DMG(SpriteTetradado, tetradado)\
_SPRITE_DMG(SpriteTetratacche, tetratacche)\
_SPRITE_DMG(SpriteTetracamera, camerafocus)\
_SPRITE_DMG(SpriteTetracursor, tetracursor)\
_SPRITE_DMG(SpriteTetrahourglass, tetrahourglass)\
_SPRITE_DMG(SpriteTetrapointing, tetrapointing)\
_SPRITE_DMG(SpriteMotherow, motherow)\
_SPRITE_DMG(SpriteMotherowarmor, motherowarmor)\
_SPRITE_DMG(SpriteDiarycursor, diarycursor)\
_SPRITE_DMG(SpriteInvcursor, invcursor)\
_SPRITE_DMG(SpriteInvblinker, invblinker)\
_SPRITE_DMG(SpriteMotherpl, motherpl)\
_SPRITE_DMG(SpriteMotherplarmor, motherplarmor)\
_SPRITE_DMG(SpriteDust, dust)\
_SPRITE_DMG(SpriteEsimple, enemysimple)\
_SPRITE_DMG(SpriteEsimplesnake, enemysnake)\
_SPRITE_DMG(SpriteEsimplerat, enemyrat)\
_SPRITE_DMG(SpriteEAttackerCobra, enemycobra)\
_SPRITE_DMG(SpriteEAttackerPine, enemyporcupine)\
_SPRITE_DMG(SpriteEthrowable, enemythrowable)\
_SPRITE_DMG(SpriteSpider, spider)\
_SPRITE_DMG(SpriteEThrowerSpider, enemyspider)\
_SPRITE_DMG(SpriteEThrowerTarantula, enemytarantula)\
_SPRITE_DMG(SpriteEThrowerScorpion, enemyscorpion)\
_SPRITE_DMG(SpriteArrow, arrow)\
_SPRITE_DMG(SpriteInvitem, invitem)\
_SPRITE_DMG(SpriteRightleft, rightleft)\
_SPRITE_DMG(SpriteItemspawned, itemspawned)\
_SPRITE_DMG(SpritePgceme, pgcemetery)\
_SPRITE_DMG(SpritePgexzoo, pgexzoo)\
_SPRITE_DMG(SpritePgoutwalker, pgoutwalker)\
_SPRITE_DMG(SpriteCavesand, cavesand)\
_SPRITE_DMG(SpriteSuperstone, superstone)\
_SPRITE_DMG(SpriteWolf, wolf)\
_SPRITE_DMG(SpriteDrop, drop)\
_SPRITE_DMG(SpriteBlackie, blackie)\
_SPRITE_DMG(SpriteBlackiechild, blackiechild)\
_SPRITE_DMG(SpriteBlackieow, blackieow)\
_SPRITE_DMG(SpriteLeaf, leaf)\
_SPRITE_DMG(SpriteChild, child)\
_SPRITE_DMG(SpriteMotherowstep, motherowstep)\
_SPRITE_DMG(SpriteFlame, flame)\
_SPRITE_DMG(SpriteTeleport, teleport)\
_SPRITE_DMG(SpriteOwpusha, owpusha)\
_SPRITE_DMG(SpriteOwcrab, owcrab)\
_SPRITE_DMG(SpriteBosscrab, crab)\
_SPRITE_DMG(SpriteGlasses, glasses)\
_SPRITE_DMG(SpriteGocursor, gocursor)\
_SPRITE_DMG(SpriteFlower, flower)\
_SPRITE_DMG(SpriteBigstone, bigstone)\
_SPRITE_DMG(SpriteStone, stone)\
_SPRITE_DMG(SpriteBigstonea, bigstonea)\
_SPRITE_DMG(SpriteBoltground, boltground)\
_SPRITE_DMG(SpriteBolt, bolt)\
_SPRITE_DMG(SpriteCart, cart)\
_SPRITE_DMG(SpriteCartelevator, cartelevator)\
_SPRITE_DMG(SpriteStartbtn, startbtn)\
_SPRITE_DMG(SpriteBirdsky, birdsky)\
_SPRITE_DMG(SpriteScorpion, scorpion)\
_SPRITE_DMG(SpriteOwscorpion, owscorpion)\
_SPRITE_DMG(SpriteDiary, diary)\
_SPRITE_DMG(SpriteOwfisherman, owfisherman)\
_SPRITE_DMG(SpriteBossscorpion, bossscorpion)\
_SPRITE_DMG(SpriteBossscorpionhead, bossscorpionhead)\
_SPRITE_DMG(SpriteScorpiontail, scorpiontail)\
_SPRITE_DMG(SpriteHerb, owherb)\
_SPRITE_DMG(SpriteOwgator, owgator)\
_SPRITE_DMG(SpriteGator, gator)\
_SPRITE_DMG(SpritePuntawater, puntawater)\
_SPRITE_DMG(SpriteFish, fish)\
_SPRITE_DMG(SpritePlusone, plusone)\
_SPRITE_DMG(SpriteBottle, bottle)\
_SPRITE_DMG(SpriteSeagull, seagull)\
_SPRITE_DMG(SpriteBossMino, minotaur)\
_SPRITE_DMG(SpriteBossMinoskull, minotaurskull)\
_SPRITE_DMG(SpriteBossMinobuff, minotaursbuff)\
_SPRITE_DMG(SpriteSkull, skull)\
_SPRITE_DMG(SpriteEBat, bat)\
_SPRITE_DMG(SpriteOwpeople, owpeople)\
_SPRITE_DMG(SpriteSeagullHarbor, seagullharbor)\
_SPRITE_DMG(SpritePgPirate, pgpirate)\
_SPRITE_DMG(SpritePgCadaver, pgcadaver)\
_SPRITE_DMG(SpritePgParrot, parrot)\
_SPRITE_DMG(SpriteBossbat, bossbat)\
_SPRITE_DMG(SpriteInvheart, invheart)\
_SPRITE_DMG(SpriteMushroom, mushroom)\
_SPRITE_DMG(SpriteLiamow, liamow)\
_SPRITE_DMG(SpriteOwmaster, masterow)\
SPRITE_DEF_END


#include "ZGBMain_Init.h"

#endif

