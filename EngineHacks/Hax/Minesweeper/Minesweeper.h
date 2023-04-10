
#include "global.h"

#include "constants/terrains.h"

#include "hardware.h"
#include "chapterdata.h"
#include "proc.h"
#include "event.h"
#include "bmunit.h"
#include "bmmap.h"
#include "bmidoten.h"
#include "bmtrick.h"
#include "rng.h"
#include "bmtrap.h"

enum { MAP_POOL_SIZE = 0x7B8 };

// TODO: figure out what's up with this (overlaps with a lot of other objects?)
extern u16 gBmMapBuffer[];

extern void Decompress(const void* src, void* dst);

EWRAM_DATA struct Vec2 gBmMapSize = {};

EWRAM_DATA u8** gBmMapUnit     = NULL;
EWRAM_DATA u8** gBmMapTerrain  = NULL;
EWRAM_DATA u8** gBmMapMovement = NULL;
EWRAM_DATA u8** gBmMapRange    = NULL;
EWRAM_DATA u8** gBmMapFog      = NULL;
EWRAM_DATA u8** gBmMapHidden   = NULL;
EWRAM_DATA u8** gBmMapOther    = NULL;

EWRAM_DATA static u8 sBmMapUnitPool[MAP_POOL_SIZE] = {};
EWRAM_DATA static u8 sBmMapTerrainPool[MAP_POOL_SIZE] = {};
EWRAM_DATA static u8 sBmMapFogPool[MAP_POOL_SIZE] = {};
EWRAM_DATA static u8 sBmMapHiddenPool[MAP_POOL_SIZE] = {};
EWRAM_DATA static u8 sBmMapOtherPool[MAP_POOL_SIZE] = {};

extern u16 sTilesetConfig[0x1000 + 0x200];

EWRAM_DATA static u16 sBmBaseTilesPool[MAP_POOL_SIZE] = {};

extern void* memset(void* dest, int ch, unsigned int count);

extern unsigned gEventSlots[0xE];

static u8** sInitializingMap;

static u8 sBmMapMovementPool[MAP_POOL_SIZE];
static u8 sBmMapRangePool[MAP_POOL_SIZE];

// called as ASMC at the start of the chapter to set mine locations
void InitMinesweeperBoard(struct Proc* parent);

// This is the handler for when A is pressed
int Map_OnAPress(struct Proc* parent);

// called as ASMC when an unrevealed tile is selected
void PropagateTileSelection(u8 xPosit, u8 yPosit);

// replaces function that generates the map
void GenerateTileMapFromMinesAndRevealed(void* pool);

// ASMC that checks if you've won yet
void CheckWinState(struct Proc* parent);

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


