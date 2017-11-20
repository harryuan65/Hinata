#ifndef _FUNCTION_
#define _FUNCTION_

/*	move generator	*/
int  Generator        (fighter &board, U16 *movelist, int turn);
void AttackGenerator  (int &start, fighter &board, U16 *movelist, int turn);
void MoveGenerator    (int &start, fighter &board, U16 *movelist, int turn);
void HandGenerator    (int &start, fighter &board, U16 *movelist, int turn);
void PromoteGenerator (int &start, int src, int dst, U16 *movelist, int turn, bool atk);
U32  Uchifuzume		  (fighter &board, int turn); // ¥´¨B¸×
U32  GenerateAllMoves (fighter board, U32 apawn, int turn, int type);
//int  UchifuzumeGenerator(fighter board, U16 *movelist, int turn, int type);
//void HandGenerator_s (int start, fighter board, int *chessboard, U16 *movelist, int turn);

/*	rook move	*/
U32 RookMove (fighter &board, int pos, int turn);
U32 RookMove_h (fighter &board, int pos, int turn);

/*	bishop move	*/
U32 BishopMove (fighter &board, int pos, int turn);
U32 BishopMove_h (fighter &board, int pos, int turn);

/*	read board	*/
//void ReadBoard(int *chessboard, fighter *board, int filenum);

/*	print	*/
void PrintBoard (U32 Board);
void PrintPV(line path, U32 from, U32 to);
void FPrintChessBoard (int *chessboard, FILE *fptr);
void PrintChessBoard (int *chessboard);
void SetColor (int);
void Information (FILE *fptr, double ttime, line pline);

/*	do move	*/
U16  DoMove        (U16 move, fighter *board, int *chessboard, int turn);
void UnDoMove      (U16 capture, U16 move, fighter *board, int *chessboard, int turn);
void DoHandMove	   (int src, fighter *board);
U8   SaveCapture   (int pce, fighter *board, int *chessboard);

/*	search	*/
//int Minimax      (line *, fighter *board, int *chessboard, int turn, int depth);
//int AlphaBetaMax (line *, fighter *board, int *chessboard, int alpha, int beta, int turn, int depth);
//int AlphaBetaMin (line *, fighter *board, int *chessboard, int alpha, int beta, int turn, int depth);
//int NegaMax      (line *, fighter *board, int *chessboard, int alpha, int beta, int turn, int depth);
//int ScoutMax     (line *, fighter *board, int *chessboard, int alpha, int beta, int turn, int depth);
//int ScoutMin     (line *, fighter *board, int *chessboard, int alpha, int beta, int turn, int depth);
int NegaScout    (line *, fighter *, int *, int , int , int , int , bool);
int QuiescenceSearch (fighter *board, int *chessboard, int alpha, int beta, int turn);
//bool Uchifuzume_Searching (fighter *board, int *chessboard, int turn, int depth);

/*	evaluating function	*/
int Evaluate (int *chessboard);

/*	initial	board	*/
void Inital (fighter *board, int *chessboard);

#endif