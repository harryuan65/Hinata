#include "head.h"
#include "main.h"

void CustomBoard(fighter board, int* chessboard) {
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

	SetFighter(board, chessboard);
}

int main (int argc, char *argv[])
{
	printf("DEBUG : 部分搜尋FailHigh\n");
	/*int chessboard[CHESS_BOARD_SIZE] = {BLANK};
	fighter board = {BLANK};
	char save[50];
	FILE *fptr;
	line path;

	_mkdir ("X:\\Hinata\\player1");
	sprintf(save, "X:\\Hinata\\player1\\pv_move.txt"); 
	fptr = fopen(save, "w");
	ReadBoard(chessboard, &board, 0);
	//PrintChessBoard(chessboard);
	nodes = 0;
	path.pv_count = 0;
	memset(path.pv, BLANK, (LIMIT_DEPTH + 1));
	int s = NegaScout(&path, &board, chessboard, -INF, INF, WHITE, LIMIT_DEPTH);
	for (int i = 0; i < path.pv_count; i++)
		fprintf(fptr, "%2d - %2d\n", path.pv[i] & SRC_MASK, (path.pv[i] & DST_MASK) >> 6);

	fclose(fptr);*/
	int chessboard[CHESS_BOARD_SIZE] = {BLANK};
	fighter board = {BLANK};
	line path;
    U16 movebuf = 4096;
	int turn = WHITE;
	int pvcount = 0;
	int player;
	int pvEvaluate = 0;
	double elapsed_secs = 0, accumulate = 0;

	//Inital(&board, chessboard);
	CustomBoard(board, chessboard);

	PrintChessBoard(chessboard);
	printf("請選擇先後手 (0)先手 (1)後手 (2)膀胱者 \n");
	//_flushall();
    scanf_s("%d", &player); getchar();
	if (player != WHITE && player != BLACK && player != SPEC)
		return 0;
	
	while(true) {
        if (turn == player) {
            char movestr[30];
            printf("Your turn!\n\a");
            _flushall();

			int src = 0, dst = 0, eat = 0, pro = 0;
			char srcStr[2], dstStr[2];
			scanf(" %2c", &srcStr);
			scanf(" %2c", &dstStr);
			scanf(" %d", &pro);
			src = boardpos2index(toupper(srcStr[0]), srcStr[1]);
			dst = boardpos2index(toupper(dstStr[0]), dstStr[1]);
			if (src == -1 || dst == -1 || pro > 1 || pro < 0) {
				fscanf_s(stdout, "Input Error\n");
				continue;
			}
			if (chessboard[dst]) 
				eat = 1;
			movebuf = (pro << 13) | (eat << 12) | (dst << 6) | src;
            DoMove(movebuf, &board, chessboard, player);
            PrintChessBoard(chessboard);
            turn ^= 1;
        }
        printf("AI thinking...\n");

        clock_t begin = clock();
		if (movebuf != path.pv[++pvcount] || pvcount > 6) {
            pvcount = 0;
			pvEvaluate = NegaScout(&path, &board, chessboard, -INF, INF, turn, LIMIT_DEPTH, false);
        } else
            ++pvcount;
		DoMove(path.pv[pvcount] , &board, chessboard, turn);
		accumulate += elapsed_secs = double(clock() - begin) / CLOCKS_PER_SEC;

		PrintChessBoard(chessboard);
        PrintPV(path, 0, path.pv_count);
		printf(
			"Total Nodes       : %11llu\n"
			"Failed-High Nodes : %11llu\n"
			"Leave Nodes       : %11llu\n"
			"Time              : %14.2lf\n"
			"Node/s            : %14.2lf\n"
			"Evaluate          : %11d\n"
			"PV leaf Evaluate  : %11d\n",
			nodes, failed_nodes, leave_nodes, elapsed_secs, 
			(elapsed_secs?nodes / elapsed_secs:0), Evaluate(chessboard), pvEvaluate);
        turn ^= 1;
		nodes = 0;
		failed_nodes = 0;
		leave_nodes = 0;
	}

	return 0;
}

