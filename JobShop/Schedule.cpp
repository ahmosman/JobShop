#include "Schedule.h"

void Schedule::createRandomSchedule()
{
	while (!queue.isEmpty()) { //TODO: Nie dodaje siê ostatnia operacja i ostatni proces Ÿle sie wykonuje
		Operation operation_to_add = queue.popRandomPendingOperation();
		addOperationToSchedule(operation_to_add);
	}
}

Schedule::Schedule(Operations operations) : queue(operations)
{
	num_machines = operations.num_machines;
	_schedule = vector<vector<Operation>>(operations.num_machines, vector<Operation>());
}

void Schedule::addOperationToSchedule(Operation operation)
{
	vector<Operation> schedule_machine = _schedule[operation.machine];
	bool operation_added = false;
	int time_unit = 0;

	for (time_unit; time_unit < schedule_machine.size(); time_unit++) {

		if (time_unit == 4 && operation.job_no == 0) {
			int gj = 1;
		}

		if (isMachineAvailable(time_unit, operation.machine) && !isJobRunning(time_unit, operation)) {

			addOperationInTimeUnit(operation, time_unit);
			operation_added = true;
			break;
		}
	}

	if (!operation_added) {
		addOperationInTimeUnit(operation, time_unit);
	}
}

void Schedule::addOperationInTimeUnit(Operation operation, int time_unit)
{

	int end_time_operation = time_unit + operation.duration;

	if (end_time_operation > makespan) {
		addNewTimeUnitsToSchedule(end_time_operation - makespan);
		makespan = end_time_operation;
	}

	if (isJobRunning(time_unit, operation)) {
		int dd = 3;
	}

	for (int tu = time_unit; tu < end_time_operation; tu++) {
		_schedule[operation.machine][tu] = operation;
	}

}


void Schedule::addNewTimeUnitsToSchedule(int num_units)
{

	for (int i = 0; i < num_units; i++) {
		for (int j = 0; j < num_machines; j++) {
			Operation empty_operation;
			empty_operation._is_null = true;
			_schedule[j].push_back(empty_operation);
		}
	}

}

void Schedule::jobRunningInAnotherMachine(int time_unit, Operation operation)
{

}

bool Schedule::isMachineAvailable(int time_unit, int machine_num)
{
	return _schedule[machine_num][time_unit]._is_null;
}

bool Schedule::isJobRunning(int time_unit, Operation operation)
{
	for (int i = 0; i < num_machines; i++) {
		for (int tu = time_unit; tu < time_unit + operation.duration && tu < makespan; tu++) {
			if (_schedule[i][tu].job_no == operation.job_no && !_schedule[i][tu]._is_null) {
				return true;
			}
		}

	}
	return false;
}

void Schedule::printSchedule()
{
	for (const auto& row : _schedule) {
		for (const auto& operation : row) {
			if (!operation._is_null) {
				cout << "\033[38;5;" << operation.job_no + 1 << "m"; //set color
				cout << operation.job_no << " ";
			}
			else {
				cout << "\033[38;5;" << 7 << "m"; //set color
				cout << "- ";
			}
		}
		cout << "\n";
	}

	cout << "Makespan:" << makespan << "\n\n";

}
