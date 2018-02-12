#include "Zobrist.h"

U64 Zobrist::ZOBRIST_TABLE[TOTAL_BOARD_SIZE][30];

void Zobrist::Initialize() {
    /* Make Zobrist Table */
    std::mt19937_64 gen(Zobrist::SEED);
    std::uniform_int_distribution<unsigned long long> dist;

    int i = 0, j;
    for (; i < 25; ++i) {
        for (j = 0; j < 30; ++j) {
            ZOBRIST_TABLE[i][j] = dist(gen);
        }
    }

    for (; i < 37; ++i) {
        for (j = 0; j < 3; ++j) {
            ZOBRIST_TABLE[i][j] = dist(gen);
        }
    }

	cout << "Zobrist Table Created. Random Seed : " << Zobrist::SEED << "\n";
}