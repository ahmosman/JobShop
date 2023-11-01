#include "Schedule.h"

void Schedule::createRandomSchedule()
{
	while (!queue.isEmpty()) {
		Operation operation_to_add = queue.popRandomPendingOperation();
	}
}

Schedule::Schedule(int num_machines)
{
	_schedule = vector<vector<Operation>>(num_machines, vector<Operation>());
	queue = new OperationQueue(operations);
}

void Schedule::addOperationToSchedule(Operation operation)
{
	vector<Operation> schedule_machine = _schedule[operation.machine];
	for (int time_unit = 0; time_unit < schedule_machine.size(); time_unit++) {
		if (!isMachineBusy(time_unit, operation.machine)) {

		}
	}
}

void Schedule::jobRunningInAnotherMachine(int time_unit, Operation operation)
{

}

bool Schedule::isMachineBusy(int time_unit, int machine_num)
{

}