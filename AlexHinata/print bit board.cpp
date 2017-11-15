#include "head.h"
#include <Windows.h>

void PrintBoard(U32 Board)
{
	Board &= 0x1ffffff;
	int BinaryNum[BOARD_SIZE] = { 0 };
	int i = 0, j = 0;
	U32 bitTmp = Board;
	// �ഫ���G���X
	while (bitTmp > 1)
	{
		BinaryNum[i++] = bitTmp % 2;
		bitTmp = bitTmp / 2;
	}
	BinaryNum[i] = bitTmp;
	printf("%ld\n", Board);
	for (i = 0; i <= 20; i += 5)
	{
		for (j = 0; j <= 4; j++)
			printf("%d ", BinaryNum[i + j]);
		printf("\n");
	}
	printf("\n");
	return;
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

void Information(FILE *fptr, double ttime, line pline)
{
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
	return;
}

void FPrintChessBoard(int *chessboard, FILE *fptr)
{
	for (int i = 0; i < CHESS_BOARD_SIZE; i++)
		fprintf(fptr, "%c%2d ", (i % 5 ? ' ' : '\n'), chessboard[i]);
	fprintf(fptr, "\n\n");
}

#define hConsole GetStdHandle(STD_OUTPUT_HANDLE)
inline void SetColor(int color = 7)
{
	SetConsoleTextAttribute(hConsole, color);
}


void PrintPV(line path, U32 from, U32 to) {
	char src[3] = "  ", dst[3]= "  ";
	for (U32 i = from; i < to; ++i) {
		index2boardpos(path.pv[i] & SRC_MASK, src);
		index2boardpos((path.pv[i] & DST_MASK) >> 6, dst);
		printf("%2d : %s -> %s Eat: %s Pro: %s , %s\n",
			i, src, dst,
			(path.pv[i] & EAT_MASK) ? "Y" : "N",
			(path.pv[i] >> 13) ? "Y" : "N",
			(i & 1) ? "White" : "Black");
	}
}

void PrintChessBoard(int *chessboard)
{
	char *rank_name[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I" };
	int rank_count = 0;
	int board_count = 0;
	SetColor();
	puts(" ");//*
	printf("%2s�U", " ");//*
	SetColor(15);
	printf("%2d", 5);
	SetColor();
	printf("�U");
	SetColor(15);
	printf("%2d", 4);
	SetColor();
	printf("�U");
	SetColor(15);
	printf("%2d", 3);
	SetColor();
	printf("�U");
	SetColor(15);
	printf("%2d", 2);
	SetColor();
	printf("�U");
	SetColor(15);
	printf("%2d", 1);
	SetColor();
	puts("�U");
	for (int i = 0; i < 9; i++)
	{
		puts("�X�U�X�U�X�U�X�U�X�U�X�U�X");
		if (i == 5)
		{
			printf("%2s�U", " ");  //*
			SetColor(15);
			printf("%2d", 5);//*
			SetColor();
			printf("�U");
			SetColor(15);
			printf("%2d", 4);
			SetColor();
			printf("�U");
			SetColor(15);
			printf("%2d", 3);
			SetColor();
			printf("�U");
			SetColor(15);
			printf("%2d", 2);
			SetColor();
			printf("�U");
			SetColor(15);
			printf("%2d", 1);//*
			SetColor();
			puts("�U");
			puts(" \n");
		}
		SetColor(15);
		printf("%2s", rank_name[rank_count]);
		SetColor();
		for (int j = 0; j < 5; j++) //128 = �� ; 143 = ��  ;207 = �����զr  ; 192 = �����¦r
		{
			printf("�U");
			switch (chessboard[board_count])
			{
			case BLANK:
				printf("%2s", " ");
				break;
			case 1: // white pawn
				SetColor(143);
				printf("%2s", "�B");
				break;
			case 2: // white silver
				SetColor(143);
				printf("%2s", "��");
				break;
			case 3: // white gold
				SetColor(143);
				printf("%2s", "��");
				break;
			case 4: // white bishop
				SetColor(143);
				printf("%2s", "��");
				break;
			case 5: // white rook
				SetColor(143);
				printf("%2s", "��");
				break;
			case 6: // white king
				SetColor(143);//�զ�r �{�b��¤F
				printf("%2s", "��");
				break;
			case 9: // white e_pawn
				SetColor(207);
				printf("%2s", "�w");
				break;
			case 10: // white e_silver
				SetColor(207);
				printf("%2s", "��");
				break;
			case 12: // white e_bishop
				SetColor(207);
				printf("%2s", "��");
				break;
			case 13: // white e_rook
				SetColor(207);
				printf("%2s", "�s");
				break;
			case 17: // black pawn
				SetColor(128);
				printf("%2s", "�B");
				break;
			case 18: // black silver
				SetColor(128);
				printf("%2s", "��");
				break;
			case 19: // black gold
				SetColor(128);
				printf("%2s", "��");
				break;
			case 20: // black bishop
				SetColor(128); //�¦�r? �{�b�o�ڧ令�դF
				printf("%2s", "��");
				break;
			case 21: // black rook
				SetColor(128);
				printf("%2s", "��");
				break;
			case 22: // black king
				SetColor(128);
				printf("%2s", "��");
				break;
			case 25: // black e_pawn
				SetColor(192);
				printf("%2s", "�w");
				break;
			case 26: // black e_silver
				SetColor(192);
				printf("%2s", "��");
				break;
			case 28: // black e_bishop
				SetColor(192);
				printf("%2s", "��");
				SetColor();
				break;
			case 29: // black e_rook
				SetColor(192);
				printf("%2s", "�s");
				break;
			}
			SetColor();
			board_count++;
		}
		printf("�U");
		SetColor(15);
		printf("%2s", rank_name[rank_count++]);//*
		/*SetColor();
		printf("\t ");
		int temp = board_count - 1;
		for (int j = temp - 4; j <= temp; j++)
		{
			printf("�U");
			SetColor(95);
			printf("%2d", j);
			SetColor();
		}
		printf("�U");
		*/
		puts(" ");		//*
	}
	puts(" ");			//*

	return;
}

int boardpos2index(char row, char col) {
	if ((int)row >= (int)'A' && (int)row <= (int)'I' && col >= '1' && col <= '5') {
		return (int)(row - 'A') * 5 + 4 - col + '1';
	}
	return -1;
}

bool index2boardpos(int index, char* pos) {
	if (index >= 0 && index <= 44) {
		pos[0] = 'A' + (int)(index / 5);
		pos[1] = '1' + 4 - index % 5;
		return true;
	}
	return false;
}