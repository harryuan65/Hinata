#ifndef _HISTORY_HEURISTIC_
#define _HISTORY_HEURISTIC_

#define HISTORY_TABLE_SIZE 625
#define HISTORY_FILE_NAME "history_table.txt"
#define Action2Index(action) (action & SRC_MASK) * 25 + ((action & DST_MASK) >> 6)
#include "head.h"

class HistoryHeuristic
{
public:
	HistoryHeuristic();
	bool SaveTable();
	void OverflowAvoid();
	void SortByTable(U16* moveList, int size);
	void UpdateTable(U16 moveAction, int delta);

	bool m_isEnable;
	bool m_isUpdate;

private:
	U32 table[HISTORY_TABLE_SIZE] = { 0 };
};

#endif // !HISTORY_HEURISTIC
