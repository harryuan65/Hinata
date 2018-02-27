#include "AI.h"

/*    Negascout Algorithm    */

//U32 max_move[3] = { 0 };

typedef void(*genmove)(Board &, Action *, U32 &);
static const genmove move_func[] = { AttackGenerator, MoveGenerator, HandGenerator };

Action IDAS(Board& board, PV &pv) {
    cout << "IDAS Searching " << Observer::depth << " Depth..." << endl;
	pv.leafEvaluate = NegaScout(pv, board, -CHECKMATE, CHECKMATE, Observer::depth, false);
    if (pv.leafEvaluate <= -CHECKMATE)
		return 0;
	return pv.action[0];
}

/*    Transposition Table    */
static TransPosition* transpositTable = nullptr;

void InitializeTP() {
#ifndef TRANSPOSITION_DISABLE
    transpositTable = new TransPosition[TPSize];
    cout << "TransPosition Table Created. ";
    cout << "Used Size : " << sizeof(TransPosition) << " * " << (TPSize >> 20);
    cout << "(MB) = " << ((TPSize * sizeof(TransPosition)) >> 20) << "MB\n";
#else
    cout << "TransPosition Table disable.\n";
#endif
}

int NegaScout(PV &pv, Board &board, int alpha, int beta, int depth, bool isResearch) {
	Observer::data[Observer::DataType::totalNode]++;
	Observer::data[Observer::DataType::researchNode] += isResearch;
	//Observer::data[Observer::DataType::scoutGeneNums]++;

#ifndef TRANSPOSITION_DISABLE
    U64 zobrist = board.GetZobristHash();
    U32 index = (board.GetTurn() << TPTurn) | (zobrist & TPMask);
    TransPosition *p = transpositTable + index;
    if (p->L_hash != (zobrist >> 32)) { // Collision
        Observer::data[Observer::DataType::indexCollisionNums]++;
    }
    else if (p->depth >= depth) { // hash hit!
        //Observer::data[Observer::DataType::totalTPDepth] += depth;
        switch (p->state) {
        case TransPosition::Exact:
            /* Exact */
            return p->value;
        case TransPosition::Unknown:
            /* Value在(-Infinity, value] */
            beta = min(p->value, beta);
            break;
        case TransPosition::FailHigh:
            /* Failed-High */
            alpha = max(p->value, alpha);
            break;
        }

        if (alpha >= beta)
            return p->value;
    }
#endif

    if (depth == 0) {
        pv.count = 0;
		//Observer::data[Observer::DataType::totalNode]--; //不然會重複計數
		return board.Evaluate();
        //return QuiescenceSearch(board, alpha, beta);
    }

    int bestScore = -CHECKMATE;
    int n = beta;
    PV tempPV;
    U32 accCnt = 0;

    /* 分三個步驟搜尋 [攻擊 移動 打入] */
    for (int i = 0; i < 3; i++) {
        Action moveList[MAX_MOVE_NUM];
        U32 cnt = 0;
        move_func[i](board, moveList, cnt);
        accCnt += cnt;

        for (U32 j = 0; j < cnt; ++j) {
			if (board.IsCheckAfter(ACTION_TO_SRCINDEX(moveList[j]), ACTION_TO_DSTINDEX(moveList[j]))) {
				accCnt--;
				continue;
			}

            board.DoMove(moveList[j]);
			if (board.IsSennichite() /*&& !(DoMove()前被將軍)*/ ) {
				/* 千日手 且 沒有被將軍 (連將時攻擊者判輸) TODO : 判斷是否被將軍 */
				board.UndoMove();
				accCnt--;
				continue;
			}
			int score = -NegaScout(tempPV, board, -n, -max(alpha, bestScore), depth - 1, isResearch);
			if (score > bestScore) {
				if (depth < 3 || score >= beta || n == beta)
					bestScore = score;
				else
					bestScore = -NegaScout(tempPV, board, -beta, -score + 1, depth - 1, true);
#ifndef PV_DISABLE
				pv.action[0] = moveList[j];
				pv.evaluate[0] = -board.Evaluate();
				memcpy(pv.action + 1, tempPV.action, tempPV.count * sizeof(Action));
				memcpy(pv.evaluate + 1, tempPV.evaluate, tempPV.count * sizeof(int));
				pv.count = tempPV.count + 1;
#else
				if (depth == Observer::depth) {
					pv.action[0] = moveList[j];
					pv.evaluate[0] = -board.Evaluate();
					memcpy(pv.action + 1, tempPV.action, tempPV.count * sizeof(Action));
					memcpy(pv.evaluate + 1, tempPV.evaluate, tempPV.count * sizeof(int));
					pv.count = tempPV.count + 1;
				}
#endif
			}
#ifndef PV_DISABLE
			else if (pv.count == 0 && score == -CHECKMATE) {
				/* moveList的第一個action是必輸的話照樣儲存pv 才能在必輸下得到pv */
				pv.action[0] = moveList[j];
				pv.evaluate[0] = -board.Evaluate();
				memcpy(pv.action + 1, tempPV.action, tempPV.count * sizeof(Action));
				memcpy(pv.evaluate + 1, tempPV.evaluate, tempPV.count * sizeof(int));
				pv.count = tempPV.count + 1;
			}
#endif
            board.UndoMove();

			if (bestScore >= beta) {
				/* Beta cutoff */
				Observer::data[Observer::DataType::scoutSearchBranch] += accCnt + j;
#ifndef TRANSPOSITION_DISABLE
                //if (depth < 3) {
                    p->L_hash = zobrist >> 32;
                    p->value = bestScore;
                    p->depth = depth;
                    p->state = TransPosition::FailHigh;
                //}
#endif
				return bestScore;
			}
			n = max(alpha, bestScore) + 1; // Set up a null window
        }
    }
	if (accCnt == 0) {
        pv.count = 0;
#ifdef BEST_ENDGAME_SEARCH
		bestScore = -CHECKMATE - 10 * depth;
#else
		bestScore = -CHECKMATE;
#endif
	}
	Observer::data[Observer::DataType::scoutSearchBranch] += accCnt;

#ifndef TRANSPOSITION_DISABLE
    //if (Observer::depth - 3 < depth && Observer::depth != depth) {
        p->L_hash = zobrist >> 32;
        p->value = bestScore;
        p->depth = depth;
        if (bestScore < alpha) {
            p->state = TransPosition::Unknown;
        }
        else {
            p->state = TransPosition::Exact;
        }
    //}
#endif
    return bestScore;
}

