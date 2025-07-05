#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <string>
#include "Schedule.h"
#include "Generation.h"

using namespace std;

// Linux compilation method: In the folder with source and header files, run: g++ -o main.o ./*.cpp

// Example usage: ./main.o instance_some.txt t 10
// 1st parameter: instance file name
// 2nd parameter: instance type t - Taillard, b - Beasley
// 3rd parameter: execution time in seconds


int main(int argc, char* argv[]) {

    int population = 50;
    int crossovers = 10;
    float mutation_rate = 0.4;

    string filename;
    int execution_time = 10;

    if (!argv[1]) {
        cout << "Provide the file name and instance type";
        exit(0);
    }

    filename = argv[1];
    Operations op(filename);


    if (argv[2][0] == 't') {
        op.readInstanceTaillard();
    }
    else {
        op.readInstanceBeasley();
    }

    if (argv[3]) {
        execution_time = stoi(argv[3]);
    }

    Generation gen1(op, population, crossovers, mutation_rate);

    Schedule optimized1 = gen1.getOptimizedSchedule(execution_time);
    optimized1.printJobTimes();

    cout << '\n';

    optimized1.printResult();

    this_thread::sleep_for(chrono::seconds(2));
 
    return 0;
}