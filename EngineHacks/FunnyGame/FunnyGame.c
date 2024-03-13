#include "FunnyGame.h"

void GameStartASMC(Proc* parent) {
	ProcStartBlocking(&ProcScr_FunnyGameMain, parent);
}

void FunnyGameInit(Proc* parent) {
	//init game
	
}

void FunnyGameMainInit(Proc* parent) {
	//runs just before main loop, while faded to black
}

void FunnyGameMainLoop(Proc* parent) {
	//main loop
	
}
