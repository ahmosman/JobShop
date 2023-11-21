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


    for (int i = 1; i <= 20; i++) {

        string filename = "instance_ta25.txt";

        Operations op(filename);

        op.readInstanceBeasley();

        Operations resized = op.getResizedOperations(i);

        Generation gen1(resized, population, crossovers, mutation_rate);

        Schedule optimized = gen1.getOptimizedSchedule(60);

        cout << "No jobs: " << i << "\tMakespan: " << optimized.makespan << endl;

        cout << endl;
    }


 
    return 0;
}