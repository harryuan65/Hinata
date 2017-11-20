#include "head.h"
#include <Windows.h>

void PrintBoard(U32 Board) {
    for (int i = 0, j; i < 25; i += 5) {
        for (j = 0; j < 5; ++j)
            printf("%d ", Board >> (i + j) & 1);
        puts("");
    }
    puts("");
}

void PrintPV(line path, U32 from, U32 to) {
    for (U32 i = from; i < to; ++i)
        printf("%2d : %2d -> %2d eat: %s pro: %s , %s\n",
            i,
            path.pv[i] & SRC_MASK,
            (path.pv[i] & DST_MASK) >> 6,
            (path.pv[i] & EAT_MASK) ? "Y" : "N",
            (path.pv[i] >> 13) ? "Y" : "N",
            (i & 1) ? "white" : "black");
}

/*
pawn     =     1      |      17
silver   =     2      |      18
gold     =     3      |      19
bishop   =     4      |      20
rook     =     5      |      21
king     =     6      |      22
e_pawn   =     9      |      25
e_silver =     10     |      26
e_bishop =     12     |      28
e_rook   =     13     |      29

	         white          black
*/

void Information (FILE *fptr, double ttime, line pline) {
	printf("time is %.4lf sec.\n", ttime);
	fprintf(fptr, "time is %.4lf sec.\n", ttime);
	printf("total nodes are %lld.\n", nodes);
	fprintf(fptr, "total nodes are %lld.\n", nodes);
	puts("pv :");
	fputs("pv :", fptr);
	fprintf(fptr, "\t%2d - %2d , %s\n", pline.pv[0] & SRC_MASK, (pline.pv[0] & DST_MASK) >> 6, (0 & 1) ? "black" : "white");
	/*for (int i = 0; i < pline.pv_count; i++)
	{
	printf("\t%2d - %2d , %s\n", pline.pv[i] & SRC_MASK, (pline.pv[i] & DST_MASK) >> 6, (i & 1) ? "black" : "white");
	fprintf(fptr, "\t%2d - %2d , %s\n", pline.pv[i] & SRC_MASK, (pline.pv[i] & DST_MASK) >> 6, (i & 1) ? "black" : "white");
	}*/
	puts(" \n");
	fputs(" \n", fptr);
	return ;
}

void FPrintChessBoard(int *chessboard, FILE *fptr) {
	for (int i = 0; i < CHESS_BOARD_SIZE; ++i)
		fprintf(fptr, "%c%2d ", (i % 5 ? ' ' : '\n'), chessboard[i]);
	fprintf(fptr, "\n\n");
}

static const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
inline void SetColor(int color = 8) {
	SetConsoleTextAttribute(hConsole, color);
}

static const char *rank_name[] = { "Ａ", "Ｂ", "Ｃ", "Ｄ", "Ｅ", "Ｆ", "  ", "Ｇ", "  " };
static const char *showchess[] = {
    "  ", "步", "銀", "金", "角", "飛", "玉", "  ",
    "  ", "ㄈ", "全", "  ", "馬", "龍", "  ", "  ",
    "  ", "步", "銀", "金", "角", "飛", "王", "  ",
    "  ", "ㄈ", "全", "  ", "馬", "龍",
};
void PrintChessBoard(int *chessboard) {
    int chess;
    int rank_count = 0;
    int board_count = 0;

    SetColor();
    puts("\n  ｜５｜４｜３｜２｜１｜");
    for (int i = 0; i < 9; ++i) {
        puts("—｜—｜—｜—｜—｜—｜—");
        if (i == 5) puts("  ｜５｜４｜３｜２｜１｜\n");
        
        printf("%s", rank_name[rank_count]);
        for (int j = 0; j < 5; ++j) {
            printf("｜");
            chess = chessboard[board_count++];
            // 12 = 黑 ; 10 = 白 ; 11 = 白升變 ; 13 = 黑升變
            if (chess & BLACKCHESS) {
                if (chess & PROMOTE) SetColor(13);
                else SetColor(12);
            }
            else {
                if (chess & PROMOTE) SetColor(11);
                else SetColor(10);
            }

            printf("%s", showchess[chess]);
            SetColor();
        }
        printf("｜%s\n", rank_name[rank_count++]);
    }
    puts("");
}