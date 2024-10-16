#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"

#include "DialogTips.h"

extern unsigned char d0[22];
extern unsigned char d1[22];
extern unsigned char d2[22];
extern unsigned char d3[22];
extern unsigned char d4[22];
extern unsigned char EMPTY_STRING_21[22];

void GetLocalizedTip_EN(TIP_TO_BE_LOCALIZED tip) BANKED{
	switch(tip){
		case TIP_HIDDEN_ARROWS:
			memcpy(d1, " SOMETHING'S ON THE  ", 22);
			memcpy(d2, " GROUND:             ", 22);
			memcpy(d3, "           ARROWS!   ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_HIDDEN_METAL:
			memcpy(d1, " SOMETHING'S ON THE  ", 22);
			memcpy(d2, " GROUND:             ", 22);
			memcpy(d3, "          METAL!     ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_HIDDEN_WOOD:
			memcpy(d1, " SOMETHING'S ON THE  ", 22);
			memcpy(d2, " GROUND:             ", 22);
			memcpy(d3, "           WOOD!     ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_HIDDEN_MONEY:
			memcpy(d1, " SOMETHING'S ON THE  ", 22);
			memcpy(d2, " GROUND:             ", 22);
			memcpy(d3, "           COINS !   ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_PRESS_SELECT:
			memcpy(d1, " PRESS SELECT TO    ", 22);
			memcpy(d2, " VIEW MY MISSION'S  ", 22);
			memcpy(d3, " DIARY FOR THE      ", 22);
			memcpy(d4, " CURRENT CHAPTER.   ", 22);
		break;
		case TIP_SMITH_NO:
			memcpy(d1, " THE SMITH IS YET TO ", 22);
			memcpy(d2, " COME.               ", 22);
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_HOSPITAL_NO:
			memcpy(d1, " THIS IS THE         ", 22);
			memcpy(d2, " HOSPITAL.           ", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_MINE_CAVE:
			memcpy(d1, " THIS IS THE OLD     ", 22);
			memcpy(d2, " MINE. HERE I FIND   ", 22);
			memcpy(d3, " METAL AND WOOD.     ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_BLACKIE_CAVE:
			memcpy(d1, "  RED RIDING HOOD   ", 22);
			memcpy(d2, "  WORST ENEMY CAVE  ", 22);
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_OWLIMIT_EAST:
			memcpy(d1, " IT IS NOT SAFE TO   ", 22);
			memcpy(d2, " GO THROUGH THIS     ", 22);
			memcpy(d3, " FOREST YET.         ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_STILL_SOMETHING:
			memcpy(d1, " I HAVE TO DO SOMETH ", 22);
			memcpy(d2, " ING ELSE HERE       ", 22);
			memcpy(d3, " BEFORE LEAVING      ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_DARK_FOREST:
			memcpy(d1, " THE DARKEST FOREST ", 22);
			memcpy(d2, "THIS IS THE WAY TO  ", 22);
			memcpy(d3, "THE CASTLE. I DON'T " , 22);
			memcpy(d4, "WANT TO GO THERE.   ", 22);
		break;
		case TIP_LABIRYNTH:
			memcpy(d1, "      NORTHERN      ", 22);
			memcpy(d2, "        MAZE        ", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_GROTTO:
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "  OUTWALKER'S CAMP  ", 22);
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_FISHERMAN:
			memcpy(d1, " THE FISHERMAN      ", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_SILVER_MINE:
			memcpy(d1, "    SILVER MINE:    ", 22);
			memcpy(d2, "      BEWARE        ", 22);
			memcpy(d3, "   THE MINOTAUR     " , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_CHITCHAT_HUNGRY:
			memcpy(d1, " OH PLEASE BRING US ", 22);
			memcpy(d2, " SOME FOOD!         ", 22);
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_CHITCHAT_SMITH:
			memcpy(d1, " THE FORGE IS THE   ", 22);
			memcpy(d2, " BUILDING WITH AN   ", 22);
			memcpy(d3, " ANVIL SIGN POSTED  ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_HOSP:
			memcpy(d1, " YOU CAN GET HEALED ", 22);
			memcpy(d2, " AT THE HOSPITAL.   ", 22);
			memcpy(d3, " IT WILL COST YOU   ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_ITEMS:
			memcpy(d1, " GET ITEM LOOTS GET ", 22);
			memcpy(d2, " TING WILD ANIMALS  ", 22);
			memcpy(d3, " DOWN!              ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_SALUTE:
			memcpy(d1, " SALUTE HEALER! OUR ", 22);
			memcpy(d2, " FAMILY WISHES YOU  ", 22);
			memcpy(d3, " A VERY GOOD LUCK!  ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_HIDDEN_00:
			memcpy(d1, " LOOK IN FRONT AT   ", 22);
			memcpy(d2, " THE CEMETERY ENTRA ", 22);
			memcpy(d3, " NCE                ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_HIDDEN_01:
			memcpy(d1, " LOOK TO THE RIGHT  ", 22);
			memcpy(d2, " OF THE HOSPITAL.   ", 22);
			memcpy(d3, " SOMETHING'S THERE. ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_HIDDEN_02:
			memcpy(d1, " THEY SAY THE CARPE ", 22);
			memcpy(d2, " NTER HAS LEFT SOME ", 22);
			memcpy(d3, " WOOD IN THE SOUTH.	", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_LABYRINTH:
			memcpy(d1, " HAVE YOU CHECKED   ", 22);
			memcpy(d2, " THE UNDERGROUND    ", 22);
			memcpy(d3, " MAZE YET?          ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_GLASS:
			memcpy(d1, " YOU KNOW THE SMITH ", 22);
			memcpy(d2, " IS ABLE TO PRODUCE ", 22);
			memcpy(d3, " GLASS? WONDERFUL!  ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_MARGARET:
			memcpy(d1, " MARGARET LOOKS BET ", 22);
			memcpy(d2, " TER SINCE HER CHI  ", 22);
			memcpy(d3, " LD'S RETURNED.     ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_MOUNTAINS:
			memcpy(d1, " THE PATH TO THE MO ", 22);
			memcpy(d2, " UNTAINS STARTS FRO ", 22);
			memcpy(d3, " M OUTWALKER'S CAMP ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_LIAM:
			memcpy(d1, " YOUR SON LIAM      ", 22);
			memcpy(d2, " FOUGHT GLORIOUSLY! ", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_HIDDEN_10:
			memcpy(d1, " I SAW SOME READY   ", 22);
			memcpy(d2, " TO USE ARROWS AT   ", 22);
			memcpy(d3, " THE BEACH!         ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_HIDDEN_11:
			memcpy(d1, " A GUY AT THE INN   ", 22);
			memcpy(d2, " SAID THERE ARE     ", 22);
			memcpy(d3, " ARROWS IN THE MAZE ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_SCORPIONS_CAME:
			memcpy(d1, " WE DON'T KNOW WHERE", 22);
			memcpy(d2, " THOSE SCORPIONS    ", 22);
			memcpy(d3, " CAME FROM...       ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
    	case TIP_CHITCHAT_SCORPIONS_ALLAROUND:
			memcpy(d1, " SCORPIONS WERE     ", 22);
			memcpy(d2, " ALL AROUND US! IT  ", 22);
			memcpy(d3, " WAS TERRIBLE!      ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
        case TIP_CHITCHAT_HERBS:
			memcpy(d1, " DURING SUMMERS A   ", 22);
			memcpy(d2, " MEDICAL HERB GROWS ", 22);
			memcpy(d3, " SOUTH.             ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
        case TIP_CHITCHAT_IBEX:
			memcpy(d1, " SOMETIMES I CAN SEE", 22);
			memcpy(d2, " THUNDERS FLASHING  ", 22);
			memcpy(d3, " THE WESTERN SKY.   ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
        case TIP_CHITCHAT_HIDDEN_20:
		case TIP_CHITCHAT_HIDDEN_21:
		case TIP_CHITCHAT_HIDDEN_22:
			memcpy(d1, " FEW DAYS AGO SOME  ", 22);
			memcpy(d2, " ITEMS WERE ON THE  ", 22);
			memcpy(d3, " GROUND... DID YOU  ", 22);
			memcpy(d4, " PICK UP SOME?      ", 22);
		break;
		case TIP_CHITCHAT_EASTSIDE:
			memcpy(d1, " OUR HUMAN COMMUNI  ", 22);
			memcpy(d2, " TY OCCUPY THE WEST ", 22);
			memcpy(d3, " ERN AND THE EASTERN", 22);
			memcpy(d4, " SIDE OF THE CANAL  ", 22);
		break;
		case TIP_CHITCHAT_BRIDGE:
			memcpy(d1, " THE ONLY WAY TO    ", 22);
			memcpy(d2, " REACH THE EAST SI  ", 22);
			memcpy(d3, " DE IS BY CROSSING  ", 22);
			memcpy(d4, " THE BRIDGE.        ", 22);
		break;
		case TIP_CHITCHAT_CARPENTER:
			memcpy(d1, " THE CARPENTER IS   ", 22);
			memcpy(d2, " VERY GOOD AT WORK  ", 22);
			memcpy(d3, " ING WITH WOOD AND  ", 22);
			memcpy(d4, " STONE.             ", 22);
		break;
		case TIP_CHITCHAT_DARKESTFOREST:
			memcpy(d1, " THE DARKEST FOREST ", 22);
			memcpy(d2, " IS A VERY OBSCURE  ", 22);
			memcpy(d3, " AND MISTERIOUS PLA ", 22);
			memcpy(d4, " CE. ARE YOU GOING? ", 22);
		break;
		case TIP_CHITCHAT_HIDDEN_30://in basso, fra i due alberi money
			memcpy(d1, " I LIKE TO WATCH    ", 22);
			memcpy(d2, " THE SKY BETWEEN    ", 22);
			memcpy(d3, " TWO TWIN TREES.    ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_CHITCHAT_HIDDEN_31://dietro al carpentiere metal
			memcpy(d1, " SOMEONE SAW THE CA ", 22);
			memcpy(d2, " RPENTER THROWING   ", 22);
			memcpy(d3, " AWAY SOME METAL BE ", 22);
			memcpy(d4, " HIND HIS SHOP.     ", 22);
		break;
		case TIP_CHITCHAT_HIDDEN_32://nei massi a nord
			memcpy(d1, " SOMEONE SAW THE    ", 22);
			memcpy(d2, " CARPENTER THROWING ", 22);
			memcpy(d3, " AWAY SOME WOOD BY  ", 22);
			memcpy(d4, " THE BEACH.         ", 22);
	    break;
		case TIP_CHITCHAT_NICEARMOR:
			memcpy(d1, " WOW! YOUR ARMOR IS ", 22);
			memcpy(d2, " SO COOL!           ", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_CHITCHAT_WEAREGOODHERE:
			memcpy(d1, " WE ARE ALL GOOD. Y ", 22);
			memcpy(d2, " OU HELPED US A LOT ", 22);
			memcpy(d3, " HERE. NOW IT IS TI ", 22);
			memcpy(d4, " ME FOR YOU TO SAIL!", 22);
		break;
        case TIP_CHITCHAT_LEGOTWIN:
			memcpy(d1, " LEGO HAS A TWIN!   ", 22);
			memcpy(d2, " HIS BROTHER WORKS  ", 22);
			memcpy(d3, " FOR CAPTAIN ONE    ", 22);
			memcpy(d4, " EYED JACK.         ", 22);
		break;
        case TIP_CHITCHAT_CAPTAIN:
			memcpy(d1, " ONE EYED JACK CAN  ", 22);
			memcpy(d2, " HELP YOU CROSS THE ", 22);
			memcpy(d3, " SEA.               ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
        case TIP_CHITCHAT_THANKYOU:
			memcpy(d1, " THANK YOU FOR ALL  ", 22);
			memcpy(d2, " THE HELP YOU GAVE  ", 22);
			memcpy(d3, " US. NOW GO SAVE    ", 22);
			memcpy(d4, " YOUR KID!          ", 22);
		break;
		case TIP_CHITCHAT_MASTER:
			memcpy(d1, " READ THAT MESSAGE  ", 22);
			memcpy(d2, " AND COME BACK TRAI ", 22);
			memcpy(d3, " NING, YOU LAZY ASS ", 22);
			memcpy(d4, "                    ", 22);
		break;
	}
}