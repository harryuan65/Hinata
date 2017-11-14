#include "head.h"
#include "move.h"

U16 DoMove (U16 move, fighter *board, int *chessboard, int turn)
{
	// ***********************************
	// 14 bytes  X  |  X  | XXXXXX | XXXXXX (<-)
	//          pro   eat    dst      src
	//			 1	   1      6		   6
	// ***********************************

	U16 ret;
	int src, dst, eat, pro;
	int src_pce, dst_pce;

	src =  move & SRC_MASK;
	dst = (move & DST_MASK) >> 6;
	eat = (move & EAT_MASK) >> 12;
	pro =  move >> 13;

	src_pce = chessboard[src];
	dst_pce = chessboard[dst];

	/*if (src_pce == BLANK)
	{
		printf("error nodes = %d\n", nodes);
		assert(src_pce != BLANK);
	}*/

	if (eat == 1)
	{
		*(&board->w_pawn + pce_bias[src_pce]) ^= (1 << src) ^ (1 << dst);
		if (pro == 1)
		{
			*(&board->w_pawn + pce_bias[src_pce]) ^= 1 << dst;
			src_pce += PROMOTE;
			*(&board->w_pawn + pce_bias[src_pce]) ^= 1 << dst;
		}
		*(&board->w_pawn + pce_bias[dst_pce]) ^=  1 << dst;

		if (turn == WHITE)
		{
			board->w_occupied ^= (1 << src) ^ (1 << dst);
			board->b_occupied ^=  1 << dst;
		}
		else
		{
			board->b_occupied ^= (1 << src) ^ (1 << dst);
			board->w_occupied ^=  1 << dst;
		}

		ret = SaveCapture(dst_pce, board, chessboard) << 5 | dst_pce;
	}
	else
	{
		if (src < BOARD_SIZE)
		{
			*(&board->w_pawn + pce_bias[src_pce]) ^= (1 << src) ^ (1 << dst);
			if (pro == 1)
			{
				*(&board->w_pawn + pce_bias[src_pce]) ^= 1 << dst;
				src_pce += PROMOTE;
				*(&board->w_pawn + pce_bias[src_pce]) ^= 1 << dst;
			}

			(turn == WHITE) ?\
				board->w_occupied ^= (1 << src) ^ (1 << dst):\
				board->b_occupied ^= (1 << src) ^ (1 << dst);
			ret = TYPE_NORMAL_MOVE;
		}
		else
		{
			DoHandMove(src, board);
			*(&board->w_pawn + pce_bias[src_pce]) ^= 1 << dst;
			(turn == WHITE) ?\
				board->w_occupied ^= 1 << dst:\
				board->b_occupied ^= 1 << dst;
			ret = TYPE_HAND_MOVE;
		}
	}

	chessboard[src] = BLANK;
	chessboard[dst] = src_pce;

	return ret;
}

void DoHandMove (int src, fighter *board)
{
	switch (src)
	{
	case F5: board->white_hand ^= HAND_ROOK1;   break;
	case F4: board->white_hand ^= HAND_ROOK2;   break;
	case F3: board->white_hand ^= HAND_BISHOP1; break;
	case F2: board->white_hand ^= HAND_BISHOP2; break;
	case F1: board->white_hand ^= HAND_GOLD1; 	break;
	case G5: board->white_hand ^= HAND_GOLD2;	break;
	case G4: board->white_hand ^= HAND_SILVER1; break;
	case G3: board->white_hand ^= HAND_SILVER2;	break;
	case G2: board->white_hand ^= HAND_PAWN1;	break;
	case G1: board->white_hand ^= HAND_PAWN2;	break;
	case H5: board->black_hand ^= HAND_ROOK1;   break;
	case H4: board->black_hand ^= HAND_ROOK2;	break;
	case H3: board->black_hand ^= HAND_BISHOP1;	break;
	case H2: board->black_hand ^= HAND_BISHOP2;	break;
	case H1: board->black_hand ^= HAND_GOLD1;	break;
	case I5: board->black_hand ^= HAND_GOLD2;	break;
	case I4: board->black_hand ^= HAND_SILVER1;	break;
	case I3: board->black_hand ^= HAND_SILVER2;	break;
	case I2: board->black_hand ^= HAND_PAWN1;	break;
	case I1: board->black_hand ^= HAND_PAWN2;   break;
	}

	return ;
}

