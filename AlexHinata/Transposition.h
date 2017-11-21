#ifndef _TRANSPOSITION_
#define _TRANSPOSITION_

#define ZOBRIST_SEED        10      // �üƺؤl
#define ZOBRIST_SIZE        30 * 35 // Zobrist Table �j�p  �Ѥl����*35+��m
//#define ZOBRIST_NUMBER_SIZE 1 << 32 // �üƤj�p
//#define TRANSPOSITION_SIZE  1 << 32 // �P�Ϊ� �j�p
#define TRANSPOSITION_FILE_NAME "transposition_table.txt"
#include "head.h"


class Transposition {
public:
	Transposition();
	bool SaveTable(); // �x�s�P�Ϊ���ɮ׸�
	U32 ZobristHashing(const int* chessboard); // �^�� Hash Code
	U32 ZobristHashingByAction(const int* chessboard, U32 hashcode, U16 action); // �^�� Hash Code

	bool m_isEnable; // �O�_�ҥΦP�Ϊ�
private:
	void InitZobristTable(int seed); // ��l�Ƥ@�Ӷü� Zobrist Table
	U32 m_zobristTable[ZOBRIST_SIZE];
	//map<int, TranspositionData> m_transpositionTables;
};

struct TranspositionData{
	unsigned char depth;
	int bestEvaluate;
	U16 bestAction;
};

#endif // !_TRANSPOSITION_
