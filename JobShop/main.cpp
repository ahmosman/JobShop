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

    vector<int> times = { 10 };
    vector<string> filenames = { "instance_ta25.txt" };

    for (int t = 0; t < times.size(); t++) {

        cout << "Time (s): " << times[t] << endl;
        for (int inst = 0; inst < filenames.size(); inst++) {
            string filename = filenames[inst];
            cout << "Instance " << filename << endl;


            for (int j = 1; j <= 20; j++) {
                
                cout << "Job NO " << j << endl;
                cout << "Makespans: ";
                for (int i = 1; i <= 5; i++) {
                    Operations op(filename);
                    op.readInstanceBeasley();
                    Operations resized = op.getResizedOperations(j);
                    Grasp grasp(resized);
                    Schedule optimized = grasp.getOptimizedSchedule(times[t]);

                    cout << optimized.makespan << " ";
                }
                cout << "\n\n";
            }

            cout << "\n\n";
        }
        cout << "\n\n";
    }
 
    return 0;
}