U8 SaveCapture (int pce, fighter *board, int *chessboard)
{
	U8 ret;

	switch (pce)
	{
	case (ROOK | COLOR_BOUND): case (eROOK | COLOR_BOUND): // F5 F4
		if ((board->white_hand & HAND_ROOK1) == 0) 
		{
			board->white_hand ^= HAND_ROOK1;
			chessboard[F5] = ROOK;
			ret = F5;
		}
		else
		{
			board->white_hand ^= HAND_ROOK2;
			chessboard[F4] = ROOK;
			ret = F4;
		}
		break;
	case (BISHOP | COLOR_BOUND): case (eBISHOP | COLOR_BOUND): // F3 F2
		if ((board->white_hand & HAND_BISHOP1) == 0)
		{
			board->white_hand ^= HAND_BISHOP1;
			chessboard[F3] = BISHOP;
			ret = F3;
		}
		else
		{
			board->white_hand ^= HAND_BISHOP2;
			chessboard[F2] = BISHOP;
			ret = F2;
		}
		break;
	case (GOLD | COLOR_BOUND): // F1 G5
		if ((board->white_hand & HAND_GOLD1) == 0)
		{
			board->white_hand ^= HAND_GOLD1;
			chessboard[F1] = GOLD;
			ret = F1;
		}
		else
		{
			board->white_hand ^= HAND_GOLD2;
			chessboard[G5] = GOLD;
			ret = G5;
		}
		break;
	case (SILVER | COLOR_BOUND): case (eSILVER | COLOR_BOUND): // G4 G3
		if ((board->white_hand & HAND_SILVER1) == 0)
		{
			board->white_hand ^= HAND_SILVER1;
			chessboard[G4] = SILVER;
			ret = G4;
		}
		else
		{
			board->white_hand ^= HAND_SILVER2;
			chessboard[G3] = SILVER;
			ret = G3;
		}
		break;
	case (PAWN | COLOR_BOUND): case (ePAWN | COLOR_BOUND): // G2 G1
		if ((board->white_hand & HAND_PAWN1) == 0)
		{
			board->white_hand ^= HAND_PAWN1;
			chessboard[G2] = PAWN;
			ret = G2;
		}
		else
		{
			board->white_hand ^= HAND_PAWN2;
			chessboard[G1] = PAWN;
			ret = G1;
		}
		break;
	case ROOK: case eROOK: // H5 H4
		if ((board->black_hand & HAND_ROOK1) == 0)
		{
			board->black_hand ^= HAND_ROOK1;
			chessboard[H5] = ROOK | COLOR_BOUND;
			ret = H5;
		}
		else
		{
			board->black_hand ^= HAND_ROOK2;
			chessboard[H4] = ROOK | COLOR_BOUND;
			ret = H4;
		}
		break;
	case BISHOP: case eBISHOP: // H3 H2
		if ((board->black_hand & HAND_BISHOP1) == 0)
		{
			board->black_hand ^= HAND_BISHOP1;
			chessboard[H3] = BISHOP | COLOR_BOUND;
			ret = H3;
		}
		else
		{
			board->black_hand ^= HAND_BISHOP2;
			chessboard[H2] = BISHOP | COLOR_BOUND;
			ret = H2;
		}
		break;
	case GOLD: // H1 I5
		if ((board->black_hand & HAND_GOLD1) == 0)
		{
			board->black_hand ^= HAND_GOLD1;
			chessboard[H1] = GOLD | COLOR_BOUND;
			ret = H1;
		}
		else
		{
			board->black_hand ^= HAND_GOLD2;
			chessboard[I5] = GOLD | COLOR_BOUND;
			ret = I5;
		}
		break;
	case SILVER: case eSILVER: // I4 I3
		if ((board->black_hand & HAND_SILVER1) == 0)
		{
			board->black_hand ^= HAND_SILVER1;
			chessboard[I4] = SILVER | COLOR_BOUND;
			ret = I4;
		}
		else
		{
			board->black_hand ^= HAND_SILVER2;
			chessboard[I3] = SILVER | COLOR_BOUND;
			ret = I3;
		}
		break;
	case PAWN: case ePAWN: // I2 I1
		if ((board->black_hand & HAND_PAWN1) == 0)
		{
			board->black_hand ^= HAND_PAWN1;
			chessboard[I2] = PAWN | COLOR_BOUND;
			ret = I2;
		}
		else
		{
			board->black_hand ^= HAND_PAWN2;
			chessboard[I1] = PAWN | COLOR_BOUND;
			ret = I1;
		}
		break;
	default:	// king
		ret = NULL_PLACE; 
		break;
	}

	return ret;
}

