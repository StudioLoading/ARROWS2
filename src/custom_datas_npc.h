typedef enum{
    WOMAN_HEAD1,
    WOMAN_HEAD2,
    WOMAN_BODY1,
    WOMAN_BODY2,
}NPCTYPE;

struct NpcInfo{
	NPCTYPE type;
	Sprite* head;
	Sprite* body;
    UINT8 configured;
};
