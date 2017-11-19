#include "head.h"

U64 max_move = 0;

//bool isFirstFlag = true; // Debug
typedef void(*genmove_t)(int *, Bitboard, U16 *, int);
const genmove_t genmove_func[] = { AttackGenerator, MoveGenerator, HandGenerator };

int NegaScout(line *mPVAction, Bitboard *mBitboard, int *chessboard, int alpha, int beta, int turn, int depth, bool isFailHigh) {
	// using fail soft with negamax:
	int bestscore = -INF, nullAlpha = beta;
	line tmpPVAction;
	nodes++;
	failed_nodes += isFailHigh;

	// terminal case
	// 王已死 回傳
	if (mBitboard->w_king == 0 || mBitboard->b_king == 0) {
		mPVAction->pv_count = 0;
		if (!isFailHigh)
			leave_nodes++;
		return -CHECKMATE;
	}
	// 深度為0 回傳
	if (depth == 0) {
		mPVAction->pv_count = 0;
		if (!isFailHigh)
			leave_nodes++;
		return QuiescenceSearch(mBitboard, chessboard, alpha, beta, turn);
	}

	// 分三個步驟搜尋 [攻擊 移動 打入]
    for (int i = 0; i < 3; i++) {
        int cnt = 0;
        U16 movelist[128] = { 0 };
		// Generation Function
        genmove_func[i](&cnt, *mBitboard, movelist, turn);
		// Sort by History Heuristic
		mHistoryHeur.SortByTable(movelist, cnt);

        if (cnt > max_move) // DEBUG:
            printf("%d:%lld ", i, (max_move = cnt));

		// 對所有移動可能做搜尋
        for (int j = 0; j < cnt; j++) {
            U16 capture = DoMove(movelist[j], mBitboard, chessboard, turn);
			int score = -NegaScout(&tmpPVAction, mBitboard, chessboard, -nullAlpha, -max(alpha, bestscore), 1 - turn, depth - 1, isFailHigh);
			if (depth == 1) { // DEBUG:
				printf("%d\nullAlpha", score);
			}
			// Failed-High
            if (score > bestscore) {
				if (nullAlpha == beta || (depth < 3 && !isFullFailHigh) || score >= beta) {
					bestscore = score;
				}
				else {
					bestscore = -NegaScout(&tmpPVAction, mBitboard, chessboard, -beta, -score + 1, 1 - turn, depth - 1, true);
				}
                mPVAction->pv[0] = movelist[j];
                memcpy(mPVAction->pv + 1, tmpPVAction.pv, tmpPVAction.pv_count * sizeof(U16));
                mPVAction->pv_count = tmpPVAction.pv_count + 1;
            }
            UnDoMove(capture, movelist[j], mBitboard, chessboard, turn);
			// Cut off
			if (bestscore >= beta) {
				// Update History Heuristic Table
				int index = Action2Index(movelist[j]);
				if (mHistoryHeur.table[index] >> 31) {
					mHistoryHeur.OverflowAvoid();
				}
				mHistoryHeur.table[index] += 1 << depth;
				return bestscore;
			}
			// Set up a null window
            nullAlpha = max(alpha, bestscore) + 1;
        }
    }
	return bestscore;
}

int ScoutMax(line *mPVAction, Bitboard *mBitboard, int *chessboard, int alpha, int beta, int turn, int depth)
{
	line tmpPVAction;
	// terminal 
	if (mBitboard->w_king == 0)
	{
		mPVAction->pv_count = 0;
		return -CHECKMATE;
	}
	if (depth == 0)
	{
		mPVAction->pv_count = 0;
		return Evaluate(chessboard); // evaluating function
	}

	int bestscore = -INF;
	int nullAlpha = beta;
	U16 movelist[MAX_MOVE_NUM] = {0};
	int cnt = Generator(*mBitboard, movelist, turn);

	for (int i = 0; i < cnt; i++)
	{
		nodes++;
		U16 capture = DoMove(movelist[i], mBitboard, chessboard, turn);
		int score = ScoutMin(&tmpPVAction, mBitboard, chessboard, max(alpha, bestscore), nullAlpha, 1 - turn, depth - 1);
		if (score > bestscore)
		{
			bestscore = ((nullAlpha == beta) || (depth < 3) || (score >= beta)) ? score : \
				ScoutMin(&tmpPVAction, mBitboard, chessboard, score - 1, beta, 1 - turn, depth - 1);
			mPVAction->pv[0] = movelist[i];
			memcpy(mPVAction->pv + 1, tmpPVAction.pv, tmpPVAction.pv_count * sizeof(U16));
			mPVAction->pv_count = tmpPVAction.pv_count + 1;
		}
		UnDoMove(capture, movelist[i], mBitboard, chessboard, turn);
		if (bestscore >= beta) return bestscore; // cut off
		nullAlpha = max(alpha, bestscore) + 1;
	}

	return bestscore;
}

int ScoutMin(line *mPVAction, Bitboard *mBitboard, int *chessboard, int alpha, int beta, int turn, int depth)
{
	line tmpPVAction;
	// terminal 
	if (mBitboard->b_king == 0)
	{
		mPVAction->pv_count = 0;
		return CHECKMATE;
	}
	if (depth == 0)
	{
		mPVAction->pv_count = 0;
		return -Evaluate(chessboard); // evaluating function
	}

	int bestscore = INF;
	int nullAlpha = alpha;
	U16 movelist[MAX_MOVE_NUM] = {0};
	int cnt = Generator(*mBitboard, movelist, turn);

	for (int i = 0; i < cnt; i++)
	{
		nodes++;
		U16 capture = DoMove(movelist[i], mBitboard, chessboard, turn);
		int score = ScoutMax(&tmpPVAction, mBitboard, chessboard, nullAlpha, min(beta, bestscore), 1 - turn, depth - 1);
		if (score < bestscore)
		{
			bestscore = ((nullAlpha == alpha) || (depth < 3) || (score <= alpha)) ? score : \
				ScoutMax(&tmpPVAction, mBitboard, chessboard, alpha, score + 1, 1 - turn, depth - 1);
			mPVAction->pv[0] = movelist[i];
			memcpy(mPVAction->pv + 1, tmpPVAction.pv, tmpPVAction.pv_count * sizeof(U16));
			mPVAction->pv_count = tmpPVAction.pv_count + 1;
		}
		UnDoMove(capture, movelist[i], mBitboard, chessboard, turn);
		if (bestscore <= alpha) return bestscore; // cut off
		nullAlpha = min(beta, bestscore) - 1;
	}

	return bestscore;
}