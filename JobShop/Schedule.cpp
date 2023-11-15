#include "Schedule.h"

void Schedule::createRandomSchedule()
{
	while (!queue.isEmpty()) {
		Operation operation_to_add = queue.popRandomPendingOperation();
		addOperationToSchedule(operation_to_add);
		//cout << "\x1B[2J\x1B[H";
		//printSchedule();
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
	schedule = vector<vector<Operation>>(num_machines, vector<Operation>());
	recent_jobs_operations = vector<Operation>(num_jobs, Operation{});
	performed_operations = vector<vector<bool>>(num_machines, vector<bool>(num_jobs, false));
}

void Schedule::addOperationToSchedule(Operation operation)
{
	bool operation_added = insertOperationInEmptySpace(operation);

	if (!operation_added) {
		insertOperationInEnd(operation);
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

	//setCurrentTimeForJob(operation.job_no, end_time_operation);
	setPerformedOperation(operation);
}


void Schedule::addNewTimeUnitsToSchedule(int num_units)
{

	for (int i = 0; i < num_units; i++) {
		for (int j = 0; j < num_machines; j++) {
			Operation empty_operation;
			empty_operation.is_empty = true;
			schedule[j].push_back(empty_operation);
		}
	}

}

bool Schedule::isMachineAvailable(int time_unit, Operation operation)
{
	for (int tu = time_unit; tu < time_unit + operation.duration && tu < makespan; tu++) {
		if (!schedule[operation.machine][tu].is_empty) {
			return false;
		}
	}
	return true;
}

bool Schedule::isJobRunning(int time_unit, Operation operation)
{
	for (int i = 0; i < num_machines; i++) {
		for (int tu = time_unit; tu < time_unit + operation.duration && tu < makespan; tu++) {
			if (schedule[i][tu].job_no == operation.job_no && !schedule[i][tu].is_empty) {
				return true;
			}
		}

	}
	return false;
}

void Schedule::printSchedule()
{
	for (const auto& row : schedule) {
		
		int fill_line = makespan;

		for (const auto& operation : row) {

			int count = operation.duration;

			while (count) {
				if (!operation.is_empty) {
					cout << "\033[38;5;" << operation.job_no + 1 << "m"; //set color
					cout << operation.job_no << " ";
				}
				else {
					cout << "\033[38;5;" << 7 << "m"; //set color
					cout << "- ";
				}
				fill_line--;
				count--;
			}
		}

		while (fill_line > 0) {
			cout << "\033[38;5;" << 7 << "m"; //set color
			cout << "- ";
			fill_line--;
		}

		cout << "\n";
	}

	cout << "\033[38;5;" << 7 << "m"; //set color

}

void Schedule::printResult()
{
	cout << "\n";
	printSchedule();
	cout << "Makespan:" << makespan << "\n\n";

}

Operation Schedule::getRecentJobOperation(int job_no)
{
	return recent_jobs_operations[job_no];
}

void Schedule::setRecentJobOperation(Operation operation)
{
	recent_jobs_operations[operation.job_no] = operation;
}

void Schedule::setPerformedOperation(Operation operation)
{
	performed_operations[operation.machine][operation.job_no] = true;
}

vector<vector<Operation>> Schedule::getScheduleQueue()
{
	vector<vector<Operation>> schedule_queue = vector<vector<Operation>>(num_machines, vector<Operation>());

	for (int machine = 0; machine < schedule.size(); machine++) {

		vector<Operation> machine_operations = schedule[machine];

		for (int operation = 0; operation < machine_operations.size(); operation++) {
			if (!machine_operations[operation].is_empty) {
				schedule_queue[machine].push_back(machine_operations[operation]);
			}
		}
	}

	return schedule_queue;
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

	if (!operation.is_empty) {

		if (isOperationPerformed(operation)) {

			int opposite_parent_index = parent_index == 0 ? 1 : 0;

			operation = parents[opposite_parent_index].schedule[machine][time_unit];
		}

		if (!operation.is_empty && !isOperationPerformed(operation)) {

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

	if (!operation.is_empty) {
		addOperationToSchedule(operation);
	}
}

bool Schedule::insertOperationInEmptySpace(Operation operation)
{
	Operation recent_operation = getRecentJobOperation(operation.job_no);

	vector<Operation> schedule_operations = schedule[operation.machine];

	bool found_empty_operation = false;

	int empty_operation_index = 0;

	for (empty_operation_index; empty_operation_index < schedule_operations.size(); empty_operation_index++) {

		Operation op = schedule_operations[empty_operation_index];

		if (op.is_empty && op.duration >= operation.duration && op.end_time - operation.duration >= recent_operation.end_time) {
			found_empty_operation = true;
			break;
		}
	}

	if (found_empty_operation) {
		overrideEmptyOperation(empty_operation_index, operation);
	}

	return found_empty_operation;
}

void Schedule::overrideEmptyOperation(int index, Operation operation)
{

	Operation override_operation = schedule[operation.machine][index];

	if (override_operation.duration > operation.duration) {

		Operation recent_operation = getRecentJobOperation(operation.job_no);

		// Dodawanie pustej operacji przed wprowadzan¹ operacj¹
		if (recent_operation.end_time >= override_operation.start_time) {

			Operation new_empty_operation;
			new_empty_operation.is_empty = true;
			new_empty_operation.machine = override_operation.machine;
			new_empty_operation.duration = recent_operation.end_time - override_operation.start_time + 1;
			new_empty_operation.start_time = override_operation.start_time;
			new_empty_operation.end_time = recent_operation.end_time;

			schedule[operation.machine].insert(schedule[operation.machine].begin() + index, new_empty_operation);
			index++;

			override_operation.start_time = new_empty_operation.end_time + 1;
			override_operation.duration = override_operation.end_time - override_operation.start_time + 1;
		}

		// Dodawanie pustej operacji przed wprowadzan¹ operacj¹
		if (override_operation.duration > operation.duration) {
			Operation new_empty_operation;
			new_empty_operation.is_empty = true;
			new_empty_operation.machine = override_operation.machine;
			new_empty_operation.duration = override_operation.duration - operation.duration;
			new_empty_operation.start_time = override_operation.start_time + operation.duration;
			new_empty_operation.end_time = override_operation.end_time;

			schedule[operation.machine].insert(schedule[operation.machine].begin() + index + 1, new_empty_operation);
		}

	}

	operation.is_empty = false;
	operation.start_time = override_operation.start_time;
	operation.end_time = operation.start_time + operation.duration - 1;

	schedule[operation.machine][index] = operation;

	setRecentJobOperation(operation);
}

void Schedule::insertOperationInEnd(Operation operation)
{
	Operation recent_job_operation = getRecentJobOperation(operation.job_no);

	if (schedule[operation.machine].empty()) {

		if (recent_job_operation.duration > 0)
		{
			Operation new_empty_operation;
			new_empty_operation.is_empty = true;
			new_empty_operation.machine = operation.machine;
			new_empty_operation.start_time = 0;
			new_empty_operation.end_time = recent_job_operation.end_time;
			new_empty_operation.duration = new_empty_operation.end_time - new_empty_operation.start_time + 1;
			schedule[operation.machine].push_back(new_empty_operation);


			operation.start_time = recent_job_operation.end_time + 1;
		}
		else {

			operation.start_time = 0;
		}

	}
	else {
		Operation last_machine_operaton = schedule[operation.machine].back();

		if (recent_job_operation.end_time > last_machine_operaton.end_time) {

			Operation new_empty_operation;
			new_empty_operation.is_empty = true;
			new_empty_operation.start_time = last_machine_operaton.end_time + 1;
			new_empty_operation.end_time = recent_job_operation.end_time;
			new_empty_operation.duration = new_empty_operation.end_time - new_empty_operation.start_time + 1;

			schedule[operation.machine].push_back(new_empty_operation);
			last_machine_operaton = new_empty_operation;
		}

		operation.start_time = last_machine_operaton.end_time + 1;
	}

	operation.end_time = operation.start_time + operation.duration - 1;
	operation.is_empty = false;

	schedule[operation.machine].push_back(operation);

	setRecentJobOperation(operation);

	int potential_makespan = operation.end_time + 1;

	if (potential_makespan > makespan) {
		makespan = potential_makespan;
	}

}