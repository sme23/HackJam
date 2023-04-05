
#include "gbafe.h"


// called as ASMC at the start of the chapter to set mine locations
void InitMinesweeperBoard(Proc* parent);

// called as ASMC when an unrevealed tile is selected
void PropagateTileSelection(Proc* parent);

// replaces function that generates the map
void GenerateTileMapFromMinesAndRevealed(void* pool);

// ASMC that checks if you've won yet
void CheckWinState(Proc* parent);

// helper function to get value of a given tile
u8 GetTileValue(u16 x, u16 y);

// helper function to set an RNG seed
void applyRNGSeed(u16* seed);

// gets tile ID from tile value
u8 GetTileIndexFromInt(int i);

// # of mines to generate per board
extern u8 initialMineCount;

// # size of board
extern u8 boardX;
extern u8 boardY;

enum {
	TILE_HIDDEN = 0,
	TILE_0 = 1,
	TILE_FLAG = 2,
	TILE_MINE = 3,
	TILE_1 = 17,
	TILE_2 = 18,
	TILE_3 = 19,
	TILE_4 = 20,
	TILE_5 = 33,
	TILE_6 = 34,
	TILE_7 = 35,
	TILE_8 = 36
};

// want to eventually move initialMineCount, boardX, and boardY to RAM and let the player set them
// EMS should save these values, the current trap layout, and the currently revealed board state
// revealed board state should go somewhere else (fog map?)

//gMapRawTiles for the visual layout, gMapMovement for the uncovered state


