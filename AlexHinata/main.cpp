#include "head.h"
#include "main.h"

int main (int argc, char *argv[])
{
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
	int turn = WHITE;
    U16 movebuf = 4096;
	int pvcount = 0;
	Inital(&board, chessboard);
	/*board.w_occupied = 3246080;
    board.b_occupied = 8210;
    board.b_pawn =     0;
    board.b_silver =   0;
    board.b_gold =     0;
    board.b_bishop =   0;
    board.b_rook =     2;
    board.b_king =     16;
    board.b_e_pawn =   0;
    board.b_e_silver = 0;
    board.b_e_bishop = 0;
    board.b_e_rook =   8192;
         
    board.w_pawn = W_PAWN_INIT;
	board.w_silver = 67584;
    board.w_gold = W_GOLD_INIT;
    board.w_bishop = 0;
    board.w_rook = 0;
    board.w_king = W_KING_INIT;
    board.w_e_pawn = 0;
    board.w_e_silver = 0;
    board.w_e_bishop = 0;
    board.w_e_rook = 0;

    board.black_hand = 284;
    board.white_hand = 0;

    chessboard[A4] = ROOK | 16;
    chessboard[A1] = KING | 16;
    chessboard[C2] = ROOK | 16 | 8;

    chessboard[C4] = SILVER;
    chessboard[D4] = SILVER;
    chessboard[E4] = GOLD;
    chessboard[E5] = KING;
    chessboard[D5] = PAWN;

    chessboard[H3] = BISHOP | 16;
    chessboard[H2] = BISHOP | 16;
    chessboard[H1] = GOLD | 16;
    chessboard[I2] = PAWN | 16;*/

	PrintChessBoard(chessboard);
	printf("請選擇先後手 0: 先手 1: 後手 2: 膀胱者 \n");
	_flushall();
	int player;
    scanf_s("%d", &player); getchar();
	if (player != WHITE && player != BLACK && player != SPEC)
		return 0;
	
	double elapsed_secs = 0, accumulate = 0;
	while(true) {
        if (turn == player) {
            char movestr[30];
            printf("Your turn!\n\a");
            _flushall();

			int src = 0, dst = 0, eat = 0, pro = 0;
			char srcStr[2], dstStr[2];
			fscanf_s(stdin, "%2c", &srcStr);
			fscanf_s(stdin, " %2c", &dstStr);
			fscanf_s(stdin, " %d", &pro);
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
        if (movebuf != path.pv[++pvcount]) {
            pvcount = 0;
            NegaScout(&path, &board, chessboard, -INF, INF, turn, LIMIT_DEPTH);
        } else
            ++pvcount;
		DoMove(path.pv[pvcount] , &board, chessboard, turn);
		accumulate += elapsed_secs = double(clock() - begin) / CLOCKS_PER_SEC;

		PrintChessBoard(chessboard);
        PrintPV(path, 0, path.pv_count);
		printf("nodes : %llu\n"
			"total search branch: %.0lf\n"
			"avg branch: %lf\n"
			"avg search branch: %lf\n"
			"time: %lf\n"
			"node/s: %lf\n"
			"Evaluate: %d\n",
			nodes, search_branch, total_branch / nodes, search_branch / nodes,
			elapsed_secs, nodes / accumulate, Evaluate(chessboard));
        turn ^= 1;
	}

	return 0;
}

