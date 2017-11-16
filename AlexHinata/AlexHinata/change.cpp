#include "head.h"
#include "change.h"

U16 String_to_move(char *movestr, int *chessboard)
{
	int src = 0, dst = 0, eat = 0, pro = 0;
	int ret = -1;
	sscanf_s(movestr, "%d %d %d", &src, &dst, &pro);
	if(*(chessboard + dst) != 0)
		eat = 1;
	else 
		eat = 0;
	ret = ( src | (dst << 6) | (eat << 12) | (pro << 13) );

	return ret;
}
