#include "Grasp.h"

Grasp::Grasp(Operations operations): operations(operations)
{
}

Schedule Grasp::getOptimizedSchedule(int time_seconds)
{
    int min_makespan = INT_MAX;

    Schedule min_schedule(operations);

    auto duration = std::chrono::seconds(time_seconds);

    auto start_time = std::chrono::steady_clock::now();


    while (std::chrono::steady_clock::now() - start_time < duration) {

        Schedule curr_schedule(operations);
        curr_schedule.createGraspSchedule();

        int curr_makespan = curr_schedule.makespan;

        if (curr_makespan < min_makespan) {
            min_makespan = curr_makespan;

            min_schedule = curr_schedule;

            //cout << "\x1B[2J\x1B[H";
            //cout << min_makespan << '\n';
        }

    }

    return min_schedule;
}
