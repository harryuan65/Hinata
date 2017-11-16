#include "head.h"

void PromoteGenerator (int *start, int src, int dst, U16 *movelist, int turn, bool atk)
{
	/***************************/
	/*    enemy's territory    */	// 0 ~ 4   (A)
	/***************************/
	/*                         */
	/*         neutral         */	// 5 ~ 19  (B)
	/*                         */
	/***************************/
	/*    my own territory     */	// 20 ~ 24 (C)
	/***************************/

	// there are three types to upgrade.
	// for example turn is WHITE,
	//		(1).  (C)/(B) ->   (A)
	//		(2).	(A)   ->	   (A)
	//		(3).	(A)   -> (B)/(C)

	// move =>   X   | X | XXXXXX | XXXXXX (1 | 1 | 6 | 6) (14 bytes) (output)
	//        promote eat   dst      src

	if (turn == WHITE)
	{
		if ((src < BOARD_SIZE && dst < 5) || (src < 5 && dst < BOARD_SIZE)) // 一般移動 & 吃子移動
		{
			movelist[*start] |= src;
			movelist[*start] |= dst << 6;
			if (atk) movelist[*start] |= EAT_MASK;
			movelist[(*start)++] |= PRO_MASK;
		}
	}
	else
	{
		if ((src < BOARD_SIZE && dst > 19) || (src > 19 && src < BOARD_SIZE && dst < BOARD_SIZE)) // 一般移動 & 吃子移動
		{
			movelist[*start] |= src;
			movelist[*start] |= dst << 6;
			if (atk) movelist[*start] |= EAT_MASK;
			movelist[(*start)++] |= PRO_MASK;
		}
	}

	return ;
}