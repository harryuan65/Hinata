#include "AI.h"

typedef void(*genmove)(Board &, Action *, U32 &);
static const genmove move_func[] = { AttackGenerator, MoveGenerator, HandGenerator };

map<U32, TranspositNode> transpositTable;

Action IDAS(Board& board, bool turn, PV &pv) {
    cout << "IDAS Searching " << Observer::IDAS_TEMP_DEPTH << " Depth..." << endl;
	pv.leafEvaluate = NegaScout(pv, board, -INT_MAX, INT_MAX, Observer::IDAS_TEMP_DEPTH, turn, false);
    if (pv.count == 0/* || pv.leafEvaluate <= -CHECKMATE*/)
		return 0;
    if (pv.leafEvaluate <= -CHECKMATE || pv.leafEvaluate >= CHECKMATE)
        Observer::IDAS_TEMP_DEPTH--;
	return pv.action[0];
}

int NegaScout(PV &pv, Board &board, int alpha, int beta, int depth, int turn, bool isFailHigh) {
	Observer::totalNode++; //如果進入寧靜搜尋要-- 不然會重複計數
	Observer::failedHighNode += isFailHigh;
    // using fail soft with negamax:
    // terminal
    if (depth == 0) {
        pv.count = 0;
        return board.Evaluate();/*QuiescenceSearch(board, alpha, beta);*/
    }

    int bestScore = -INT_MAX;
    int n = beta;
    PV tempPV;
    U32 accCnt = 0;
	Observer::scoutGeneNums++;
    /* 分三個步驟搜尋 [攻擊 移動 打入] */
    for (int i = 0; i < 3; i++) {
        Action moveList[MAX_MOVE_NUM];
        U32 cnt = 0;
        move_func[i](board, moveList, cnt);
        accCnt += cnt;

        for (U32 j = 0; j < cnt; ++j) {
            if (board.IsSennichite(moveList[j]) ||
                i != 2 && board.IsCheckingAfter(ACTION_TO_SRCINDEX(moveList[j]), ACTION_TO_DSTINDEX(moveList[j]))) {
                accCnt--;
                continue;
            }

            /* Search Depth */
            board.DoMove(moveList[j]);

            int score = -NegaScout(tempPV, board, -n, -max(alpha, bestScore), depth - 1, turn ^ 1, isFailHigh);
            if (score > bestScore) {
                if (depth < 3 || score >= beta || n == beta)
                    bestScore = score;
                else
                    bestScore = -NegaScout(tempPV, board, -beta, -score + 1, depth - 1, turn ^ 1, true);

                pv.action[0] = moveList[j];
				pv.evaluate[0] = -board.Evaluate();
                memcpy(pv.action + 1, tempPV.action, tempPV.count * sizeof(Action));
				memcpy(pv.evaluate + 1, tempPV.evaluate, tempPV.count * sizeof(int));
                pv.count = tempPV.count + 1;
            }
            else if (score == -INT_MAX && n == beta) {
                pv.action[0] = moveList[j];
                pv.evaluate[0] = -board.Evaluate();
                memcpy(pv.action + 1, tempPV.action, tempPV.count * sizeof(Action));
                memcpy(pv.evaluate + 1, tempPV.evaluate, tempPV.count * sizeof(int));
                pv.count = tempPV.count + 1;
            }

            board.UndoMove();
			if (bestScore >= beta) {
				Observer::scoutSearchBranch += accCnt + j;
				return bestScore; // cut off
			}
			n = max(alpha, bestScore) + 1; // set up a null window
        }
    }
	if (!accCnt) {
		pv.count = 0;
        return -CHECKMATE - (depth << 3);
	}
	Observer::scoutSearchBranch += accCnt;
    return bestScore;
}

int QuiescenceSearch(Board &board, int alpha, int beta) {
    // terminal
	Observer::totalNode++;
	Observer::quieNode++;
    //if (!board.bitboard[KING] || !board.bitboard[KING | BLACKCHESS])
    //    return -CHECKMATE;

    int bestScore = board.Evaluate();
    if (bestScore >= beta) return bestScore;

    int n = beta;
    U32 moveList[MAX_MOVE_NUM];
    U32 cnt = 0;
    AttackGenerator(board, moveList, cnt);
    MoveGenerator(board, moveList, cnt);

    for (U32 i = 0; i < cnt; i++) {
        if (board.IsCheckingAfter(ACTION_TO_SRCINDEX(moveList[i]), ACTION_TO_DSTINDEX(moveList[i])))
            continue;
        //if (SEE(board, ACTION_TO_DSTINDEX(moveList[i])) > 0) {
            board.DoMove(moveList[i]);
            int score = -QuiescenceSearch(board, -n, -max(alpha, bestScore));

            if (score > bestScore)
                bestScore = ((n == beta) || (score >= beta)) ? score : \
                - QuiescenceSearch(board, -beta, -score + 1);

            board.UndoMove();
            if (bestScore >= beta) {
                return bestScore; // cut off
            }
            n = max(alpha, bestScore) + 1; // set up a null window
        //}
    }
    return bestScore;
}

