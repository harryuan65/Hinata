#ifndef _MOVE_
#define _MOVE_

#define PCE_SIZE            30
#define COLOR_BOUND       0x10  // 16
#define NULL_PLACE          45  // to save the king when it dead.
#define TYPE_NORMAL_MOVE 0x5de  
#define TYPE_HAND_MOVE   0x5df  

/* mask */
#define CAPTURE_PCE  0x1f
#define CAPTURE_DST  0x7e0

// start from w_pawn
// struct have a natural order
// 利用記憶體位置有連續的性質，來在特定棋種 bitboard 的移動
const int pce_bias[PCE_SIZE] = {
	0,  0,  2,  4,  6,  8, 10, 0, 
	0, 12, 14,  0, 16, 18,  0, 0, 
	0,  1,  3,  5,  7,  9, 11, 0,
	0, 13, 15,  0, 17, 19
};

#endif