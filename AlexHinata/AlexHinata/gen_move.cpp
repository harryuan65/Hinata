#include "head.h"

// white use LSB
// black use MSB
// 利用 (key目前沒加)(小 -> 大)吃(大 -> 小) | (大 -> 小)移動(小 -> 大)
// 14 bits  pro  eat  dst  src
//		 	 1    1    6    6

typedef void (*genmove_t)(int &, fighter &, U16 *, int);

int Generator (fighter &board, U16 *movelist, int turn) {
	genmove_t genmove_func[3] = {AttackGenerator, MoveGenerator, HandGenerator}; 

	//  attack -> move -> hand
    int ret = 0;
	for (int i = 0; i < 3; i++)
		genmove_func[i](ret, board, movelist, turn);

	return ret;
}

void AttackGenerator (int &start, fighter &board, U16 *movelist, int turn)
{
    unsigned long src, dst;
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
					_BitScanForward(&src, srcboard);
					dstboard = w_king_attack(src);
					while (dstboard)
					{
						_BitScanForward(&dst, dstboard); // LSB
                        dstboard ^= 1 << dst;
						movelist[start]     |= src;
						movelist[start]     |= dst << 6;
						movelist[(start)++] |= EAT_MASK;
					}
					break;
				}
			case GOLD:
				{
					if (board.w_gold == 0) continue; // no chess
					srcboard = board.w_gold;
					while (srcboard)
					{
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_gold_attack(src);
						while (dstboard)
						{
							_BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_silver_attack(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_pawn_attack(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							if (dst < 5)
							{
								movelist[start]     |= src;
								movelist[start]     |= dst << 6;
								movelist[start]     |= EAT_MASK;
								movelist[(start)++] |= PRO_MASK;
							}
							else
							{
								movelist[start]     |= src;
								movelist[start]     |= dst << 6;
								movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_bishop_attack(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							PromoteGenerator(start, src, dst, movelist, turn, true);
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_rook_attack(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							PromoteGenerator(start, src, dst, movelist, turn, true);
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_gold_attack(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_gold_attack(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_e_bishop_attack(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_e_rook_attack(src);
						while (dstboard)
						{
							_BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                    _BitScanForward(&src, srcboard);
					dstboard = b_king_attack(src);
					while (dstboard)
					{
                        _BitScanReverse(&dst, dstboard); // MSB
                        dstboard ^= 1 << dst;
						movelist[start]     |= src;
						movelist[start]     |= dst << 6;
						movelist[(start)++] |= EAT_MASK;
					}
					break;
				}
			case GOLD:
				{
					if (board.b_gold == 0) continue; // no chess
					srcboard = board.b_gold;
					while (srcboard)
					{
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_gold_attack(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_silver_attack(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
							PromoteGenerator(start, src, dst, movelist, turn, true);
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_pawn_attack(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							if (dst > 19 && dst < BOARD_SIZE)
							{
								movelist[start]     |= src;
								movelist[start]     |= dst << 6;
								movelist[start]     |= EAT_MASK;
								movelist[(start)++] |= PRO_MASK;
							}
							else
							{
								movelist[start]     |= src;
								movelist[start]     |= dst << 6;
								movelist[(start)++] |= EAT_MASK;
							}
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_bishop_attack(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							PromoteGenerator(start, src, dst, movelist, turn, true);
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_rook_attack(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							PromoteGenerator(start, src, dst, movelist, turn, true);
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_gold_attack(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_gold_attack(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_e_bishop_attack(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_e_rook_attack(src);
						while (dstboard)
						{
							_BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[start]     |= dst << 6;
							movelist[(start)++] |= EAT_MASK;
						}
					}
					break;
				}
			} // switch
		} // for
	} // else

	return ;
}

void MoveGenerator (int &start, fighter &board, U16 *movelist, int turn)
{
    unsigned long src, dst;
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
					_BitScanForward(&src, srcboard);
					dstboard = w_king_movement(src);
					while (dstboard)
					{
                        _BitScanForward(&dst, dstboard); // LSB
                        dstboard ^= 1 << dst;
						movelist[start]     |= src;
						movelist[(start)++] |= dst << 6;
					}
					break;
				}
			case GOLD:
				{
					if (board.w_gold == 0) continue; // no chess
					srcboard = board.w_gold;
					while (srcboard)
					{
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_gold_movement(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_silver_movement(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_pawn_movement(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							if (dst < 5)
							{
								movelist[start]     |= src;
								movelist[start]     |= dst << 6;
								movelist[(start)++] |= PRO_MASK;
							}
							else
							{
								movelist[start]     |= src;
								movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_bishop_movement(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							PromoteGenerator(start, src, dst, movelist, turn, false);
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_rook_movement(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							PromoteGenerator(start, src, dst, movelist, turn, false);
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_gold_movement(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_gold_movement(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_e_bishop_movement(src);
						while (dstboard)
						{
                            _BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = w_e_rook_movement(src);
						while (dstboard)
						{
							_BitScanForward(&dst, dstboard); // LSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                    _BitScanForward(&src, srcboard);
					dstboard = b_king_movement(src);
					while (dstboard)
					{
                        _BitScanReverse(&dst, dstboard); // MSB
                        dstboard ^= 1 << dst;
						movelist[start]     |= src;
						movelist[(start)++] |= dst << 6;
					}
					break;
				}
			case GOLD:
				{
					if (board.b_gold == 0) continue; // no chess
					srcboard = board.b_gold;
					while (srcboard)
					{
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_gold_movement(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_silver_movement(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
							PromoteGenerator(start, src, dst, movelist, turn, false);
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_pawn_movement(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							if (dst > 19 && dst < BOARD_SIZE)
							{
								movelist[start]     |= src;
								movelist[start]	 |= dst << 6;
								movelist[(start)++] |= PRO_MASK;
							}
							else
							{
								movelist[start]     |= src;
								movelist[(start)++] |= dst << 6;
							}
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_bishop_movement(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							PromoteGenerator(start, src, dst, movelist, turn, false);
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_rook_movement(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							PromoteGenerator(start, src, dst, movelist, turn, false);
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_gold_movement(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_gold_movement(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_e_bishop_movement(src);
						while (dstboard)
						{
                            _BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
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
                        _BitScanForward(&src, srcboard);
                        srcboard ^= 1 << src;
						dstboard = b_e_rook_movement(src);
						while (dstboard)
						{
							_BitScanReverse(&dst, dstboard); // MSB
                            dstboard ^= 1 << dst;
							movelist[start]     |= src;
							movelist[(start)++] |= dst << 6;
						}
					}
					break;
				}
			} // switch
		} // for 
	} // else 

	return ;
}

/*
***************************
*    enemy's territory    *    // 0 ~ 4   (A)
***************************
*                         *
*         neutral         *    // 5 ~ 19  (B)
*                         *
***************************
*    my own territory     *    // 20 ~ 24 (C)
***************************
*/
// there are three types to upgrade.
// for example turn is WHITE,
//		(1).  (C)/(B) ->   (A)
//		(2).	(A)   ->	   (A)
//		(3).	(A)   -> (B)/(C)
void PromoteGenerator(int &start, int src, int dst, U16 *movelist, int turn, bool atk) {
    if (turn == WHITE ?
        // 一般移動 || 吃子移動
        (src < BOARD_SIZE && dst < 5) || (src < 5 && dst < BOARD_SIZE) :
        (src < BOARD_SIZE && dst > 19) || (src > 19 && src < BOARD_SIZE && dst < BOARD_SIZE))
        movelist[start++] = PRO_MASK | (atk ? EAT_MASK : 0) | dst << 6 | src;
}

U32 RookMove (fighter &board, int pos, int turn) {
    U32 ret = RookMove_h(board, pos, turn);
	return (turn == WHITE) ?
        ret & (ret ^ board.w_occupied) :
        ret & (ret ^ board.b_occupied);
}

U32 BishopMove (fighter &board, int pos, int turn) {
    U32 ret = BishopMove_h(board, pos, turn);
    /*int p, tmp, dl = 0, ur = 0, dr = 0, ul = 0;
    for (p = pos, tmp = pos + 4; tmp < 25 && p % 5; tmp = (p = tmp) + 4) {
        dl |= 1 << tmp;
        if (occupied & dl) break;
    }

    for (p = pos, tmp = pos - 4; tmp >= 0 && p % 5 != 4; tmp = (p = tmp) - 4) {
        ur |= 1 << tmp;
        if (occupied & ur) break;
    }

    for (p = pos, tmp = pos + 6; tmp < 25 && p % 5 != 4; tmp = (p = tmp) + 6) {
        dr |= 1 << tmp;
        if (occupied & dr) break;
    }

    for (p = pos, tmp = pos - 6; tmp >= 0 && p % 5; tmp = (p = tmp) - 6) {
        ul |= 1 << tmp;
        if (occupied & ul) break;
    }

    ret = dl | ur | dr | ul;*/
    return (turn == WHITE) ?
        ret & (ret ^ board.w_occupied) :
        ret & (ret ^ board.b_occupied);
}