//TODO : 改成negascout 加同形表?
//int QuiescenceSearch(const Board &board, int alpha, int beta) {
//	int bestScore = board.Evaluate();
//	if (bestScore >= beta) return bestScore;
//
//	Action moveList[MAX_MOVE_NUM];
//	int cnt = 0;
//	//TODO : moveGene 吃掉 將軍 解將軍
//
//	for (int i = 0; i < cnt; i++) {
//		if (ACTION_TO_DSTCHESS(moveList[i]) || SEE(board, ACTION_TO_DSTINDEX(moveList[i])) > 0) {
//			board.DoMove(moveList[i]);
//			bestScore = max(bestScore, -QuiescenceSearch(board, max(-beta, bestScore), -alpha));
//			board.UndoMove();
//			if (bestScore >= beta)
//				return bestScore;
//		}
//	}
//	
//	return bestScore;
//}

int SEE(const Board &board, int dstIndex) {
    int exchangeScore = CHESS_SCORE[board.board[dstIndex]];
	vector<int> myMoveChess, opMoveChess;

    // [ Start Add opMoveChess ]
    const U32 psbboard = (RookMove(board, dstIndex) | BishopMove(board, dstIndex));
    U32 srcboard = psbboard & board.occupied[board.GetTurn() ^ 1],
        dstboard = 1 << dstIndex;
    while (srcboard) {
        U32 attsrc = BitScan(srcboard);
        srcboard ^= 1 << attsrc;
        if (Movable(board, attsrc) & dstboard)
            opMoveChess.push_back(CHESS_SCORE[board.board[attsrc]]);
    }
	if (opMoveChess.size() == 0) return board.GetTurn() ? -exchangeScore : exchangeScore;
    // [ End Add opMoveChess ]

    // [ Start myMoveChess ]
    srcboard = psbboard & board.occupied[board.GetTurn()];
    dstboard = 1 << dstIndex;
    while (srcboard) {
        U32 attsrc = BitScan(srcboard);
        srcboard ^= 1 << attsrc;
        if (Movable(board, attsrc) & dstboard)
            myMoveChess.push_back(CHESS_SCORE[board.board[attsrc]]);
    }
	if (myMoveChess.size() == 0) return board.GetTurn() ? -exchangeScore : exchangeScore;
    // [ End myMoveChess ]

    // [ Start sort ]
    sort(opMoveChess.begin(), opMoveChess.end(), [](const int &a, const int &b) { return abs(a) < abs(b); });
    sort(myMoveChess.begin(), myMoveChess.end(), [](const int &a, const int &b) { return abs(a) < abs(b); });
    // [ End sort ]

    std::vector<int>::iterator op = opMoveChess.begin();
    std::vector<int>::iterator my = myMoveChess.begin();
    for (; op != opMoveChess.end(); op++) {
        exchangeScore += *my;
        if (++my != myMoveChess.end())
            exchangeScore += *op;
        else break;
    }
    // debug 用
    /*if (opMoveChess.size() > 2 && myMoveChess.size() > 2) {
        board.PrintChessBoard();
        cout << " to: " << dstIndex << " see: " << (board.GetTurn() ? -exchangeScore : exchangeScore) << endl;
        system("pause");
    }*/
	return board.GetTurn() ? -exchangeScore : exchangeScore;
}

bool ReadTransposit(U32 hashcode, TranspositNode& bestNode) {
	if (transpositTable.find(hashcode) != transpositTable.end()) {
		bestNode = transpositTable[hashcode];
		//TODO: if board.IsMovable(bestNode.bestAction) collision avoid
		return true;
	}
	return false;
}

void UpdateTransposit(U32 hashcode, int score, bool isExact, U32 depth, Action action) {
	map<U32, TranspositNode>::iterator it = transpositTable.find(hashcode);
	if (it != transpositTable.end() && depth >= ACTION_TO_DEPTH(it->second.bestAction)) {
		it->second = TranspositNode(score, isExact, depth, action);
	}
}
