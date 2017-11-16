#include "head.h"

// white use LSB
// black use MSB
// 利用 (key目前沒加)(小 -> 大)吃(大 -> 小) | (大 -> 小)移動(小 -> 大)
// 14 bits  pro  eat  dst  src
//		 	 1    1    6    6

typedef void (*genmove_t)(int *, fighter, U16 *, int);

int Generator (fighter board, U16 *movelist, int turn)
{
	genmove_t genmove_func[3] = {AttackGenerator, MoveGenerator, HandGenerator}; 

	//  attack -> move -> hand
	int ret = 0;	

	for (int i = 0; i < 3; i++)
		genmove_func[i](&ret, board, movelist, turn);

	return ret;
}

void AttackGenerator (int *start, fighter board, U16 *movelist, int turn)
{
	// start is movelist which it starts from.
	if (turn == WHITE)
	{
		for (int i = 0; i < CHESS_SIZE; i++)
		{
			U32 srcboard = 0, dstboard = 0;
			switch (attack_ordering[i])
			{
			case KING:
				{
					srcboard = board.w_king;
					int src = BitScan(&srcboard);
					dstboard = w_king_attack(src);
					while (dstboard)
					{
						int dst = BitScan(&dstboard); // LSB
						movelist[*start]     |= src;
						movelist[*start]     |= dst << 6;
						movelist[(*start)++] |= EAT_MASK;
					}
					break;
				}
			case GOLD:
				{
					if (board.w_gold == 0) continue; // no chess
					srcboard = board.w_gold;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_gold_attack(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
						}
					}
					break;
				}
			case SILVER:
				{
					if (board.w_silver == 0) continue; // no chess
					srcboard = board.w_silver;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_silver_attack(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
							PromoteGenerator(start, src, dst, movelist, turn, true);
						}
					}
					break;
				}
			case PAWN:
				{
					if (board.w_pawn == 0) continue; // no chess
					srcboard = board.w_pawn;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_pawn_attack(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							if (dst < 5)
							{
								movelist[*start]     |= src;
								movelist[*start]     |= dst << 6;
								movelist[*start]     |= EAT_MASK;
								movelist[(*start)++] |= PRO_MASK;
							}
							else
							{
								movelist[*start]     |= src;
								movelist[*start]     |= dst << 6;
								movelist[(*start)++] |= EAT_MASK;
							}
						}
					}
					break;
				}
			case BISHOP:
				{
					if (board.w_bishop == 0) continue; // no chess
					srcboard = board.w_bishop;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_bishop_attack(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							PromoteGenerator(start, src, dst, movelist, turn, true);
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
						}
					}
					break;
				}
			case ROOK:
				{
					if (board.w_rook == 0) continue; // no chess
					srcboard = board.w_rook;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_rook_attack(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							PromoteGenerator(start, src, dst, movelist, turn, true);
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
						}
					}
					break;
				}
			case eSILVER:
				{
					if (board.w_e_silver == 0) continue; // no chess
					srcboard = board.w_e_silver;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_gold_attack(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
						}
					}
					break;
				}
			case ePAWN:
				{
					if (board.w_e_pawn == 0) continue; // no chess
					srcboard = board.w_e_pawn;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_gold_attack(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
						}
					}
					break;
				}
			case eBISHOP:
				{
					if (board.w_e_bishop == 0) continue; // no chess
					srcboard = board.w_e_bishop;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_e_bishop_attack(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
						}
					}
					break;
				}
			case eROOK:
				{
					if (board.w_e_rook == 0) continue; // no chess
					srcboard = board.w_e_rook;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_e_rook_attack(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
						}
					}
					break;
				}
			} // switch
		} // for 
	} // if 
	else
	{
		for (int i = 0; i < CHESS_SIZE; i++)
		{
			U32 srcboard = 0, dstboard = 0;
			switch (attack_ordering[i])
			{
			case KING:
				{
					srcboard = board.b_king;
					int src = BitScan(&srcboard);
					dstboard = b_king_attack(src);
					while (dstboard)
					{
						int dst = BitScanReverse(dstboard); // MSB
						movelist[*start]     |= src;
						movelist[*start]     |= dst << 6;
						movelist[(*start)++] |= EAT_MASK;
						dstboard ^= 1 << dst;
					}
					break;
				}
			case GOLD:
				{
					if (board.b_gold == 0) continue; // no chess
					srcboard = board.b_gold;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_gold_attack(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case SILVER:
				{
					if (board.b_silver == 0) continue; // no chess
					srcboard = board.b_silver;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_silver_attack(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
							PromoteGenerator(start, src, dst, movelist, turn, true);
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case PAWN:
				{
					if (board.b_pawn == 0) continue; // no chess
					srcboard = board.b_pawn;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_pawn_attack(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							if (dst > 19 && dst < BOARD_SIZE)
							{
								movelist[*start]     |= src;
								movelist[*start]     |= dst << 6;
								movelist[*start]     |= EAT_MASK;
								movelist[(*start)++] |= PRO_MASK;
							}
							else
							{
								movelist[*start]     |= src;
								movelist[*start]     |= dst << 6;
								movelist[(*start)++] |= EAT_MASK;
							}
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case BISHOP:
				{
					if (board.b_bishop == 0) continue; // no chess
					srcboard = board.b_bishop;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_bishop_attack(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							PromoteGenerator(start, src, dst, movelist, turn, true);
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case ROOK:
				{
					if (board.b_rook == 0) continue; // no chess
					srcboard = board.b_rook;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_rook_attack(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							PromoteGenerator(start, src, dst, movelist, turn, true);
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case eSILVER:
				{
					if (board.b_e_silver == 0) continue; // no chess
					srcboard = board.b_e_silver;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_gold_attack(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case ePAWN:
				{
					if (board.b_e_pawn == 0) continue; // no chess
					srcboard = board.b_e_pawn;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_gold_attack(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case eBISHOP:
				{
					if (board.b_e_bishop == 0) continue; // no chess
					srcboard = board.b_e_bishop;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_e_bishop_attack(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case eROOK:
				{
					if (board.b_e_rook == 0) continue; // no chess
					srcboard = board.b_e_rook;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_e_rook_attack(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							movelist[*start]     |= src;
							movelist[*start]     |= dst << 6;
							movelist[(*start)++] |= EAT_MASK;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			} // switch
		} // for
	} // else

	return ;
}

void MoveGenerator (int *start, fighter board, U16 *movelist, int turn)
{
	// start is movelist which it starts from.
	if (turn == WHITE)
	{
		for (int i = 0; i < CHESS_SIZE; i++)
		{
			U32 srcboard = 0, dstboard = 0;
			switch (move_ordering[i])
			{
			case KING:
				{
					srcboard = board.w_king;
					int src = BitScan(&srcboard);
					dstboard = w_king_movement(src);
					while (dstboard)
					{
						int dst = BitScan(&dstboard); // LSB
						movelist[*start]     |= src;
						movelist[(*start)++] |= dst << 6;
					}
					break;
				}
			case GOLD:
				{
					if (board.w_gold == 0) continue; // no chess
					srcboard = board.w_gold;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_gold_movement(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
						}
					}
					break;
				}
			case SILVER:
				{
					if (board.w_silver == 0) continue; // no chess
					srcboard = board.w_silver;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_silver_movement(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
							PromoteGenerator(start, src, dst, movelist, turn, false);
						}
					}
					break;
				}
			case PAWN:
				{
					if (board.w_pawn == 0) continue; // no chess
					srcboard = board.w_pawn;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_pawn_movement(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							if (dst < 5)
							{
								movelist[*start]     |= src;
								movelist[*start]     |= dst << 6;
								movelist[(*start)++] |= PRO_MASK;
							}
							else
							{
								movelist[*start]     |= src;
								movelist[(*start)++] |= dst << 6;
							}
						}
					}
					break;
				}
			case BISHOP:
				{
					if (board.w_bishop == 0) continue; // no chess
					srcboard = board.w_bishop;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_bishop_movement(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							PromoteGenerator(start, src, dst, movelist, turn, false);
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
						}
					}
					break;
				}
			case ROOK:
				{
					if (board.w_rook == 0) continue; // no chess
					srcboard = board.w_rook;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_rook_movement(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							PromoteGenerator(start, src, dst, movelist, turn, false);
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
						}
					}
					break;
				}
			case eSILVER:
				{
					if (board.w_e_silver == 0) continue; // no chess
					srcboard = board.w_e_silver;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_gold_movement(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
						}
					}
					break;
				}
			case ePAWN:
				{
					if (board.w_e_pawn == 0) continue; // no chess
					srcboard = board.w_e_pawn;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_gold_movement(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
						}
					}
					break;
				}
			case eBISHOP:
				{
					if (board.w_e_bishop == 0) continue; // no chess
					srcboard = board.w_e_bishop;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_e_bishop_movement(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
						}
					}
					break;
				}
			case eROOK:
				{
					if (board.w_e_rook == 0) continue; // no chess
					srcboard = board.w_e_rook;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = w_e_rook_movement(src);
						while (dstboard)
						{
							int dst = BitScan(&dstboard); // LSB
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
						}
					}
					break;
				}
			} // switch 
		} // for 
	} // if 
	else
	{
		for (int i = 0; i < CHESS_SIZE; i++)
		{
			U32 srcboard = 0, dstboard = 0;
			switch (move_ordering[i])
			{
			case KING:
				{
					srcboard = board.b_king;
					int src = BitScan(&srcboard);
					dstboard = b_king_movement(src);
					while (dstboard)
					{
						int dst = BitScanReverse(dstboard); // MSB
						movelist[*start]     |= src;
						movelist[(*start)++] |= dst << 6;
						dstboard ^= 1 << dst;
					}
					break;
				}
			case GOLD:
				{
					if (board.b_gold == 0) continue; // no chess
					srcboard = board.b_gold;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_gold_movement(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case SILVER:
				{
					if (board.b_silver == 0) continue; // no chess
					srcboard = board.b_silver;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_silver_movement(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
							PromoteGenerator(start, src, dst, movelist, turn, false);
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case PAWN:
				{
					if (board.b_pawn == 0) continue; // no chess
					srcboard = board.b_pawn;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_pawn_movement(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							if (dst > 19 && dst < BOARD_SIZE)
							{
								movelist[*start]     |= src;
								movelist[*start]	 |= dst << 6;
								movelist[(*start)++] |= PRO_MASK;
							}
							else
							{
								movelist[*start]     |= src;
								movelist[(*start)++] |= dst << 6;
							}
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case BISHOP:
				{
					if (board.b_bishop == 0) continue; // no chess
					srcboard = board.b_bishop;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_bishop_movement(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							PromoteGenerator(start, src, dst, movelist, turn, false);
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case ROOK:
				{
					if (board.b_rook == 0) continue; // no chess
					srcboard = board.b_rook;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_rook_movement(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							PromoteGenerator(start, src, dst, movelist, turn, false);
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case eSILVER:
				{
					if (board.b_e_silver == 0) continue; // no chess
					srcboard = board.b_e_silver;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_gold_movement(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case ePAWN:
				{
					if (board.b_e_pawn == 0) continue; // no chess
					srcboard = board.b_e_pawn;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_gold_movement(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case eBISHOP:
				{
					if (board.b_e_bishop == 0) continue; // no chess
					srcboard = board.b_e_bishop;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_e_bishop_movement(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			case eROOK:
				{
					if (board.b_e_rook == 0) continue; // no chess
					srcboard = board.b_e_rook;
					while (srcboard)
					{
						int src = BitScan(&srcboard);
						dstboard = b_e_rook_movement(src);
						while (dstboard)
						{
							int dst = BitScanReverse(dstboard); // MSB
							movelist[*start]     |= src;
							movelist[(*start)++] |= dst << 6;
							dstboard ^= 1 << dst;
						}
					}
					break;
				}
			} // switch
		} // for 
	} // else 

	return ;
}

U32 RookMove (fighter board, int pos, int turn)
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
	ret &= (turn == WHITE) ? ret ^ board.w_occupied \
		: ret ^ board.b_occupied;

	return ret;
}

U32 BishopMove (fighter board, int pos, int turn)
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
	ret = (turn == WHITE) ? ret & (ret ^ board.w_occupied) \
		: ret & (ret ^ board.b_occupied);
	return ret;
}