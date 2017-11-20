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


//U32 Movement[20][BOARD_SIZE] = {
//    /* 0 */{ 0 },
//    /* 1 PAWN */
//    {
//        0x0000000, 	0x0000000, 	0x0000000, 	0x0000000, 	0x0000000,
//        0x0000001, 	0x0000002, 	0x0000004, 	0x0000008, 	0x0000010,
//        0x0000020, 	0x0000040, 	0x0000080, 	0x0000100,	0x0000200,
//        0x0000400, 	0x0000800, 	0x0001000, 	0x0002000,	0x0004000,
//        0x0008000, 	0x0010000, 	0x0020000, 	0x0040000,	0x0080000
//    },
//    /* 2 SILVER*/
//    {
//        0x0000040, 	0x00000a0, 	0x0000140, 	0x0000280, 	0x0000100,
//        0x0000803, 	0x0001407, 	0x000280e, 	0x000501c, 	0x0002018,
//        0x0010060, 	0x00280e0, 	0x00501c0, 	0x00a0380,	0x0040300,
//        0x0200c00, 	0x0501c00, 	0x0a03800, 	0x1407000,	0x0806000,
//        0x0018000, 	0x0038000, 	0x0070000, 	0x00e0000,	0x00c0000
//    }
//    ,
//    /* 3 GOLD*/
//    {
//        0x0000022, 	0x0000045, 	0x000008a, 	0x0000114, 	0x0000208,
//        0x0000443, 	0x00008a7, 	0x000114e, 	0x000229c, 	0x0004118,
//        0x0008860, 	0x00114e0, 	0x00229c0, 	0x0045380,	0x0082300,
//        0x0110c00, 	0x0229c00, 	0x0453800, 	0x08a7000,	0x1046000,
//        0x0218000, 	0x0538000, 	0x0a70000, 	0x14e0000,	0x08c0000
//    },
//    /*4*/{ 0 },
//    /*5*/{ 0 },
//    /* 6=22 KING (Both)*/
//    {
//        0x0000062, 	0x00000e5, 	0x00001ca, 	0x0000394, 	0x0000308,
//        0x0000c43, 	0x0001ca7, 	0x000394e, 	0x000729c, 	0x0006118,
//        0x0018860, 	0x00394e0, 	0x00729c0, 	0x00e5380, 	0x00c2300,
//        0x0310c00, 	0x0729c00, 	0x0e53800, 	0x1ca7000, 	0x1846000,
//        0x0218000, 	0x0538000, 	0x0a70000, 	0x14e0000, 	0x08c0000
//    },
//    /*7*/{ 0 },
//    /*8*/{ 0 },
//    /*9*/{ 0 },
//    /*10*/{ 0 },
//    /*11*/{ 0 },
//    /*12*/{ 0 },
//    /*13*/{ 0 },
//    /*14*/{ 0 },
//    /*15*/{ 0 },
//    /*16*/{ 0 },
//    /*17 黑PAWN*/
//    {
//        0x0000020, 	0x0000040, 	0x0000080, 	0x0000100, 	0x0000200,
//        0x0000400, 	0x0000800, 	0x0001000, 	0x0002000, 	0x0004000,
//        0x0008000, 	0x0010000, 	0x0020000, 	0x0040000,	0x0080000,
//        0x0100000, 	0x0200000, 	0x0400000, 	0x0800000,	0x1000000,
//        0x0000000, 	0x0000000, 	0x0000000, 	0x0000000,	0x0000000
//    },
//    /*18 黑SILVER*/
//    {
//        0x0000060, 	0x00000e0, 	0x00001c0, 	0x0000380, 	0x0000300,
//        0x0000c02, 	0x0001c05, 	0x000380a, 	0x0007014, 	0x0006008,
//        0x0018040, 	0x00380a0, 	0x0070140, 	0x00e0280,	0x00c0100,
//        0x0300800, 	0x0701400, 	0x0e02800, 	0x1c05000,	0x1802000,
//        0x0010000, 	0x0028000, 	0x0050000, 	0x00a0000,	0x0040000
//    },
//    /*19 黑GOLD*/
//    {
//        0x0000062, 	0x00000e5, 	0x00001ca, 	0x0000394, 	0x0000308,
//        0x0000c41, 	0x0001ca2, 	0x0003944, 	0x0007288, 	0x0006110,
//        0x0018820, 	0x0039440, 	0x0072880, 	0x00e5100,	0x00c2200,
//        0x0310400, 	0x0728800, 	0x0e51000, 	0x1ca2000,	0x1844000,
//        0x0208000, 	0x0510000, 	0x0a20000, 	0x1440000,	0x0880000
//    },
//};

#endif