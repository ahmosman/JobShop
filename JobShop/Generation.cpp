#include "Generation.h"

Schedule Generation::generateChild(Schedule parent1, Schedule parent2)
{
    vector<Schedule> parents = { parent1, parent2 };
    Schedule child_schedule(operations);
    int time_unit = 0;

    int min_makespan = min(parent1.makespan, parent2.makespan);

    while (!child_schedule.queue.isEmpty() && time_unit < min_makespan) {

        for (int machine = 0; machine < operations.num_machines; machine++) {
            
            if (shouldMutate()) {
                child_schedule.addRandomPendingOperation(machine, time_unit);
            }
            else {
                child_schedule.addOperationsByParents(parents, machine, time_unit);
            }
        }

        time_unit++;
    }

    child_schedule.createRandomSchedule();

    return child_schedule;
}

Generation::Generation(Operations operations, int population, int crossovers, float mutation_rate): operations(operations)
{
    this->population = population;
    this->crossovers = crossovers;
    this->mutation_rate = mutation_rate;
}

bool Generation::shouldMutate()
{

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0, 1.0);

    float random_value = dis(gen);

    return random_value < mutation_rate;
}

void Generation::optimize()
{
    int min_makespan = 99999;

    vector<Schedule> generation;
    for (int i = 0; i < population; i++) {
        Schedule schedule(operations);
        schedule.createRandomSchedule();
        generation.push_back(schedule);
    }

    while (true) {

        vector<Schedule> children = getChildrenFromTournament(generation);

        //for (int c = 0; c < crossovers; c++) {

        //    int parent_index_1 = getRandomIndex(0, generation.size());
        //    int parent_index_2 = getRandomIndex(0, generation.size());

        //    while (parent_index_1 == parent_index_2) {
        //        parent_index_2 = getRandomIndex(0, generation.size());
        //    }

        //    children.push_back(generateChild(generation[parent_index_1], generation[parent_index_2]));

        //}
        
        while (!children.empty()) {
            generation.push_back(children.back());
            children.pop_back();
        }
                
        sort(generation.begin(), generation.end(),
            [](const Schedule& a, const Schedule& b) {
                return a.makespan < b.makespan;
           });
        
        vector<Schedule> resized_generation(generation.begin(), generation.begin() + population);
        generation = resized_generation;
        



        int curr_makespan = generation[0].makespan;
        if (curr_makespan < min_makespan) {
            min_makespan = curr_makespan;
        }
        cout << "\x1B[2J\x1B[H";
        cout << "Current makespans: ";
        for (int i = 0; i < generation.size(); i++) {
            cout << generation[i].makespan << "\t";
        }
        cout << '\n';
        cout << "Min makespan: " << min_makespan;
    }

    

}

int Generation::getRandomIndex(int from, int to)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(from, to - 1);

    return dis(gen);
}

vector<Schedule> Generation::getChildrenFromTournament(vector<Schedule> generation)
{

    vector<Schedule> children;
    vector<Schedule> parents;

    // Losowe wymieszanie generacji
    random_device rd;
    mt19937 g(rd());
    shuffle(generation.begin(), generation.end(), g);

    int tournament_count = population / crossovers;

    for (int i = 0; i < population; i += tournament_count) {
        // Tworzenie turnieju
        vector<Schedule> tournament(generation.begin() + i, generation.begin() + i + tournament_count);

        // Wybranie zwyciêzcy turnieju na podstawie makespan
        sort(tournament.begin(), tournament.end(),
            [](const Schedule& a, const Schedule& b) {
                return a.makespan < b.makespan;
            });
        Schedule winner = tournament.front();

        // Dodanie zwyciêzcy do rodziców
        parents.push_back(winner);

        if (parents.size() == 2) {
            children.push_back(generateChild(parents[0], parents[1]));
            parents.clear();
        }
    }

    return children;
}
