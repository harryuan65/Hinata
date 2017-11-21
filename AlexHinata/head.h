#ifndef _HEAD_
#define _HEAD_

using namespace std;

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <assert.h>
#include <time.h>
#include <direct.h>
#include <ctype.h>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <windows.h>
#include "define.h"
#include "change.h"
#include "library.h"
#include "function.h"
#include "evaluate.h"
#include "HistoryHeuristic.h"
#include "Transposition.h"
//#include "Efficacy.h"

extern bool isFullFailHigh;
extern U64 nodes;
extern U64 failed_nodes;
extern U64 leave_nodes;
extern HistoryHeuristic mHistoryHeur;

#endif