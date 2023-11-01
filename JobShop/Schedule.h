#pragma once

#include <vector>
#include "Operation.h"
#include "OperationQueue.h"

using namespace std;

class Schedule
{
	public:
		int makespan = 0;
		vector<vector<Operation>> _schedule;
		OperationQueue queue;

		void createRandomSchedule();
		Schedule(int no_machines);
		void addOperationToSchedule(Operation operation);
		void jobRunningInAnotherMachine(int time_unit, Operation operation);
		bool isMachineBusy(int time_unit, int machine_num);
};

