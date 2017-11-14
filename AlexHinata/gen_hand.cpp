#include "head.h"

void HandGenerator (int *start, fighter board, U16 *movelist, int turn)
{
	// start is movelist which it starts from.
	int all_moves = *start; // before doing HandGenerator, start is all moves
	bool once[5] = {false}; // 5 個棋種 { rook ; bishop; gold; silver; pawn }

	if (turn == WHITE)
	{
		U32 handboard = (U32)board.white_hand;
		U32 nifu = 0; // 二步
		U32 pboard = board.w_pawn;
		while (handboard)
		{
			U32 dstboard = blank_board;
			int src = BitScan(&handboard);
			switch (src)
			{
			case 0: // rook
				once[0] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 1: // rook
				if (once[0]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 2: // bishop
				once[1] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 3: // bishop
				if (once[1]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 4: // gold
				once[2] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 5: // gold
				if (once[2]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 6: // silver
				once[3] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 7: // silver
				if (once[3]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 8: // pawn
				once[4] = true;
				while (pboard)
				{
					int pos = BitScan(&pboard);
					nifu |= file_mask(pos);
				}
				dstboard &= ~(BLACK_CAMP | nifu) & Uchifuzume (board, WHITE);

				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 9: // pawn
				if (once[4]) break;
				while (pboard)
				{
					int pos = BitScan(&pboard);
					nifu |= file_mask(pos);
				}
				dstboard &= ~(BLACK_CAMP | nifu) & Uchifuzume (board, WHITE);

				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			} // switch
		} // while
	} // if 
	else
	{
		U32 handboard = (U32)board.black_hand;
		U32 nifu = 0; // 二步
		U32 pboard = board.b_pawn;
		while (handboard)
		{
			U32 dstboard = blank_board;
			int src = BitScan(&handboard);
			switch (src)
			{
			case 0: // rook
				once[0] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 1: // rook
				if (once[0]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 2: // bishop
				once[1] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 3: // bishop
				if (once[1]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 4: // gold
				once[2] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 5: // gold
				if (once[2]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 6: // silver
				once[3] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 7: // silver
				if (once[3]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 8: // pawn
				once[4] = true;
				while (pboard)
				{
					int pos = BitScan(&pboard);
					nifu |= file_mask(pos);
				}
				dstboard &= ~(WHITE_CAMP | nifu) & Uchifuzume (board, BLACK);

				while (dstboard)
				{
					int	dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 9: // pawn
				if (once[4]) break;
				while (pboard)
				{
					int pos = BitScan(&pboard);
					nifu |= file_mask(pos);
				}
				dstboard &= ~(WHITE_CAMP | nifu) & Uchifuzume (board, BLACK);

				while (dstboard)
				{
					int	dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			} // switch 
		} // while 
	} // else

	return ;
}

void HandGenerator_s (int *start, fighter board, int *chessboard, U16 *movelist, int turn)
{
	// start is movelist which it starts from.
	int all_moves = *start; // before doing HandGenerator, start is all moves
	bool once[5] = {false}; // 5 個棋種 { rook ; bishop; gold; silver; pawn }
	bool t;
	U32 temp = 0;

	if (turn == WHITE)
	{
		U32 handboard = (U32)board.white_hand;
		U32 nifu = 0; // 二步
		U32 pboard = board.w_pawn;
		while (handboard)
		{
			U32 dstboard = blank_board;
			int src = BitScan(&handboard);
			switch (src)
			{
			case 0: // rook
				once[0] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 1: // rook
				if (once[0]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 2: // bishop
				once[1] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 3: // bishop
				if (once[1]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 4: // gold
				once[2] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 5: // gold
				if (once[2]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 6: // silver
				once[3] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 7: // silver
				if (once[3]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 8: // pawn
				once[4] = true;
				while (pboard)
				{
					int pos = BitScan(&pboard);
					nifu |= file_mask(pos);
				}
				//dstboard &= ~(BLACK_CAMP | nifu) & Uchifuzume (board, WHITE);
				t = Uchifuzume_Searching(&board, chessboard, turn, DEPTH_UCHI);

				if (t)
				{
					int pos = 0;
					if (turn == WHITE)
					{
						temp = board.b_king;
						pos = BitScan(&temp);
						temp = 1 << (pos + 5);
					}
					else
					{
						temp = board.w_king;
						pos = BitScan(&temp);
						temp = 1 << (pos - 5);
					}
					dstboard &= ~(BLACK_CAMP | nifu) & ~temp;
				}
				else
					dstboard &= ~(BLACK_CAMP | nifu);

				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 9: // pawn
				if (once[4]) break;
				while (pboard)
				{
					int pos = BitScan(&pboard);
					nifu |= file_mask(pos);
				}
				//dstboard &= ~(BLACK_CAMP | nifu) & Uchifuzume (board, WHITE);
				t = Uchifuzume_Searching(&board, chessboard, turn, DEPTH_UCHI);

				if (t)
				{
					int pos = 0;
					if (turn == WHITE)
					{
						temp = board.b_king;
						pos = BitScan(&temp);
						temp = 1 << (pos + 5);
					}
					else
					{
						temp = board.w_king;
						pos = BitScan(&temp);
						temp = 1 << (pos - 5);
					}
					dstboard &= ~(BLACK_CAMP | nifu) & ~temp;
				}
				else
					dstboard &= ~(BLACK_CAMP | nifu);

				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			} // switch
		} // while
	} // if 
	else
	{
		U32 handboard = (U32)board.black_hand;
		U32 nifu = 0; // 二步
		U32 pboard = board.b_pawn;
		while (handboard)
		{
			U32 dstboard = blank_board;
			int src = BitScan(&handboard);
			switch (src)
			{
			case 0: // rook
				once[0] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 1: // rook
				if (once[0]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 2: // bishop
				once[1] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 3: // bishop
				if (once[1]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 4: // gold
				once[2] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 5: // gold
				if (once[2]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 6: // silver
				once[3] = true;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 7: // silver
				if (once[3]) break;
				while (dstboard)
				{
					int dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 8: // pawn
				once[4] = true;
				while (pboard)
				{
					int pos = BitScan(&pboard);
					nifu |= file_mask(pos);
				}
				//dstboard &= ~(WHITE_CAMP | nifu) & Uchifuzume (board, BLACK);
				t = Uchifuzume_Searching(&board, chessboard, turn, DEPTH_UCHI);

				if (t)
				{
					int pos = 0;
					if (turn == WHITE)
					{
						temp = board.b_king;
						pos = BitScan(&temp);
						temp = 1 << (pos + 5);
					}
					else
					{
						temp = board.w_king;
						pos = BitScan(&temp);
						temp = 1 << (pos - 5);
					}
					dstboard &= ~(WHITE_CAMP | nifu) & ~temp;
				}
				else
					dstboard &= ~(WHITE_CAMP | nifu);

				while (dstboard)
				{
					int	dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			case 9: // pawn
				if (once[4]) break;
				while (pboard)
				{
					int pos = BitScan(&pboard);
					nifu |= file_mask(pos);
				}
				//dstboard &= ~(WHITE_CAMP | nifu) & Uchifuzume (board, BLACK);
				t = Uchifuzume_Searching(&board, chessboard, turn, DEPTH_UCHI);

				if (t)
				{
					int pos = 0;
					if (turn == WHITE)
					{
						temp = board.b_king;
						pos = BitScan(&temp);
						temp = 1 << (pos + 5);
					}
					else
					{
						temp = board.w_king;
						pos = BitScan(&temp);
						temp = 1 << (pos - 5);
					}
					dstboard &= ~(WHITE_CAMP | nifu) & ~temp;
				}
				else
					dstboard &= ~(WHITE_CAMP | nifu);

				while (dstboard)
				{
					int	dst = BitScan(&dstboard);
					movelist[*start]     |= src + BOARD_SIZE + 10;
					movelist[(*start)++] |= dst << 6;
				}
				break;
			} // switch 
		} // while 
	} // else

	return ;
}

U32 Uchifuzume (fighter board, int turn)
{
	U32 ret = FULL_BOARD;
	U32 king_front_square, kmoves;
	U32 temp;
	int pos;

	if (turn == WHITE)
	{
		temp = board.b_king;
		pos = BitScan(&temp);
		if ((pos + 5) >= BOARD_SIZE) return ret;
		king_front_square = 1 << (pos + 5);
		kmoves = b_king_attack(pos) | b_king_movement(pos);
	}
	else
	{
		temp = board.w_king;
		pos = BitScan(&temp);
		if ((pos - 5) < 0) return ret;
		king_front_square = 1 << (pos - 5);
		kmoves = w_king_attack(pos) | w_king_movement(pos);
	}

	if ((~GenerateAllMoves(board, king_front_square, turn, ALL_MOVE) & kmoves) == 0)
		ret = ~king_front_square | GenerateAllMoves(board, king_front_square, 1 - turn, EXCEPT_KING);


	return ret;
}

U32 GenerateAllMoves (fighter board, U32 apawn, int turn, int type)
{
	U32 ret = 0;
	U32 tempboard;
	int pos;

	if (turn == WHITE)
	{
		if (type == ALL_MOVE) 
		{
			board.w_occupied |= apawn;
			*(&board.w_pawn + white_pce_bias[0]) |= apawn;
		}
		for (int i = 0; i < CHESS_SIZE; i++)
		{
			if (i == 5 && type == EXCEPT_KING)
				continue;
			tempboard = *(&board.w_pawn + white_pce_bias[i]);
			switch (i)
			{
			case 0:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= w_pawn_move[pos];
				}
				break;
			case 1:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= w_silver_move[pos];
				}
				break;
			case 2:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= w_gold_move[pos];
				}
				break;
			case 3:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= BishopMove_h(board, pos, WHITE) & (slope1_mask(pos) | slope2_mask(pos));
				}
				break;
			case 4:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= RookMove_h(board, pos, WHITE) & (rank_mask(pos) | file_mask(pos));
				}
				break;
			case 5:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= king_move[pos];
				}
				break;
			case 6:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= w_gold_move[pos];
				}
				break;
			case 7:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= w_gold_move[pos];
				}
				break;
			case 8:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= BishopMove_h(board, pos, WHITE) & (slope1_mask(pos) | slope2_mask(pos)) | king_move[pos];
				}
				break;
			case 9:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= RookMove_h(board, pos, WHITE) & (rank_mask(pos) | file_mask(pos)) | king_move[pos];
				}
				break;
			} // switch ()
		} // for ()
	}
	else
	{
		if (type == ALL_MOVE)
		{
			board.b_occupied |= apawn;
			*(&board.w_pawn + black_pce_bias[0]) |= apawn;
		}
		for (int i = 0; i < CHESS_SIZE; i++)
		{
			if (i == 5 && type == EXCEPT_KING)
				continue;
			tempboard = *(&board.w_pawn + black_pce_bias[i]);
			switch (i)
			{
			case 0:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= b_pawn_move[pos];
				}
				break;
			case 1:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= b_silver_move[pos];
				}
				break;
			case 2:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= b_gold_move[pos];
				}
				break;
			case 3:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= BishopMove_h(board, pos, BLACK) & (slope1_mask(pos) | slope2_mask(pos));
				}
				break;
			case 4:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= RookMove_h(board, pos, BLACK) & (rank_mask(pos) | file_mask(pos));
				}
				break;
			case 5:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= king_move[pos];
				}
				break;
			case 6:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= b_gold_move[pos];
				}
				break;
			case 7:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= b_gold_move[pos];
				}
				break;
			case 8:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= BishopMove_h(board, pos, BLACK) & (slope1_mask(pos) | slope2_mask(pos)) | king_move[pos];
				}
				break;
			case 9:
				while (tempboard)
				{
					pos  = BitScan(&tempboard);
					ret |= RookMove_h(board, pos, BLACK) & (rank_mask(pos) | file_mask(pos)) | king_move[pos];
				}
				break;
			} // switch ()
		} // for ()
	}

	return ret;
}

