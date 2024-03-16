#include "FunnyGame.h"

void GameStartASMC(Proc* parent) {
	ProcStartBlocking(&ProcScr_FunnyGameMain, parent);
}

void FunnyGameInit(FunnyGameProc* parent) {
	//init game
	
	//unlock basic rod
	FunnyGameState->rods[ROD_1] = 1;
	
	//100 basic bait
	FunnyGameState->baitCount[BAIT_1] = 100;
	
}

void FunnyGameMainInit(FunnyGameProc* parent) {
	//runs just before main loop, while faded to black
	
	//load the main game screen
	
	//generate 4 random fish
	
	//start the cast selection menu
	
	
}

void FunnyGameMainLoop(FunnyGameProc* parent) {
	//main loop
	
	//move fish as needed
	
	//wait for cast selection menu to not exist
	
		//switch on game state
	
			//case: casting line
			
			//case: line casted
			
			//case: fish caught or escaped
	
}
