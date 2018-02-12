#ifndef _BOARD_
#define _BOARD_
#include "head.h"

#define BOARD_PATH   "board//"
#define KIFU_PATH    "output//"

class Board {
private:
    bool m_turn;
	int m_step;
    U64 m_hashcode;
	int m_evaluate;

	//第i個recordAction動完後的Zobrist儲存在第i+1個recordZobrist
	//recordZobrist第0個是初始盤面
    Action recordAction[256];
    U64 recordZobrist[256];
    int recordScore[256];

public:
    U32 occupied[2];
    U32 bitboard[32];
    int board[TOTAL_BOARD_SIZE];

    Board();
    ~Board();
    void Initialize();
    void Initialize(const char *str);
    void PrintChessBoard() const;
	void PrintNoncolorBoard(ostream &os) const;

	void DoMove(Action m_Action);
	void UndoMove();

    bool SaveBoard(const string filename, const string comment) const;
    bool LoadBoard(const string filename, streamoff &offset);
	bool SaveKifu(const string filename, const string comment) const;

	bool IsGameOver();
	bool IsSennichite() const;
	bool IsCheckAfter(const int src, const int dst);

    inline void NextTurn() { m_turn ^= 1; }
    inline int GetTurn() const { return m_turn; }
	inline int GetStep() const { return m_step; }
	inline int Evaluate() const { return m_turn ? m_evaluate : -m_evaluate; }
	inline U64 GetZobristHash() const { return m_hashcode; }
	unsigned int GetKifuHash() const;
};

#endif