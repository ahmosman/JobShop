#include "Operations.h"
using namespace std;

void Operations::readInstanceTaillard()
{
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Nie uda³o siê otworzyæ pliku." << endl;
        exit(0);
    }

    file >> num_jobs >> num_machines;

    _operations = vector<vector<Operation>>(num_jobs, vector<Operation>());

    operations_duration = vector<vector<int>>(num_jobs, vector<int>(num_machines));
    operations_machines = vector<vector<int>>(num_jobs, vector<int>(num_machines));

    for (int i = 0; i < num_jobs; ++i) {
        for (int j = 0; j < num_machines; ++j) {
            file >> operations_duration[i][j];
            Operation operation;
            operation.duration = operations_duration[i][j];
            operation.job_no = i;
            operation.is_empty = false;
            _operations[i].push_back(operation);
        }
    }

    for (int i = 0; i < num_jobs; ++i) {
        for (int j = 0; j < num_machines; ++j) {
            file >> operations_machines[i][j];
            _operations[i][j].machine = operations_machines[i][j] - 1;
        }
    }

    _reversed_operations = getReversedOperations(_operations);

    file.close();
}

void Operations::readInstanceBeasley()
{
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Nie uda³o siê otworzyæ pliku." << endl;
        exit(0);
    }

    file >> num_jobs >> num_machines;

    _operations = vector<vector<Operation>>(num_jobs, vector<Operation>());

    operations_duration = vector<vector<int>>(num_jobs, vector<int>(num_machines));
    operations_machines = vector<vector<int>>(num_jobs, vector<int>(num_machines));

    for (int i = 0; i < num_jobs; ++i) {
        for (int j = 0; j < num_machines; ++j) {
            file >> operations_machines[i][j] >> operations_duration[i][j]; //TODO: Remove?
            Operation operation;
            operation.duration = operations_duration[i][j];
            operation.machine = operations_machines[i][j];
            operation.job_no = i;
            operation.is_empty = false;
            _operations[i].push_back(operation);
        }
    }

    _reversed_operations = getReversedOperations(_operations);

    file.close();
}

vector<vector<Operation>> Operations::getReversedOperations(vector<vector<Operation>> operations)
{

    vector<vector<Operation>> reversed_operations(num_jobs, vector<Operation>(num_machines));

    for (int j = 0; j < num_machines; ++j) {
        for (int i = 0; i < num_jobs; ++i) {
            reversed_operations[i][j] = operations[i][num_machines - 1 - j];
        }
    }

    return reversed_operations;

}

Operations::Operations(string filename)
{
    this->filename = filename;
}

Operations::Operations(const Operations& operations)
{
    _operations = operations._operations;
    _reversed_operations = operations._reversed_operations;
    num_jobs = operations.num_jobs;
    num_machines = operations.num_machines;
}

Operations Operations::getResizedOperations(int limit)
{
    vector<vector<Operation>> resized_operations(_operations.begin(), _operations.begin() + limit);

    Operations operations(*this);
    operations.num_jobs = limit;
    operations._operations = resized_operations;
    operations._reversed_operations = operations.getReversedOperations(_operations);


    return operations;
}

int Operations::getJobsNum()
{
    return _operations.size();
}
