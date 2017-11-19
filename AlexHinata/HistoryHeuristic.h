#ifndef _HISTORY_HEURISTIC_
#define _HISTORY_HEURISTIC_

#define HISTORY_TABLE_SIZE 625
#define HISTORY_FILE_NAME "history_table.txt"
#define Action2Index(action) (action & SRC_MASK) * 25 + ((action & DST_MASK) >> 6)
#include "head.h"

class HistoryHeuristic
{
private:
	bool m_isEnable;
public:
	HistoryHeuristic(bool isEnable);
	bool SaveTable();
	void OverflowAvoid();
	void SortByTable(U16* moveList, int size);
	void UpdateTable(U16 moveAction, int delta);

	U32 table[HISTORY_TABLE_SIZE] = { 0 };
};

#endif // !HISTORY_HEURISTIC
