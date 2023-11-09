#include "Schedule.h"

void Schedule::createRandomSchedule()
{
	while (!queue.isEmpty()) { //TODO: Nie dodaje siê ostatnia operacja i ostatni proces Ÿle sie wykonuje
		Operation operation_to_add = queue.popRandomPendingOperation();
		addOperationToSchedule(operation_to_add);
	}
}

void Schedule::createScheduleByJobsOrder(vector<int> job_order)
{
	for (int job : job_order) {
		Operation operation_to_add = queue.popNextOperationForJob(job);
		addOperationToSchedule(operation_to_add);
	}
}

Schedule::Schedule(Operations operations) : queue(operations)
{
	num_machines = operations.num_machines;
	num_jobs = operations.getJobsNum();
	schedule = vector<vector<Operation>>(operations.num_machines, vector<Operation>());
	current_job_time = vector<int>(operations.getJobsNum(), 0);
	performed_operations = vector<vector<bool>>(num_machines, vector<bool>(num_jobs, false));
}

void Schedule::addOperationToSchedule(Operation operation, int time_unit)
{
	vector<Operation> schedule_machine = schedule[operation.machine];
	bool operation_added = false;

	int current_job_time = getCurrentTimeForJob(operation.job_no);

	if (!time_unit || time_unit < current_job_time) {
		time_unit = current_job_time;
	}

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
	setPerformedOperation(operation);
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

	cout << "\033[38;5;" << 7 << "m"; //set color
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

void Schedule::setPerformedOperation(Operation operation)
{
	performed_operations[operation.machine][operation.job_no] = true;
}

bool Schedule::isOperationPerformed(Operation operation)
{
	return performed_operations[operation.machine][operation.job_no];
}

int Schedule::getRandomIndex(int from, int to)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(from, to - 1);

	return dis(gen);
}

void Schedule::addOperationsByParents(vector<Schedule> parents, int machine, int time_unit)
{
	int parent_index = getRandomIndex(0, 2);

	Operation operation = parents[parent_index].schedule[machine][time_unit];

	if (!operation._is_null) {

		if (isOperationPerformed(operation)) {

			int opposite_parent_index = parent_index == 0 ? 1 : 0;

			operation = parents[opposite_parent_index].schedule[machine][time_unit];
		}

		if (!operation._is_null && !isOperationPerformed(operation)) {

			Operation next_in_queue = queue.popNextOperationForJob(operation.job_no);

			while (!checkSameOperations(operation, next_in_queue)) {
				addOperationToSchedule(next_in_queue);
				next_in_queue = queue.popNextOperationForJob(operation.job_no);
			}

			addOperationToSchedule(next_in_queue);
		}
	}
}

bool Schedule::checkSameOperations(Operation op1, Operation op2)
{
	return op1.duration == op2.duration && op1.machine == op2.machine && op1.job_no == op2.job_no;
}

void Schedule::addRandomPendingOperation(int machine, int time_unit)
{
	Operation operation = queue.popRandomPendingOperationByMachine(machine);

	if (!operation._is_null) {
		addOperationToSchedule(operation);
	}
}