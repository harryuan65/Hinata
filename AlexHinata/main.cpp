#include "head.h"

bool isFullFailHigh = false;
U64 nodes;
U64 failed_nodes;
U64 leave_nodes;
U64 depth2_nodes;
HistoryHeuristic mHistoryHeur;
Transposition mTransposition;

void CustomBoard(Bitboard mBitboard, int* chessboard) {
	// �´� �W
	chessboard[D5] = ROOK | 0x10;
	chessboard[A1] = KING | 0x10;
	chessboard[B4] = SILVER | 0x10;
	chessboard[B2] = GOLD | 0x10;
	chessboard[B1] = PAWN | 0x10;

	// �մ� �U
	chessboard[D4] = SILVER;
	chessboard[D3] = GOLD;
	chessboard[D1] = ROOK;
	chessboard[E5] = KING;

	//��P
	chessboard[F3] = BISHOP;
	chessboard[H3] = BISHOP | 0x10;
	chessboard[I2] = PAWN | 0x10;

	SetBitboard(mBitboard, chessboard);
}

int main (int argc, char *argv[])
{
	int chessboard[CHESS_BOARD_SIZE] = {BLANK};
	Bitboard mBitboard = {BLANK};
	line path;
    U16 action = 4096;
	bool turn = WHITE;
	int player[2];
	int step = 1;
	int pvCount = 0;
	int pvEvaluate = 0;
	int gameMode;
	double durationTime = 0;
	HWND hwnd;

	// Inital or Custom Board
	Inital(&mBitboard, chessboard);
	//CustomBoard(mBitboard, chessboard);

	//printf("DEBUG : Full FailHigh?(true : 1/ false : 0)");
	//scanf("%d", &isFullFailHigh);
	bool boolbuf;
	printf("DEBUG : isHistoryEnable?(true : 1/ false : 0)");
	scanf("%d", &boolbuf);
	mHistoryHeur.m_isEnable = boolbuf;
	printf("DEBUG : isHistoryUpdate?(true : 1/ false : 0)");
	scanf("%d", &boolbuf);
	mHistoryHeur.m_isUpdate = boolbuf;
	printf("�п�ܹ��:\n(0)���avs�q��\n(1)�q��vs���a\n(2)���a�若\n(3)�q���若\n(4)�q���若 ����vs��L�{��\n(5)�q���若 ��L�{��vs����\n");
    scanf("%d", &gameMode);
	if (gameMode == 4 || gameMode == 5) {
		printf("�A��hwnd�O %d\n", GetConsoleWindow());
		printf("�п�J�ؼе{����hwnd : ");
		scanf("%d", &hwnd);
		/*if (!hwnd) {
			cout << "FindWindow Failed" << endl;
			system("pause");
			return 0;
		}*/
	}
	switch (gameMode)
	{
	case 0:
	case 5:
		player[0] = HUMAN; player[1] = AI;
		break;
	case 1:
	case 4:
		player[0] = AI; player[1] = HUMAN;
		break;
	case 2:
		player[0] = HUMAN; player[1] = HUMAN;
		break;
	case 3:
		player[0] = AI; player[1] = AI;
		break;
	default:
		return 0;
		break;
	}
	
	while (1) {
		printf("\n---------Step %d Player %d turn---------\n", step, turn);
		PrintChessBoard(chessboard);
		printf("Zobrist Number %lu\n", mTransposition.ZobristHashing(chessboard));
		if (player[turn] == HUMAN) {
			int src = 0, dst = 0, eat = 0, pro = 0;
			char srcStr[2], dstStr[2];

			while(1) {
				printf("��J�A�����ʰʧ@ (FromPos ToPos IsPromote) : ");
				_flushall();
				scanf(" %2c", &srcStr);
				scanf(" %2c", &dstStr);
				scanf(" %d", &pro);
				src = boardpos2index(toupper(srcStr[0]), srcStr[1]);
				dst = boardpos2index(toupper(dstStr[0]), dstStr[1]);
				if (src != -1 && dst != -1 && pro <= 1 && pro >= 0) {
					break;
				}
				printf("Input Error\n");
			}
			if (chessboard[dst]) 
				eat = 1;
			action = (pro << 13) | (eat << 12) | (dst << 6) | src;
            DoMove(action, &mBitboard, chessboard, turn);
        }
		else {
			printf("AI thinking...\n");

			// AI moving
			clock_t begin = clock();
			if (action != path.pv[++pvCount] || pvCount > 6) {
				pvCount = 0;
				pvEvaluate = NegaScout(&path, &mBitboard, chessboard, -INF, INF, turn, LIMIT_DEPTH, false);
			}
			else{
				pvCount++;
			}
			DoMove(path.pv[pvCount], &mBitboard, chessboard, turn);
			durationTime = double(clock() - begin) / CLOCKS_PER_SEC;

			// Print Result
			printf("\n");
			PrintPV(path, pvCount, path.pv_count);
			//printf("Depth2 Failed-High Node : %d\n", depth2_nodes);
			printf(
				"Total Nodes       : %11llu\n"
				"Failed-High Nodes : %11llu\n"
				"Leave Nodes       : %11llu\n"
				"Time              : %14.2lf\n"
				"Node/s            : %14.2lf\n"
				"Evaluate          : %11d\n"
				"PV leaf Evaluate  : %11d\n",
				nodes, failed_nodes, leave_nodes, durationTime,
				(durationTime ? nodes / durationTime : 0), Evaluate(chessboard), pvEvaluate);
			nodes = 0;
			//depth2_nodes = 0;
			failed_nodes = 0;
			leave_nodes = 0;
			mHistoryHeur.SaveTable();
			if (gameMode == 4 || gameMode == 5) {
				char src[3] = "  ", dst[3] = "  ";
				index2boardpos(path.pv[pvCount] & SRC_MASK, src);
				index2boardpos((path.pv[pvCount] & DST_MASK) >> 6, dst);
				PostMessage(hwnd, WM_KEYDOWN, src[0], 0);
				PostMessage(hwnd, WM_KEYDOWN, src[1], 0);
				PostMessage(hwnd, WM_KEYDOWN, VK_SPACE, 0);
				PostMessage(hwnd, WM_KEYDOWN, dst[0], 0);
				PostMessage(hwnd, WM_KEYDOWN, dst[1], 0);
				PostMessage(hwnd, WM_KEYDOWN, VK_SPACE, 0);
				PostMessage(hwnd, WM_KEYDOWN, (bool)(path.pv[pvCount] >> 13) + '0', 0);
				PostMessage(hwnd, WM_KEYDOWN, VK_RETURN, 0);
			}
			printf("\a");
		}
		turn ^= 1;
		step++;
	}

	return 0;
}

