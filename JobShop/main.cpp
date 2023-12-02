#include <iostream>
#include <fstream>
#include <string>
#include "Schedule.h"
#include "Grasp.h"

using namespace std;

//Sposob kompilacji linux: W folderze z plikami zrodlowymi i naglowkowymi wykonac: g++ -o main.o ./*.cpp

// Przyklad wywolania: ./main.o instance_some.txt t 10
// 1 parametr: nazwa pliku z instancja
// 2 parametr: typ instancji t -Taillard b - Beasley
// 3 parametr: dlugosc wykonywania w sekundach


int main(int argc, char* argv[]) {

    string filename;
    int execution_time = 10;

    if (!argv[1]) {
       cout << "Podaj nazwe pliku i typ instancji";
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

    Grasp grasp(op);
    Schedule optimized1 = grasp.getOptimizedSchedule(execution_time);

    //optimized1.printResult();

    optimized1.printJobTimes();

    cout << '\n';
 
    return 0;
}