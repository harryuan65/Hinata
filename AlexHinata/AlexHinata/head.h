#ifndef _HEAD_
#define _HEAD_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <direct.h>
#include <intrin.h>

#include "define.h"
#include "library.h"
#include "function.h"
#include "evaluate.h"

#pragma intrinsic(_BitScanForward)  
#pragma intrinsic(_BitScanReverse)

extern U64 nodes;
extern bool isFailHigh;
extern U64 failed_nodes;
extern U64 leave_nodes;

#endif