int QuiescenceSearch(Board& board, int alpha, int beta) {
	Observer::data[Observer::DataType::totalNode]++;
	Observer::data[Observer::DataType::quiesNode]++;

    int bestScore = board.Evaluate();
    if (bestScore >= beta) return bestScore;

	//int bestScore = -CHECKMATE;
    int n = beta;
	U32 accCnt = 0;

	/* 分三個步驟搜尋 [攻擊 移動 打入] */
	for (int i = 0; i < 3; i++) {
		Action moveList[MAX_MOVE_NUM];
		U32 cnt = 0;
		move_func[i](board, moveList, cnt);
		accCnt += cnt;

		for (U32 j = 0; j < cnt; ++j) {
			if (board.IsCheckAfter(ACTION_TO_SRCINDEX(moveList[j]), ACTION_TO_DSTINDEX(moveList[j]))
				/*|| !我是否被將軍 || !動完是否將軍對方(moveList[j]) || !(i == 0 && SEE >= 0)*/) {
				accCnt--;
				continue;
			}

            if (SEE(board, ACTION_TO_DSTINDEX(moveList[j])) >= 0) {
                /* Search Depth */
                board.DoMove(moveList[j]);

                int score = -QuiescenceSearch(board, -n, -max(alpha, bestScore));
                if (score > bestScore) {
                    if (score >= beta || n == beta)
                        bestScore = score;
                    else
                        bestScore = -QuiescenceSearch(board, -beta, -score + 1);
                }
                board.UndoMove();
                if (bestScore >= beta) {
                    return bestScore; // cut off
                }
                n = max(alpha, bestScore) + 1; // set up a null window
            }
		}
	}
    /*U32 cnt = 0;
	AttackGenerator(board, moveList, cnt);
    for (U32 i = 0; i < cnt; i++) {
        board.DoMove(moveList[i]);
        int score = -QuiescenceSearch(board, -n, -max(alpha, bestScore), 1 - turn);

        if (score > bestScore)
            bestScore = ((n == beta) || (score >= beta)) ? score : \
            - QuiescenceSearch(board, -beta, -score + 1, 1 - turn);

        board.UndoMove();
		if (bestScore >= beta) {
			return bestScore; // cut off
		}
        n = max(alpha, bestScore) + 1; // set up a null window
    }*/
	if (!accCnt) return -CHECKMATE;
    return bestScore;
}

int SEE(const Board &board, int dstIndex) {
    int exchangeScore = CHESS_SCORE[board.board[dstIndex]];
    int my = 0, op = 0;
    int myMoveChess[8], opMoveChess[8];

    // [ Start Add opMoveChess ]
    const U32 psbboard = (RookMove(board, dstIndex) | BishopMove(board, dstIndex));
    U32 srcboard = psbboard & board.occupied[board.GetTurn() ^ 1],
        dstboard = 1 << dstIndex;
    while (srcboard) {
        U32 attsrc = BitScan(srcboard);
        srcboard ^= 1 << attsrc;
        if (Movable(board, attsrc) & dstboard)
            opMoveChess[op++] = CHESS_SCORE[board.board[attsrc]];
    }
    if (op == 0) return board.GetTurn() ? -exchangeScore : exchangeScore;
    // [ End Add opMoveChess ]

    // [ Start myMoveChess ]
    srcboard = psbboard & board.occupied[board.GetTurn()];
    dstboard = 1 << dstIndex;
    while (srcboard) {
        U32 attsrc = BitScan(srcboard);
        srcboard ^= 1 << attsrc;
        if (Movable(board, attsrc) & dstboard)
            myMoveChess[my++] = CHESS_SCORE[board.board[attsrc]];
    }
    if (my == 0) return board.GetTurn() ? -exchangeScore : exchangeScore;
    // [ End myMoveChess ]

    // [ Start sort ]
    sort(opMoveChess, opMoveChess + op, [](const int &a, const int &b) { return abs(a) < abs(b); });
    sort(myMoveChess, myMoveChess + my, [](const int &a, const int &b) { return abs(a) < abs(b); });
    // [ End sort ]

    for (int op_i = 0, my_i = 0; op_i < op;) {
        exchangeScore += myMoveChess[my_i++];
        if (my_i < my)
            exchangeScore += opMoveChess[op_i++];
        else break;
    }
    // debug 用
    /*if (op > 2 && my > 2) {
    board.PrintChessBoard();
    cout << " to: " << dstIndex << " see: " << (board.GetTurn() ? -exchangeScore : exchangeScore) << endl << "my: ";
    for (int m : myMoveChess)
        cout << m << ", ";
    cout << endl << "op: ";
    for (int op : opMoveChess)
        cout << op << ", ";
    cout << endl;
    system("pause");
    }*/
    return board.GetTurn() ? -exchangeScore : exchangeScore;
}
