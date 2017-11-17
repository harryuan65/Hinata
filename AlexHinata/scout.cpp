#include "head.h"

U64 max_move = 0;
U64 count = 0;

//bool isFirstFlag = true; // Debug
typedef void(*genmove_t)(int *, Bitboard, U16 *, int);
const genmove_t genmove_func[] = { AttackGenerator, MoveGenerator, HandGenerator };

int NegaScout(line *pvAction, Bitboard *mBitboard, int *chessboard, int alpha, int beta, int turn, int depth, bool isFailHigh) {
	/*if (isFirstFlag && isFailHigh){
		printf("FirstDepth : %d\n", depth);
		isFirstFlag = false;
	}*/
	if (isFailHigh && depth == 2){
		depth2_nodes++;
	}
	line path;
	// using fail soft with negamax:
	// terminal 
	nodes++;
	failed_nodes += isFailHigh;
	// 王已死 回傳
	if (mBitboard->w_king == 0 || mBitboard->b_king == 0) {
		pvAction->pv_count = 0;
		if (!isFailHigh)
			leave_nodes++;
		return -CHECKMATE;
	}
	// 深度為0 回傳
	if (depth == 0) {
		pvAction->pv_count = 0;
		if (!isFailHigh)
			leave_nodes++;
		//return (turn == WHITE) ? Evaluate(chessboard) : -Evaluate(chessboard); // evaluating function
		return QuiescenceSearch(mBitboard, chessboard, alpha, beta, turn);
	}
	int bestscore = -INF;
	int n = beta;
	// <Test>
	/*if (depth == 3 && firstFlag) {
		PrintChessBoard(chessboard);
	}*/
	// </Test>

	// 分三個步驟搜尋 [攻擊 移動 打入]
    for (int i = 0; i < 3; i++) {
        int cnt = 0;
        U16 movelist[128] = { 0 };
        genmove_func[i](&cnt, *mBitboard, movelist, turn);
        if (cnt > max_move) //Debug
            printf("%d:%lld ", i, (max_move = cnt));

		// 對所有移動可能做搜尋
        for (int j = 0; j < cnt; j++) {
            U16 capture = DoMove(movelist[j], mBitboard, chessboard, turn);
			int score = -NegaScout(&path, mBitboard, chessboard, -n, -max(alpha, bestscore), 1 - turn, depth - 1, isFailHigh);
            if (score > bestscore) {
                bestscore = ((n == beta) || (depth < 3 && !isFullFailHigh) || (score >= beta)) ? score : \
                        -NegaScout(&path, mBitboard, chessboard, -beta, -score + 1, 1 - turn, depth - 1, true);
                pvAction->pv[0] = movelist[j];
                memcpy(pvAction->pv + 1, path.pv, path.pv_count * sizeof(U16));
                pvAction->pv_count = path.pv_count + 1;
            }
            UnDoMove(capture, movelist[j], mBitboard, chessboard, turn);
			// Cut off
			if (bestscore >= beta) {
				//firstFlag = false; //Debug
				return bestscore;
			}
			// Set up a null window
            n = max(alpha, bestscore) + 1;
        }
    }
	//firstFlag = false; //Debug
	return bestscore;
}

int ScoutMax(line *pvAction, Bitboard *mBitboard, int *chessboard, int alpha, int beta, int turn, int depth)
{
	line path;
	// terminal 
	if (mBitboard->w_king == 0)
	{
		pvAction->pv_count = 0;
		return -CHECKMATE;
	}
	if (depth == 0)
	{
		pvAction->pv_count = 0;
		return Evaluate(chessboard); // evaluating function
	}

	int bestscore = -INF;
	int n = beta;
	U16 movelist[MAX_MOVE_NUM] = {0};
	int cnt = Generator(*mBitboard, movelist, turn);

	for (int i = 0; i < cnt; i++)
	{
		nodes++;
		U16 capture = DoMove(movelist[i], mBitboard, chessboard, turn);
		int score = ScoutMin(&path, mBitboard, chessboard, max(alpha, bestscore), n, 1 - turn, depth - 1);
		if (score > bestscore)
		{
			bestscore = ((n == beta) || (depth < 3) || (score >= beta)) ? score : \
				ScoutMin(&path, mBitboard, chessboard, score - 1, beta, 1 - turn, depth - 1);
			pvAction->pv[0] = movelist[i];
			memcpy(pvAction->pv + 1, path.pv, path.pv_count * sizeof(U16));
			pvAction->pv_count = path.pv_count + 1;
		}
		UnDoMove(capture, movelist[i], mBitboard, chessboard, turn);
		if (bestscore >= beta) return bestscore; // cut off
		n = max(alpha, bestscore) + 1;
	}

	return bestscore;
}

int ScoutMin(line *pvAction, Bitboard *mBitboard, int *chessboard, int alpha, int beta, int turn, int depth)
{
	line path;
	// terminal 
	if (mBitboard->b_king == 0)
	{
		pvAction->pv_count = 0;
		return CHECKMATE;
	}
	if (depth == 0)
	{
		pvAction->pv_count = 0;
		return -Evaluate(chessboard); // evaluating function
	}

	int bestscore = INF;
	int n = alpha;
	U16 movelist[MAX_MOVE_NUM] = {0};
	int cnt = Generator(*mBitboard, movelist, turn);

	for (int i = 0; i < cnt; i++)
	{
		nodes++;
		U16 capture = DoMove(movelist[i], mBitboard, chessboard, turn);
		int score = ScoutMax(&path, mBitboard, chessboard, n, min(beta, bestscore), 1 - turn, depth - 1);
		if (score < bestscore)
		{
			bestscore = ((n == alpha) || (depth < 3) || (score <= alpha)) ? score : \
				ScoutMax(&path, mBitboard, chessboard, alpha, score + 1, 1 - turn, depth - 1);
			pvAction->pv[0] = movelist[i];
			memcpy(pvAction->pv + 1, path.pv, path.pv_count * sizeof(U16));
			pvAction->pv_count = path.pv_count + 1;
		}
		UnDoMove(capture, movelist[i], mBitboard, chessboard, turn);
		if (bestscore <= alpha) return bestscore; // cut off
		n = min(beta, bestscore) - 1;
	}

	return bestscore;
}