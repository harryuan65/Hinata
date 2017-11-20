#include "head.h"

bool isFailHigh;
U64 nodes = 0;
U64 failed_nodes = 0;
U64 leave_nodes = 0;

U16 input(int *chessboard, int turn) {
    int src = 0, dst = 0, eat = 0, pro = 0;
    fscanf_s(stdin, "%d %d %d", &src, &dst, &pro);
    if (!chessboard[src] || chessboard[src] >> 4 != turn) return 0;
    if (chessboard[dst]) eat = 1;
    return (pro << 13) | (eat << 12) | (dst << 6) | src;
}

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
    int pvEvaluate = 0;
    double durationTime = 0;
	Inital(&board, chessboard);

	PrintChessBoard(chessboard);
	printf("請選擇先後手 0: 先手 1: 後手 2: 膀胱者 \n");
	_flushall();
	int player;
    scanf_s("%d", &player); getchar();
	if (player != WHITE && player != BLACK && player != SPEC) return 0;
	
	double elapsed_secs = 0, accumulate = 0;
	while(true) {
        if (turn == player) {
            printf("Your turn!\n\a");
            _flushall();
            while (!(movebuf = input(chessboard, turn)))
                puts("Invalid Input! Please try again!");
            DoMove(movebuf, &board, chessboard, player);
            PrintChessBoard(chessboard);
        }
        else {
            printf("AI thinking...\n");

            clock_t begin = clock();
            if (movebuf != path.pv[++pvcount]) {
                pvcount = 0;
                pvEvaluate = NegaScout(&path, &board, chessboard, -INF, INF, turn, LIMIT_DEPTH, isFailHigh);
            }
            else
                ++pvcount;
            DoMove(path.pv[pvcount], &board, chessboard, turn);
            durationTime = double(clock() - begin) / CLOCKS_PER_SEC;

            // Print Result
            PrintPV(path, pvcount, path.pv_count);
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
            failed_nodes = 0;
            leave_nodes = 0;
        }
        turn ^= 1;
	}

	return 0;
}

