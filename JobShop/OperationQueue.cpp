#include "OperationQueue.h"


void OperationQueue::createOperationQueue(Operations operations)
{
    // first jobs to do are in the back columns, because of using pop_back()
    _queue = operations._reversed_operations;
}

OperationQueue::OperationQueue(Operations operations)
{
    createOperationQueue(operations);
}

int OperationQueue::getRandomIndex(int from, int to)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(from, to - 1);

    return dis(gen);
}

Operation OperationQueue::popNextOperationForJob(int job_no)
{
    Operation picked_operation = _queue[job_no].back();
    _queue[job_no].pop_back();
    return picked_operation;
}

Operation OperationQueue::getNextOperationForJob(int job_no)
{
    Operation picked_operation = _queue[job_no].back();
    return picked_operation;
}

Operation OperationQueue::popRandomPendingOperation()
{
    int random_index = getRandomIndex(0, _queue.size());

    while (_queue[random_index].empty()) {
        random_index = getRandomIndex(0, _queue.size());
    }

    Operation picked_operation = _queue[random_index].back();
    _queue[random_index].pop_back();

   //cout << picked_operation.job_no << '\t';

    return picked_operation;
}

Operation OperationQueue::popRandomPendingOperationByMachine(int machine)
{
    vector<Operation> pending_machine_operations;

    for (vector<Operation> &job_operations : _queue) {

        if (!job_operations.empty()) {

            Operation next_operation = job_operations.back();

            if (next_operation.machine == machine) {
                pending_machine_operations.push_back(next_operation);
            }
        }
    }

    random_device rd;
    default_random_engine rng(rd());
    shuffle(pending_machine_operations.begin(), pending_machine_operations.end(), rng);

    Operation random_operation = pending_machine_operations.front();

    if (!random_operation.is_empty) {
       _queue[random_operation.job_no].pop_back();
    }

    return random_operation;
}

bool OperationQueue::isEmpty()
{
    for (const auto& row : _queue) {
        if (!row.empty()) {
            return false;
        }
    }
    return true;
}
