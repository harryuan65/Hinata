#include "head.h"

U64 max_move = 0;
U64 count = 0;
double total_branch;
double search_branch;
typedef void(*genmove_t)(int *, fighter, U16 *, int);
const genmove_t genmove_func[] = { AttackGenerator, MoveGenerator, HandGenerator };

int NegaScout (line *pline, fighter *board, int *chessboard, int alpha, int beta, int turn, int depth) {
	line path;
	// using fail soft with negamax:
	// terminal 
	if (board->w_king == 0 || board->b_king == 0) {
		pline->pv_count = 0;
		return -CHECKMATE;
	}
	if (depth == 0) {
		pline->pv_count = 0;
		//return (turn == WHITE) ? Evaluate(chessboard) : -Evaluate(chessboard); // evaluating function
		return QuiescenceSearch(board, chessboard, alpha, beta, turn);
	}

	int bestscore = -INF;
	int n = beta;
    ++nodes;

    for (int i = 0; i < 3; ++i) {
        int cnt = 0;
        U16 movelist[128] = { 0 };
        genmove_func[i](&cnt, *board, movelist, turn);
        total_branch += cnt;
        if (cnt > max_move)
            printf("%d:%lld ", i, (max_move = cnt));

        for (int j = 0; j < cnt; ++j) {
            ++search_branch;
            U16  capture = DoMove(movelist[j], board, chessboard, turn);
            int score = -NegaScout(&path, board, chessboard, -n, -max(alpha, bestscore), 1 - turn, depth - 1);
            if (score > bestscore) {
                bestscore = ((n == beta) || (depth < 3) || (score >= beta)) ? score : \
                    - NegaScout(&path, board, chessboard, -beta, -score + 1, 1 - turn, depth - 1);

                pline->pv[0] = movelist[j];
                memcpy(pline->pv + 1, path.pv, path.pv_count * sizeof(U16));
                pline->pv_count = path.pv_count + 1;
            }
            UnDoMove(capture, movelist[j], board, chessboard, turn);
            if (bestscore >= beta) return bestscore; // cut off
            n = max(alpha, bestscore) + 1; // set up a null window
        }
    }

	return bestscore;
}

int ScoutMax(line *pline, fighter *board, int *chessboard, int alpha, int beta, int turn, int depth)
{
	line path;
	// terminal 
	if (board->w_king == 0)
	{
		pline->pv_count = 0;
		return -CHECKMATE;
	}
	if (depth == 0)
	{
		pline->pv_count = 0;
		return Evaluate(chessboard); // evaluating function
	}

	int bestscore = -INF;
	int n = beta;
	U16 movelist[MAX_MOVE_NUM] = {0};
	int cnt = Generator(*board, movelist, turn);

	for (int i = 0; i < cnt; i++)
	{
		nodes++;
		U16 capture = DoMove(movelist[i], board, chessboard, turn);
		int score = ScoutMin(&path, board, chessboard, max(alpha, bestscore), n, 1 - turn, depth - 1);
		if (score > bestscore)
		{
			bestscore = ((n == beta) || (depth < 3) || (score >= beta)) ? score : \
				ScoutMin(&path, board, chessboard, score - 1, beta, 1 - turn, depth - 1);
			pline->pv[0] = movelist[i];
			memcpy(pline->pv + 1, path.pv, path.pv_count * sizeof(U16));
			pline->pv_count = path.pv_count + 1;
		}
		UnDoMove(capture, movelist[i], board, chessboard, turn);
		if (bestscore >= beta) return bestscore; // cut off
		n = max(alpha, bestscore) + 1;
	}

	return bestscore;
}

int ScoutMin(line *pline, fighter *board, int *chessboard, int alpha, int beta, int turn, int depth)
{
	line path;
	// terminal 
	if (board->b_king == 0)
	{
		pline->pv_count = 0;
		return CHECKMATE;
	}
	if (depth == 0)
	{
		pline->pv_count = 0;
		return -Evaluate(chessboard); // evaluating function
	}

	int bestscore = INF;
	int n = alpha;
	U16 movelist[MAX_MOVE_NUM] = {0};
	int cnt = Generator(*board, movelist, turn);

	for (int i = 0; i < cnt; i++)
	{
		nodes++;
		U16 capture = DoMove(movelist[i], board, chessboard, turn);
		int score = ScoutMax(&path, board, chessboard, n, min(beta, bestscore), 1 - turn, depth - 1);
		if (score < bestscore)
		{
			bestscore = ((n == alpha) || (depth < 3) || (score <= alpha)) ? score : \
				ScoutMax(&path, board, chessboard, alpha, score + 1, 1 - turn, depth - 1);
			pline->pv[0] = movelist[i];
			memcpy(pline->pv + 1, path.pv, path.pv_count * sizeof(U16));
			pline->pv_count = path.pv_count + 1;
		}
		UnDoMove(capture, movelist[i], board, chessboard, turn);
		if (bestscore <= alpha) return bestscore; // cut off
		n = min(beta, bestscore) - 1;
	}

	return bestscore;
}