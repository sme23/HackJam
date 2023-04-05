
#include "gbafe.h"


// called as ASMC at the start of the chapter to set mine locations
void InitMinesweeperBoard(Proc* parent);

// called as ASMC when an unrevealed tile is selected
void PropagateTileSelection(Proc* parent);

// replaces function that generates the map
void GenerateTileMapFromMinesAndRevealed();

// ASMC that checks if you've won yet
void CheckWinState(Proc* parent);

// helper function to get value of a given tile
void GetTileValue(u16 x, u16 y);

// # of mines to generate per board
extern u8 initialMineCount;

// # size of board
extern u8 boardX;
extern u8 boardY;

// want to eventually move initialMineCount, boardX, and boardY to RAM and let the player set them
// EMS should save these values, the current trap layout, and the currently revealed board state
// revealed board state should go somewhere else (fog map?)




