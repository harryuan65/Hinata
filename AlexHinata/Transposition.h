#ifndef _TRANSPOSITION_
#define _TRANSPOSITION_

#define ZOBRIST_SEED        10      // 亂數種子
#define ZOBRIST_SIZE        30 * 35 // Zobrist Table 大小  棋子種類*35+位置
//#define ZOBRIST_NUMBER_SIZE 1 << 32 // 亂數大小
//#define TRANSPOSITION_SIZE  1 << 32 // 同形表 大小
#define TRANSPOSITION_FILE_NAME "transposition_table.txt"
#include "head.h"


class Transposition {
public:
	Transposition();
	bool SaveTable(); // 儲存同形表到檔案裡
	U32 ZobristHashing(const int* chessboard); // 回傳 Hash Code
	U32 ZobristHashingByAction(const int* chessboard, U32 hashcode, U16 action); // 回傳 Hash Code

	bool m_isEnable; // 是否啟用同形表
private:
	void InitZobristTable(int seed); // 初始化一個亂數 Zobrist Table
	U32 m_zobristTable[ZOBRIST_SIZE];
	//map<int, TranspositionData> m_transpositionTables;
};

struct TranspositionData{
	unsigned char depth;
	int bestEvaluate;
	U16 bestAction;
};

#endif // !_TRANSPOSITION_
