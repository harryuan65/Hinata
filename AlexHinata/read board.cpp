#include "head.h"

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

void ReadBoard(int *chessboard, fighter *board, int filenum)
{
	// can read check ok!!!
	// resolve chessboard to bitboard
	//    white  <- | 16 | ->  black
	#define COLOR_CHANGE   16
	#define COLOR_BLACK  0x10  // 16
	FILE *fptr;
	char buffer[50];
	int temp = 0;
	int counter = 0;
	//sprintf(buffer, "D:\\sample.txt"); // home
	//sprintf(buffer, "D:\\¬ã¨s«Ç\\shogi\\test board\\board%05d.txt", filenum); // school
	//sprintf(buffer, "X:\\Hinata\\test board\\board%05d.txt", filenum); // test
	sprintf_s(buffer, "X:\\Hinata\\testing_battle\\board.txt"); // test
	fptr = fopen(buffer, "r");

	if (!fptr) 
	{
		printf ("can't find file!\n");
		return ;
	}

	while (fscanf(fptr, "%d", &temp) == 1)
	{
		chessboard[counter] = temp; 
		if (temp == BLANK)
		{
			counter++;
			continue;
		}
		if (chessboard[counter] > COLOR_CHANGE) // black
		{
			if (counter < BOARD_SIZE) // board
			{
				switch (chessboard[counter])
				{
				case (KING | COLOR_BLACK):
					{
						board->b_king |= 1 << counter;
						break;
					}
				case (GOLD | COLOR_BLACK):
					{
						board->b_gold |= 1 << counter;
						break;
					}
				case (SILVER | COLOR_BLACK):
					{
						board->b_silver |= 1 << counter;
						break;
					}
				case (PAWN | COLOR_BLACK):
					{
						board->b_pawn |= 1 << counter;
						break;
					}
				case (BISHOP | COLOR_BLACK):
					{
						board->b_bishop |= 1 << counter;
						break;
					}
				case (ROOK | COLOR_BLACK):
					{
						board->b_rook |= 1 << counter;
						break;
					}
				case (eSILVER | COLOR_BLACK):
					{
						board->b_e_silver |= 1 << counter;
						break;
					}
				case (ePAWN | COLOR_BLACK):
					{
						board->b_e_pawn |= 1 << counter;
						break;
					}
				case (eBISHOP | COLOR_BLACK):
					{
						board->b_e_bishop |= 1 << counter;
						break;
					}
				case (eROOK | COLOR_BLACK):
					{
						board->b_e_rook |= 1 << counter;
						break;
					}
				} // switch ()
				board->b_occupied |= 1 << counter;
			}
			else // hand
			{
				switch (chessboard[counter])
				{
				case (GOLD | COLOR_BLACK):
					{
						int p = counter - BOARD_SIZE - 10;
						board->black_hand |= 1 << p;
						break;
					}
				case (SILVER | COLOR_BLACK):
					{
						int p = counter - BOARD_SIZE - 10;
						board->black_hand |= 1 << p;
						break;
					}
				case (PAWN | COLOR_BLACK):
					{
						int p = counter - BOARD_SIZE - 10;
						board->black_hand |= 1 << p;
						break;
					}
				case (BISHOP | COLOR_BLACK):
					{
						int p = counter - BOARD_SIZE - 10;
						board->black_hand |= 1 << p;
						break;
					}
				case (ROOK | COLOR_BLACK):
					{
						int p = counter - BOARD_SIZE - 10;
						board->black_hand |= 1 << p;
						break;
					}
				} // switch ()
			}
		}
		else // white
		{
			if (counter < BOARD_SIZE) // board
			{
				switch (chessboard[counter])
				{
				case KING:
					{
						board->w_king |= 1 << counter;
						break;
					}
				case GOLD:
					{
						board->w_gold |= 1 << counter;
						break;
					}
				case SILVER:
					{
						board->w_silver |= 1 << counter;
						break;
					}
				case PAWN:
					{
						board->w_pawn |= 1 << counter;
						break;
					}
				case BISHOP:
					{
						board->w_bishop |= 1 << counter;
						break;
					}
				case ROOK:
					{
						board->w_rook |= 1 << counter;
						break;
					}
				case eSILVER:
					{
						board->w_e_silver |= 1 << counter;
						break;
					}
				case ePAWN:
					{
						board->w_e_pawn |= 1 << counter;
						break;
					}
				case eBISHOP:
					{
						board->w_e_bishop |= 1 << counter;
						break;
					}
				case eROOK:
					{
						board->w_e_rook |= 1 << counter;
						break;
					}
				} // switch ()
				board->w_occupied |= 1 << counter;
			}
			else // hand
			{
				switch (chessboard[counter])
				{
				case GOLD:
					{
						int p = counter - BOARD_SIZE;
						board->white_hand |= 1 << p;
						break;
					}
				case SILVER:
					{
						int p = counter - BOARD_SIZE;
						board->white_hand |= 1 << p;
						break;
					}
				case PAWN:
					{
						int p = counter - BOARD_SIZE;
						board->white_hand |= 1 << p;
						break;
					}
				case BISHOP:
					{
						int p = counter - BOARD_SIZE;
						board->white_hand |= 1 << p;
						break;
					}
				case ROOK:
					{
						int p = counter - BOARD_SIZE;
						board->white_hand |= 1 << p;
						break;
					}
				} // switch ()
			}
		} // if () ... else () ...
		counter++;
	}
	
	#undef COLOR_BLACK
	#undef COLOR_CHANGE
	fclose(fptr);
	return ;
}