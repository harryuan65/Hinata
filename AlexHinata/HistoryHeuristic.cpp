#include "head.h"

HistoryHeuristic::HistoryHeuristic(bool isEnable)
{
	m_isEnable = isEnable;
	if (!m_isEnable) {
		return;
	}
	FILE *file = fopen(HISTORY_FILE_NAME, "r");
	if (file != NULL) {
		printf("Loading %s\n", HISTORY_FILE_NAME);
		for (int i = 0; i < HISTORY_TABLE_SIZE; i++) {
			fscanf(file, "%d", &table[i]);
		}
		fclose(file);
	}
}

bool HistoryHeuristic::SaveTable() {
	if (!m_isEnable) {
		return false;
	}
	FILE *file = fopen(HISTORY_FILE_NAME, "w");
	printf("Saving %s\n", HISTORY_FILE_NAME);
	for (int i = 0; i < HISTORY_TABLE_SIZE; i++) {
		fprintf(file, "%d ", table[i]);
	}
	fclose(file);
	return true;
}

void HistoryHeuristic::OverflowAvoid() {
	if (!m_isEnable) {
		return;
	}
	for (int i = 0; i < HISTORY_TABLE_SIZE; i++) {
		table[i] >>= 2;
	}
}

void HistoryHeuristic::SortByTable(U16* moveList, int size) {
	if (!m_isEnable) {
		return;
	}
	pair<U16, U32>* sortList = new pair<U16, U32>[size];
	for (int i = 0; i < size; i++) {
		sortList[i] = make_pair(moveList[i], table[Action2Index(moveList[i])]);
	}
	sort(sortList, sortList + size,
		[](const pair<U16, U32>& i, const pair<U16, U32>& j) -> bool {return i.second > j.second; });
	for (int i = 0; i < size; i++) {
		moveList[i] = sortList[i].first;
	}
	delete(sortList);
}

void HistoryHeuristic::UpdateTable(U16 moveAction, int delta) {
	int index = Action2Index(moveAction);
	if (mHistoryHeur.table[index] >> 31) {
		mHistoryHeur.OverflowAvoid();
	}
	mHistoryHeur.table[index] += delta;
}