void UnDoMove (U16 capture, U16 move, fighter *board, int *chessboard, int turn)
{
	// ***********************************
	// 14 bytes  X  |  X  | XXXXXX | XXXXXX (<-)
	//			pro	  eat    dst      src
	//		 	 1	   1 	  6		   6
	// ***********************************
	// 11 bytes  XXXXXX   |   XXXXX (<-)
	//           cap_dst       pce
	// ***********************************

	int src, dst, pro;
	int src_pce, dst_pce;

	src =  move & SRC_MASK;
	dst = (move & DST_MASK) >> 6;
	pro =  move >> 13;

	src_pce = chessboard[src];
	dst_pce = chessboard[dst];

	if (dst_pce == BLANK)
	{
		printf("nodes = %d\n", nodes);
		assert(dst_pce != BLANK);
	}

	if (capture == TYPE_NORMAL_MOVE)
	{
		*(&board->w_pawn + pce_bias[dst_pce]) ^= (1 << src) ^ (1 << dst);
		if (pro == 1)
		{
			*(&board->w_pawn + pce_bias[dst_pce]) ^= 1 << src;
			dst_pce -= PROMOTE;
			*(&board->w_pawn + pce_bias[dst_pce]) ^= 1 << src;
		}

		(turn == WHITE) ? \
			board->w_occupied ^= (1 << src) ^ (1 << dst): \
			board->b_occupied ^= (1 << src) ^ (1 << dst);

		chessboard[src] = dst_pce;
		chessboard[dst] = BLANK;
	}
	else if (capture == TYPE_HAND_MOVE)
	{
		UnDoHandMove(src, board);

		*(&board->w_pawn + pce_bias[dst_pce]) ^=  1 << dst;
		(turn == WHITE) ? \
			board->w_occupied ^= 1 << dst: \
			board->b_occupied ^= 1 << dst;

		chessboard[src] = dst_pce;
		chessboard[dst] = BLANK;
	}
	else
	{
		int cap_pce =  capture & CAPTURE_PCE;
		int cap_dst = (capture & CAPTURE_DST) >> 5;

		*(&board->w_pawn + pce_bias[dst_pce]) ^= (1 << src) ^ (1 << dst);
		if (pro == 1)
		{
			*(&board->w_pawn + pce_bias[dst_pce]) ^= 1 << src;
			dst_pce -= PROMOTE;
			*(&board->w_pawn + pce_bias[dst_pce]) ^= 1 << src;
		}
		*(&board->w_pawn + pce_bias[cap_pce]) ^=  1 << dst;

		if (turn == WHITE)
		{
			board->w_occupied ^= (1 << src) ^ (1 << dst);
			board->b_occupied ^=  1 << dst;
		}
		else
		{
			board->b_occupied ^= (1 << src) ^ (1 << dst);
			board->w_occupied ^=  1 << dst;
		}

		RemoveCapture(cap_dst, board);
		chessboard[src] = dst_pce;
		chessboard[dst] = cap_pce;

		if (cap_dst != NULL_PLACE)
			chessboard[cap_dst] = BLANK;
	}
	
	return ;
}

void UnDoHandMove (int src, fighter *board)
{
	switch (src)
	{
	case F5: board->white_hand ^= HAND_ROOK1;   break;
	case F4: board->white_hand ^= HAND_ROOK2;   break;
	case F3: board->white_hand ^= HAND_BISHOP1; break;
	case F2: board->white_hand ^= HAND_BISHOP2; break;
	case F1: board->white_hand ^= HAND_GOLD1; 	break;
	case G5: board->white_hand ^= HAND_GOLD2;	break;
	case G4: board->white_hand ^= HAND_SILVER1; break;
	case G3: board->white_hand ^= HAND_SILVER2;	break;
	case G2: board->white_hand ^= HAND_PAWN1;	break;
	case G1: board->white_hand ^= HAND_PAWN2;	break;
	case H5: board->black_hand ^= HAND_ROOK1;   break;
	case H4: board->black_hand ^= HAND_ROOK2;	break;
	case H3: board->black_hand ^= HAND_BISHOP1;	break;
	case H2: board->black_hand ^= HAND_BISHOP2;	break;
	case H1: board->black_hand ^= HAND_GOLD1;	break;
	case I5: board->black_hand ^= HAND_GOLD2;	break;
	case I4: board->black_hand ^= HAND_SILVER1;	break;
	case I3: board->black_hand ^= HAND_SILVER2;	break;
	case I2: board->black_hand ^= HAND_PAWN1;	break;
	case I1: board->black_hand ^= HAND_PAWN2;   break;
	}

	return ;
}

void RemoveCapture (int cap_place, fighter *board)
{
	switch (cap_place)
	{
	case F5: board->white_hand ^= HAND_ROOK1;   break;
	case F4: board->white_hand ^= HAND_ROOK2;   break;
	case F3: board->white_hand ^= HAND_BISHOP1; break;
	case F2: board->white_hand ^= HAND_BISHOP2; break;
	case F1: board->white_hand ^= HAND_GOLD1; 	break;
	case G5: board->white_hand ^= HAND_GOLD2;	break;
	case G4: board->white_hand ^= HAND_SILVER1; break;
	case G3: board->white_hand ^= HAND_SILVER2;	break;
	case G2: board->white_hand ^= HAND_PAWN1;	break;
	case G1: board->white_hand ^= HAND_PAWN2;	break;
	case H5: board->black_hand ^= HAND_ROOK1;   break;
	case H4: board->black_hand ^= HAND_ROOK2;	break;
	case H3: board->black_hand ^= HAND_BISHOP1;	break;
	case H2: board->black_hand ^= HAND_BISHOP2;	break;
	case H1: board->black_hand ^= HAND_GOLD1;	break;
	case I5: board->black_hand ^= HAND_GOLD2;	break;
	case I4: board->black_hand ^= HAND_SILVER1;	break;
	case I3: board->black_hand ^= HAND_SILVER2;	break;
	case I2: board->black_hand ^= HAND_PAWN1;	break;
	case I1: board->black_hand ^= HAND_PAWN2;   break;
	}

	return ;
}
