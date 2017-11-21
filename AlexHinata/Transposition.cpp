#include "head.h"

Transposition::Transposition() {
	m_isEnable = true;
	FILE *file = fopen(TRANSPOSITION_FILE_NAME, "r");
	if (file != NULL) {
		printf("Loading %s\n", TRANSPOSITION_FILE_NAME);
		// TODO:
		//for (int i = 0; i < TRANSPOSITION_FILE_NAME; i++) {
		//	fscanf(file, "%d", &table[i]); TODO:
		//}
		fclose(file);
	}
	printf("Make Zobrist Table by seed %d\n", ZOBRIST_SEED);
	InitZobristTable(ZOBRIST_SEED);
}

U32 Transposition::ZobristHashing(const int* chessboard) {
	U32 hashcode = 0;
	/*for (int i = 0; i < 35; i++) {
		if (chessboard[i] != BLANK) {
			hashcode ^= m_zobristTable[chessboard[i] * 45 + i];
		}
	}*/
	return hashcode;
}

U32 Transposition::ZobristHashingByAction(const int* chessboard, U32 hashcode, U16 action) {
	/*int srcChess = action & SRC_CHESS_MASK;
	int srcIndex = action & SRC_MASK;
	int dstChess = action & DST_CHESS_MASK;
	int dstIndex = action & DST_MASK;
	hashcode ^= m_zobristTable[srcChess * 45 + srcIndex];
	if (dstChess != BLANK) {
		hashcode ^= m_zobristTable[dstChess * 45 + dstIndex];
		hashcode ^= m_zobristTable[dstChess * 45 + (dstChess-1) & 0x7 + 5 * (dstChess >> 3)];
	}
	hashcode ^= m_zobristTable[srcChess * 45 + dstIndex];*/
	return hashcode;
}

bool Transposition::SaveTable() {
	if (!m_isEnable) {
		return false;
	}
	FILE *file = fopen(TRANSPOSITION_FILE_NAME, "w");
	printf("Saving %s\n", TRANSPOSITION_FILE_NAME);
	// TODO:
	//for (int i = 0; i < TRANSPOSITION_FILE_NAME; i++) {
	//	fprintf(file, "%d ", table[i]);
	//}
	fclose(file);
	return true;
}

void Transposition::InitZobristTable(int seed) {
	for (int i = 0; i < ZOBRIST_SIZE; i++) {
		m_zobristTable[i] = rand();
	}
	return;
}