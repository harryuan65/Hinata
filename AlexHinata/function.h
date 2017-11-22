#ifndef _FUNCTION_
#define _FUNCTION_
#include <string.h>
/*	reverse bit	*/
/*	LSB -> MSB	*/
int BitScanReverse(U32 board);

/*	read board	*/
void ReadBoard(int *chessboard, Bitboard *board, int filenum);

/*	move generator	*/
int  Generator        (Bitboard board, U16 *movelist, int turn);
void AttackGenerator  (int *start, Bitboard board, U16 *movelist, int turn);
void MoveGenerator    (int *start, Bitboard board, U16 *movelist, int turn);
void HandGenerator    (int *start, Bitboard board, U16 *movelist, int turn);
U32  Uchifuzume		  (Bitboard board, int turn); // ¥´¨B¸×
U32  GenerateAllMoves (Bitboard board, U32 apawn, int turn, int type);
void PromoteGenerator (int *start, int src, int dst, U16 *movelist, int turn, bool atk);
int  UchifuzumeGenerator(Bitboard board, U16 *movelist, int turn, int type);
void HandGenerator_s (int *start, Bitboard board, int *chessboard, U16 *movelist, int turn);

/*	rook move	*/
U32 RookMove (Bitboard board, int pos, int turn);
U32 RookMove_h (Bitboard board, int pos, int turn);

/*	bishop move	*/
U32 BishopMove (Bitboard board, int pos, int turn);
U32 BishopMove_h (Bitboard board, int pos, int turn);

/*	bit scan	*/
int BitScan(U32 *board);

/*  board position to index  */
void SetBitboard(Bitboard& mFighter, int* mBoard);
int boardpos2index(char row, char col);
bool index2boardpos(int index, char* pos);

/*	print	*/
void PrintBoard (U32 Board);
void PrintPV(line path, U32 from, U32 to);
void FPrintChessBoard (int *chessboard, FILE *fptr);
void PrintChessBoard (int *chessboard);
void SetColor (int);
void Information (FILE *fptr, double ttime, line pline);

/*	do move	*/
U16  DoMove        (U16 move, Bitboard *board, int *chessboard, int turn);
void UnDoMove      (U16 capture, U16 move, Bitboard *board, int *chessboard, int turn);
void DoHandMove	   (int src, Bitboard *board);
void UnDoHandMove  (int src, Bitboard *board);
U8   SaveCapture   (int pce, Bitboard *board, int *chessboard);
void RemoveCapture (int cap_place, Bitboard *board);

/*	search	*/
int Minimax      (line *pline, Bitboard *board, int *chessboard, int turn, int depth);
int AlphaBetaMax (line *pline, Bitboard *board, int *chessboard, int alpha, int beta, int turn, int depth);
int AlphaBetaMin (line *pline, Bitboard *board, int *chessboard, int alpha, int beta, int turn, int depth);
int NegaMax      (line *pline, Bitboard *board, int *chessboard, int alpha, int beta, int turn, int depth);
int ScoutMax     (line *pline, Bitboard *board, int *chessboard, int alpha, int beta, int turn, int depth);
int ScoutMin     (line *pline, Bitboard *board, int *chessboard, int alpha, int beta, int turn, int depth);
int NegaScout    (line *pline, Bitboard *board, int *chessboard, int alpha, int beta, int turn, int depth, bool isFailHigh);
int QuiescenceSearch (Bitboard *board, int *chessboard, int alpha, int beta, int turn, bool isFailHigh);
//bool Uchifuzume_Searching (Bitboard *board, int *chessboard, int turn, int depth);

/*	evaluating function	*/
int Evaluate (int *chessboard);

/*	initial	board	*/
void Inital (Bitboard *board, int *chessboard);

#endif