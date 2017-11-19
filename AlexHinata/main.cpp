#include "head.h"

bool isFullFailHigh;
U64 nodes;
U64 failed_nodes;
U64 leave_nodes;
U64 depth2_nodes;
HistoryHeuristic mHistoryHeur(true);
Transposition mTransposition(true);

void CustomBoard(Bitboard mBitboard, int* chessboard) {
	// 黑棋 上
	chessboard[D5] = ROOK | 0x10;
	chessboard[A1] = KING | 0x10;
	chessboard[B4] = SILVER | 0x10;
	chessboard[B2] = GOLD | 0x10;
	chessboard[B1] = PAWN | 0x10;

	// 白棋 下
	chessboard[D4] = SILVER;
	chessboard[D3] = GOLD;
	chessboard[D1] = ROOK;
	chessboard[E5] = KING;

	//手牌
	chessboard[F3] = BISHOP;
	chessboard[H3] = BISHOP | 0x10;
	chessboard[I2] = PAWN | 0x10;

	SetBitboard(mBitboard, chessboard);
}

int main (int argc, char *argv[])
{
	/*int chessboard[CHESS_BOARD_SIZE] = {BLANK};
	Bitboard mBitboard = {BLANK};
	char save[50];
	FILE *fptr;
	line path;

	_mkdir ("X:\\Hinata\\player1");
	sprintf(save, "X:\\Hinata\\player1\\pv_move.txt"); 
	fptr = fopen(save, "w");
	ReadBoard(chessboard, &mBitboard, 0);
	//PrintChessBoard(chessboard);
	nodes = 0;
	path.pv_count = 0;
	memset(path.pv, BLANK, (LIMIT_DEPTH + 1));
	int s = NegaScout(&path, &mBitboard, chessboard, -INF, INF, WHITE, LIMIT_DEPTH);
	for (int i = 0; i < path.pv_count; i++)
		fprintf(fptr, "%2d - %2d\n", path.pv[i] & SRC_MASK, (path.pv[i] & DST_MASK) >> 6);

	fclose(fptr);*/
	int chessboard[CHESS_BOARD_SIZE] = {BLANK};
	Bitboard mBitboard = {BLANK};
	line path;
    U16 action = 4096;
	bool turn = WHITE;
	int player[2];
	int step = 1;
	int pvCount = 0;
	int pvEvaluate = 0;
	double durationTime = 0;

	// Inital or Custom Board
	Inital(&mBitboard, chessboard);
	//CustomBoard(mBitboard, chessboard);

	printf("DEBUG : Full FailHigh?(true : 1/ false : 0)");
	scanf("%d", &isFullFailHigh);
	printf("請選擇先後手 (0)先手 (1)後手 (2)玩家對打 (3)電腦對打 \n");
    scanf("%d", &player[0]);
	switch (player[0])
	{
	case 0:
		player[0] = HUMAN; player[1] = AI;
		break;
	case 1:
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
				printf("輸入你的移動動作 (FromPos ToPos IsPromote) : ");
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
		}
		turn ^= 1;
		step++;
	}

	return 0;
}