U32 RookMove_h (fighter board, int pos, int turn)
{
	// upper (find LSB) ; lower (find MSB)
	U32 ret = 0;
	U32 occupied = board.b_occupied | board.w_occupied;
	U32 rank, file;
	U32 upper, lower;
	int bitpos;

	// rank
	upper = (occupied & rank_upper[pos]) | HIGHTEST_BOARD_POS;
	lower = (occupied & rank_lower[pos]) | LOWEST_BOARD_POS;

	bitpos = BitScan(&upper);
	upper  = 1 << (bitpos + 1);

	bitpos = BitScanReverse(lower);
	lower  = 1 << bitpos;

	rank = (upper - lower) & rank_mask(pos);

	// file
	upper = (occupied & file_upper[pos]) | HIGHTEST_BOARD_POS;
	lower = (occupied & file_lower[pos]) | LOWEST_BOARD_POS;

	bitpos = BitScan(&upper);
	upper  = 1 << (bitpos + 1);

	bitpos = BitScanReverse(lower);
	lower  = 1 << bitpos;

	file = (upper - lower) & file_mask(pos);

	ret = rank | file;

	return ret;
}

U32 BishopMove_h (fighter board, int pos, int turn)
{
	// upper (find LSB) ; lower (find MSB)
	U32 ret = 0;
	U32 occupied = board.b_occupied | board.w_occupied;	
	U32 slope1, slope2;
	U32 upper, lower;
	int bitpos;

	// slope1 "/"
	upper = (occupied & slope1_upper[pos]) | HIGHTEST_BOARD_POS;
	lower = (occupied & slope1_lower[pos]) | LOWEST_BOARD_POS;

	bitpos = BitScan(&upper);
	upper = 1 << (bitpos + 1);

	bitpos = BitScanReverse(lower);
	lower = 1 << bitpos;

	slope1 = (upper - lower) & slope1_mask(pos);

	// slope2 "\"
	upper = (occupied & slope2_upper[pos]) | HIGHTEST_BOARD_POS;
	lower = (occupied & slope2_lower[pos]) | LOWEST_BOARD_POS;

	bitpos = BitScan(&upper);
	upper = 1 << (bitpos + 1);

	bitpos = BitScanReverse(lower);
	lower = 1 << bitpos;

	slope2 = (upper - lower) & slope2_mask(pos);

	ret = slope1 | slope2;

	return ret;
}

