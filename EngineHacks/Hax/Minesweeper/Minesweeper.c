
#include "Minesweeper.h"

void InitMinesweeperBoard(Proc* parent) {
	//create 'initialMineCount' # of mine traps at random locations on the board

	for (int i = 0; i < initialMineCount; i++) {
		AddTrap(NextRN_N(boardX-1), NextRN_N(boardY-1), TRAP_MINE, 0);
	}
	BmMapFill(gMapMovement, 0);

}

void InitChapterMap(u8 chapterIndex) {
	//we'll see if lyn autohooks this, if not its just a replaceWithHack
	//for each tile in gMapRawTiles, write TILE_HIDDEN to it
	//then call InitMinesweeperBoard

	memset(&gGenericBuffer, TILE_HIDDEN, 0x2000);

	BmMapInit(&gGenericBuffer,&gMapMovement,boardX,boardY);

	InitMinesweeperBoard(NULL);

}

void PropagateTileSelection(Proc* parent) {
	//reveal the tile selected
	//if the tile selected is a mine, game over
	//if the tile selected has a value of 0, reveal all adjacent tiles
	//repeat recursively until all necessary tiles are revealed

	u8 xPos = gGameState.cursorMapPos.x;
	u8 yPos = gGameState.cursorMapPos.x;

	if (GetTrapAt(xPos,yPos)->xPosition == xPos) {
		SetEventId(0x65);
		return;
	}

	u8* pos = *gMapMovement;
	pos = pos+(xPos*yPos);
	*pos = 1;
}

u8 GetTileValue(u16 x, u16 y) {
	//get # of mines within the surrounding 8 tiles of the given position
	//this is just the sum of how many of 8 GetTrapAt calls return a trap

	u8 i = 0;

	for (int j = 0; j < 8; j++) {
		Trap* curTrap = GetTrapAt(x,y);
		if (curTrap->xPosition == x) i++;
	}
	return i;
	
}

void CheckWinState(Proc* parent) {
	//check each tile on the map
	//if not uncovered and not a mine, return false to memory slot sC
	//if reaching the end of the map and everything is uncovered, return true to memory slot sC
	u8* curPos = *gMapMovement;
	bool j = false;
	for (int i = 0; i < (boardX*boardY); i++) {
		if (!*curPos) j = true;
		curPos++;
	}
	if (j) gEventSlot[0xC] = false;
	else gEventSlot[0xC] = true;
	
}

void GenerateTileMapFromMinesAndRevealed(void* pool) {
	//this function will replace whatever handles copying the chapter map into RAM
	//instead it will set everything to covered tile, then copy changes defined on another map overtop
		
	memset(pool, TILE_HIDDEN, 0x2000);
	u8* buffer = pool;

	//go through (boardX*boardY) tiles in gMapMovement and if a tile is revealed change its appearance
	if (gMapMovement == 0) return;
	u8* curPos = gMapMovement;
	u8 curX = 0;
	u8 curY = 0;

	for (int i = 0; i < (boardX*boardY); i++) {
		if (*curPos) {
			u8* b = buffer+i;
			*b = GetTileIndexFromInt(GetTileValue(curX,curY));
		}
		curX ++;
		if (curX == boardX) {
			curX = 0;
			curY ++;
		}
		curPos++;
	} 

}

u8 GetTileIndexFromInt(int i) {
	switch (i) {
	case 0:
		return TILE_0;
	case 1:
		return TILE_1;
	case 2:
		return TILE_2;
	case 3:
		return TILE_3;
	case 4:
		return TILE_4;
	case 5:
		return TILE_5;
	case 6:
		return TILE_6;
	case 7:
		return TILE_7;
	case 8:
		return TILE_8;
	}
}

// take an 8-byte input string, cast it to a u16 array, pass to this function
void ApplyRNGSeed(u16* seed) {
	const u16* aSeed = seed;
	SetRandState(aSeed);
}


void PackMinesweeperData() {

}

void UnpackMinesweeperData() {

}

