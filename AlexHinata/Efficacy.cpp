/*#include "head.h"

Efficacy::Efficacy() {
	Init();
}

void Efficacy::Init() {
	total_nodes = 0;
	failed_nodes = 0;
	leave_nodes = 0;
	depth2_nodes = 0;
	durationTime = 0;
}

void Efficacy::PrintDetail() {
	printf(
		"Total Nodes       : %11llu\n"
		"Failed-High Nodes : %11llu\n"
		"Leave Nodes       : %11llu\n"
		"Time              : %14.2lf\n"
		"Node/s            : %14.2lf\n"
		"Evaluate          : %11d\n"
		"PV leaf Evaluate  : %11d\n",
		nodes, failed_nodes, leave_nodes, durationTime,
		(durationTime ? nodes / durationTime : 0), Evaluate(chessboard), pvEvaluate);
}*/