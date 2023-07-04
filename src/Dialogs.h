#include "custom_datas.h"

typedef enum{
	DIARIO_MISSIONI,
	OPZIONI_PARTITA
}TO_BE_LOCALIZED;

typedef enum{
	TIP_NOTHING,
	TIP_SMITH_NO,
	TIP_HOSPITAL_NO,
	TIP_BLACKIE_CAVE,
	TIP_MINE_CAVE,
	TIP_OWLIMIT_EAST,
	TIP_OWLIMIT_SOUTH,
	TIP_STILL_SOMETHING
}TIP_TO_BE_LOCALIZED;


void GetLocalizedLog_EN() BANKED;
void GetLocalizedLabel_EN(TO_BE_LOCALIZED label, char* d) BANKED;
void GetLocalizedTip_EN(TIP_TO_BE_LOCALIZED label) BANKED;
void GetLocalizedDialog_EN(UINT8* n_lines) BANKED;