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
}

void Schedule::addOperationToSchedule(Operation operation)
{
	bool operation_added = insertOperationInEmptySpace(operation);

	if (!operation_added) {
		insertOperationInEnd(operation);
	}

}


Operation Schedule::getRecentJobOperation(int job_no)
{
	return recent_jobs_operations[job_no];
}

void Schedule::setRecentJobOperation(Operation operation)
{
	recent_jobs_operations[operation.job_no] = operation;
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

		// Adding an empty operation before the inserted operation
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

		// Adding an empty operation before the inserted operation
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

void Schedule::printJobTimes()
{
	cout << makespan << '\n';

	vector<vector<int>> schedule_job_times = vector<vector<int>>(num_jobs, vector<int>(0));

	for (const auto& machine_times : schedule) {

		for (const auto& operation : machine_times) {


			if (!operation.is_empty) {

				auto it = lower_bound(schedule_job_times[operation.job_no].begin(), schedule_job_times[operation.job_no].end(), operation.start_time);
				schedule_job_times[operation.job_no].insert(it, operation.start_time);
			}
		}
	}

	for (const auto& job_times : schedule_job_times) {
		for (const auto& time : job_times) {
			cout << time << ' ';
		}
		cout << '\n';
	}
}

void Schedule::printResult()
{
	cout << "\n";
	printSchedule();
	cout << "Makespan:" << makespan << "\n\n";
}