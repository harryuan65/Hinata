#ifndef _TRANSPOSITION_
#define _TRANSPOSITION_

#define ZOBRIST_SEED        10      // 亂數種子
#define ZOBRIST_SIZE        32 * 45 // Zobrist Table 大小  棋子種類*45+位置
//#define ZOBRIST_NUMBER_SIZE 1 << 24 // 亂數大小
//#define TRANSPOSITION_SIZE  1 << 24 // 同形表 大小
#define HASHVALUE_MASK      0x00FFFFFF
#define TRANSPOSITION_FILE_NAME "transposition_table.txt"
#include "head.h"


class Transposition {
public:
	Transposition(bool isEnable);
	bool SaveTable(); // 儲存同形表到檔案裡
	U32 ZobristHashing(const int* chessboard); // 回傳 Hash Code
	U32 ZobristHashingByAction(const int* chessboard, U32 hashcode, U16 action); // 回傳 Hash Code

private:
	void InitZobristTable(int seed); // 初始化一個亂數 Zobrist Table
	
	bool m_isEnable; // 是否啟用同形表
	U32 m_zobristTable[ZOBRIST_SIZE];
	//map<int, TranspositionData> m_transpositionTables;
};

struct TranspositionData{
	unsigned char depth;
	int bestEvaluate;
	U16 bestAction;
};

#endif // !_TRANSPOSITION_
