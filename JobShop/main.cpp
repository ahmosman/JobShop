#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <string>
#include "Schedule.h"
#include "Generation.h"

using namespace std;

//Sposob kompilacji linux: W folderze z plikami zrodlowymi i naglowkowymi wykonac: g++ -o main.o ./*.cpp

// Przyklad wywolania: ./main.o instance_some.txt t 10
// 1 parametr: nazwa pliku z instancja
// 2 parametr: typ instancji t -Taillard b - Beasley
// 3 parametr: dlugosc wykonywania w sekundach

int main(int argc, char* argv[]) {

    int population = 600;
    int crossovers = 20;
    float mutation_rate = 0.1;

    string filename;
    int execution_time = 10;

    vector<int> times = { 60, 120, 180 };
    vector<string> filenames = { "instance_ta20.txt", "instance_ta21.txt", "instance_ta22.txt", "instance_ta23.txt", "instance_ta24.txt", "instance_ta25.txt" };

    for (int t = 0; t < times.size(); t++) {
        cout << "Time: " << times[t] << endl;
        for (int inst = 0; inst < filenames.size(); inst++) {

            string filename = filenames[inst];

            Operations op(filename);

            op.readInstanceBeasley();

            Generation gen1(op, population, crossovers, mutation_rate);

            Schedule optimized = gen1.getOptimizedSchedule(times[t]);
           
            cout << "Instance: " << filename << "\tMakespan: " << optimized.makespan << endl;

            //this_thread::sleep_for(chrono::seconds(2));

        }

        cout << endl;
        cout << endl;
    }


 
    return 0;
}