int UchifuzumeGenerator(fighter board, U16 *movelist, int turn, int type)
{
	int ret = 0;
	U32 srcboard, dstboard, temp;
	int pos = -1;

	if (type == PUT_PAWN_BACK)
	{
		if (turn == WHITE)
		{
			temp = board.b_king;
			pos = BitScan(&temp);
			if ((pos + 5) >= BOARD_SIZE) return ret;
			if ((board.white_hand & HAND_PAWN1) != 0) 
			{
				movelist[ret]   |= 33;
				movelist[ret++] |= (pos + 5) << 6;
			}
			else
			{
				movelist[ret]   |= 34;
				movelist[ret++] |= (pos + 5) << 6;
			}
		}
		else
		{
			temp = board.w_king;
			pos = BitScan(&temp);
			if ((pos - 5) < 0) return ret;
			if ((board.black_hand & HAND_PAWN1) != 0) 
			{
				movelist[ret]   |= 43;
				movelist[ret++] |= (pos - 5) << 6;
			}
			else
			{
				movelist[ret]   |= 44;
				movelist[ret++] |= (pos - 5) << 6;
			}
		}
	}
	else
	{
		AttackGenerator(&ret, board, movelist, turn);

		if (turn == WHITE)
		{
			srcboard = board.w_king;
			int src = BitScan(&srcboard);
			dstboard = w_king_movement(src);
			while (dstboard)
			{
				int dst = BitScan(&dstboard); // LSB
				movelist[ret]   |= src;
				movelist[ret++] |= dst << 6;
			}
		}
		else
		{
			srcboard = board.b_king;
			int src = BitScan(&srcboard);
			dstboard = b_king_movement(src);
			while (dstboard)
			{
				int dst = BitScanReverse(dstboard); // MSB
				movelist[ret]   |= src;
				movelist[ret++] |= dst << 6;
				dstboard ^= 1 << dst;
			}
		}
	}

	return ret;
}