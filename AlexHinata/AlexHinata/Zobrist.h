#ifndef _ZOBRIST_
#define _ZOBRIST_
#include <iostream>
#include <random>

#include "define.h"
using namespace std;

namespace Zobrist {
	static const U64 SEED = std::mt19937_64::default_seed;
	extern U64 ZOBRIST_TABLE[TOTAL_BOARD_SIZE][30];

	void Initialize();
}

#endif