typedef enum{
	INV_EMPTY_STRING,
	CROSSBOW_NAME,
	CROSSBOW_DETAIL1,
	CROSSBOW_DETAIL2,
	CROSSBOW_DETAIL3,
	CROSSBOW_DETAIL4,
	MONEY_NAME,
	MONEY_DETAIL1,
	MONEY_DETAIL2,
	MONEY_DETAIL3,
	MONEY_DETAIL4,
	BOMB_NAME,
	BOMB_DETAIL1,
	BOMB_DETAIL2,
	BOMB_DETAIL3,
	BOMB_DETAIL4,
	ARROWNORMAL_NAME,
	ARROWNORMAL_DETAIL1,
	ARROWNORMAL_DETAIL2,
	ARROWNORMAL_DETAIL3,
	ARROWNORMAL_DETAIL4,
	ARROWPERFO_NAME,
	ARROWPERFO_DETAIL1,
	ARROWPERFO_DETAIL2,
	ARROWPERFO_DETAIL3,
	ARROWPERFO_DETAIL4,
	ARROWBASTARD_NAME,
	ARROWBASTARD_DETAIL1,
	ARROWBASTARD_DETAIL2,
	ARROWBASTARD_DETAIL3,
	ARROWBASTARD_DETAIL4
}INV_TO_BE_LOCALIZED;

void GetLocalizedINVLabel_EN(INV_TO_BE_LOCALIZED label, char* d) BANKED;