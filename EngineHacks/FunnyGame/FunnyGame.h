#include "gbafe.h"

struct FishInfo {
	u8 fishType;
	u8 movementTimer;
	u8 xPos;
	u8 yPos;
}

struct FunnyGameProc {
	PROC_FIELDS;
	u8 gameState;
	FishInfo activeFish[4];
} typedef FunnyGameProc;

struct BestiaryRecord {
	u8 sizeRecord;
	u8 numCaught;
} typedef BestiaryRecord;

struct GameStateInfo {
	u8 rods[4];
	u8 baitCount[4];
	BestiaryRecord bestiaryRecords[256];
} typedef GameStateInfo;

struct BestiaryInfo {
	short nameTextID;
	short descTextID;
	u16 minLength;
	u16 maxLength;
} typedef BestiaryInfo;

struct 

#define ROD_1 0
#define ROD_2 1
#define ROD_3 2
#define ROD_4 3

#define BAIT_1 0
#define BAIT_2 1
#define BAIT_3 2
#define BAIT_4 3

extern const ProcInstruction ProcScr_FunnyGameMain;
extern const BestiaryInfo BestiaryTable[256];
extern GameStateInfo* FunnyGameState;

void GameStartASMC(Proc* parent);
void FunnyGameInit(Proc* parent);
void FunnyGameMainInit(Proc* parent);
void FunnyGameMainLoop(Proc* parent);


