#include "Schedule.h"

void Schedule::createRandomSchedule()
{
	while (!queue.isEmpty()) { //TODO: Nie dodaje siê ostatnia operacja i ostatni proces Ÿle sie wykonuje
		Operation operation_to_add = queue.popRandomPendingOperation();
		addOperationToSchedule(operation_to_add);
	}
	cout << '\n';
}

void Schedule::createScheduleByJobsOrder(vector<int> job_order)
{
	for (int job : job_order) {
		Operation operation_to_add = queue.getNextOperationForJob(job);
		addOperationToSchedule(operation_to_add);
	}
}

Schedule::Schedule(Operations operations) : queue(operations)
{
	num_machines = operations.num_machines;
	schedule = vector<vector<Operation>>(operations.num_machines, vector<Operation>());
	current_job_time = vector<int>(operations.getJobsNum(), 0);
}

void Schedule::addOperationToSchedule(Operation operation)
{
	vector<Operation> schedule_machine = schedule[operation.machine];
	bool operation_added = false;
	int time_unit = getCurrentTimeForJob(operation.job_no);

	for (time_unit; time_unit < schedule_machine.size(); time_unit++) {

		if (isMachineAvailable(time_unit, operation) && !isJobRunning(time_unit, operation)) {

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

	for (int tu = time_unit; tu < end_time_operation; tu++) {
		schedule[operation.machine][tu] = operation;
	}

	setCurrentTimeForJob(operation.job_no, end_time_operation);

}


void Schedule::addNewTimeUnitsToSchedule(int num_units)
{

	for (int i = 0; i < num_units; i++) {
		for (int j = 0; j < num_machines; j++) {
			Operation empty_operation;
			empty_operation._is_null = true;
			schedule[j].push_back(empty_operation);
		}
	}

}

bool Schedule::isMachineAvailable(int time_unit, Operation operation)
{
	for (int tu = time_unit; tu < time_unit + operation.duration && tu < makespan; tu++) {
		if (!schedule[operation.machine][tu]._is_null) {
			return false;
		}
	}
	return true;
}

bool Schedule::isJobRunning(int time_unit, Operation operation)
{
	for (int i = 0; i < num_machines; i++) {
		for (int tu = time_unit; tu < time_unit + operation.duration && tu < makespan; tu++) {
			if (schedule[i][tu].job_no == operation.job_no && !schedule[i][tu]._is_null) {
				return true;
			}
		}

	}
	return false;
}

void Schedule::printSchedule()
{
	for (const auto& row : schedule) {
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

int Schedule::getCurrentTimeForJob(int job_no)
{
	return current_job_time[job_no];
}

void Schedule::setCurrentTimeForJob(int job_no, int current_time)
{
	current_job_time[job_no] = current_time;
}