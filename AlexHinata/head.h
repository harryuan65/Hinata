#ifndef _HEAD_
#define _HEAD_

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <direct.h>
#include <ctype.h>
#include <cstring>
#include <vector>
#include "define.h"
#include "change.h"
#include "library.h"
#include "function.h"
#include "evaluate.h"

extern bool isFullFailHigh;
extern U64 nodes;
extern U64 failed_nodes;
extern U64 leave_nodes;
extern U64 depth2_nodes;
extern int pv_chessboard[CHESS_BOARD_SIZE];

#endif