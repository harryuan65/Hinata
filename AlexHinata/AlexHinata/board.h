#ifndef _BOARD_
#define _BOARD_
#include "head.h"

#define BOARD_PATH   "board//"
#define LBOARD_PATH L"board//"
#define KIFU_PATH    "output//"
#define LKIFU_PATH  L"output//"

class Board {
private:
    static U64 Board::ZOBRIST_TABLE[TOTAL_BOARD_SIZE][30];
    static const int ZOBRIST_SEED = 10;

    bool m_turn;
	int m_evaluate;
    U64 m_whiteHashcode;
    U64 m_blackHashcode;

    U64 m_hashcode;

	//第i個recordAction動完後的Zobrist儲存在第i+1個recordZobrist
	//recordZobrist第0個是初始盤面
    U32 m_step;
	Action recordAction[256];
	U64 recordZobrist[256];

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
	void CalZobristNumber();
	void CalZobristNumber(int srcIndex, int dstIndex, int srcChess, int dstChess);

	void DoMove(Action m_Action);
	void UndoMove();

    bool SaveBoard(const string filename, const string comment) const;
    bool LoadBoard(const string filename, int &offset);
	bool SaveKifu(const string filename, const string comment) const;

	bool IsGameOver();
	bool IsSennichite() const;
	bool IsCheckAfter(const int src, const int dst);
    inline bool GetTurn() const { return m_turn; }
	inline int GetStep() const { return m_step; }
	inline int Evaluate() const { return m_turn ? m_evaluate : -m_evaluate; };
	inline U64 GetZobristHash() const {
		//return m_turn ? (m_blackHashcode << 32) | m_whiteHashcode : (m_whiteHashcode << 32) | m_blackHashcode;
        return m_hashcode;
	}
	unsigned int GetKifuHash() const ;
};

#endif 