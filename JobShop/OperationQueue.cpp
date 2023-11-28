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


Operation OperationQueue::popGreedyRandomPendingOperation()
{
    vector<Operation> all_pending_operations = getAllPendingOperations();

    int pending_operations_no = all_pending_operations.size();

    Operation picked_operation;

    if (pending_operations_no > 1) {

        int subset_no = getRandomIndex(2, pending_operations_no + 1);

        // Sortuj wg. dlugosci wykonywania operacji
        sort(all_pending_operations.begin(), all_pending_operations.end(),
        [](const Operation& a, const Operation& b) {
            return a.duration < b.duration;
        });

        //Otrzymaj zbior najkrotszych operacji o dlugosci subset_no
        vector<Operation> resized_pending_operations(all_pending_operations.begin(), all_pending_operations.begin() + subset_no);

        vector<Operation> pending_operations = resized_pending_operations;

        // ze zbioru wylosuj operacje do dodania
        int operation_index = getRandomIndex(0, subset_no);

        picked_operation = pending_operations[operation_index];
    }
    else {
        picked_operation = all_pending_operations[0];
    }

    _queue[picked_operation.job_no].pop_back();

   //cout << picked_operation.job_no << '\t';

    return picked_operation;
}

vector<Operation> OperationQueue::getAllPendingOperations()
{
    vector<Operation> pending_operations;

    for (vector<Operation>& job_operations : _queue) {

        if (!job_operations.empty()) {

            Operation next_operation = job_operations.back();

            pending_operations.push_back(next_operation);
        }
    }

    return pending_operations;
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
