#include "Generation.h"

Schedule Generation::generateChild(Schedule parent1, Schedule parent2)
{
    vector<Schedule> parents = { parent1, parent2 };
    Schedule child_schedule(operations);
    
    vector<vector<Operation>> schedule_queue_1 = parent1.getScheduleQueue();
    vector<vector<Operation>> schedule_queue_2 = parent2.getScheduleQueue();


    while (!child_schedule.queue.isEmpty()) {

        for (int machine = 0; machine < operations.num_machines; machine++) {
            
            if (child_schedule.queue.anyPendingOperationByMachine(machine)) {

                Operation next_operation;

                if (shouldMutate()) {

                    next_operation = child_schedule.queue.popRandomPendingOperationByMachine(machine);

                }
                else {

                    int pick_parent_1 = getRandomIndex(0, 2);

                    if (pick_parent_1) {
                        next_operation = schedule_queue_1[machine].front();
                    }
                    else {
                        next_operation = schedule_queue_2[machine].front();
                    }
                }

                if (checkSameOperations(next_operation, child_schedule.queue.getNextOperationForJob(next_operation.job_no))) {

                    child_schedule.addOperationToSchedule(next_operation);

                    removeOperation(child_schedule.queue._queue[next_operation.job_no], next_operation);
                    removeOperation(schedule_queue_1[machine], next_operation);
                    removeOperation(schedule_queue_2[machine], next_operation);
                }


            }  
        }
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

Schedule Generation::getOptimizedSchedule(int time_seconds)
{
    int min_makespan = 99999999999;

    auto duration = std::chrono::seconds(time_seconds);

    auto start_time = std::chrono::steady_clock::now();

    vector<Schedule> generation;
    for (int i = 0; i < population; i++) {
        Schedule schedule(operations);
        schedule.createRandomSchedule();
        generation.push_back(schedule);
    }

    while (std::chrono::steady_clock::now() - start_time < duration) {

        vector<Schedule> children = getChildrenFromTournament(generation);
        
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

        //cout << "\x1B[2J\x1B[H";
        //printCurrentMakespans(generation);
    }

    //cout << "\x1B[2J\x1B[H";

    return generation[0];

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

    int tournament_count = population / crossovers / 2;

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

void Generation::removeOperation(vector<Operation> &remove_from, Operation operation)
{
    const auto iterator = find_if(remove_from.begin(), remove_from.end(),
        [this, &operation](const Operation& op) { return checkSameOperations(op, operation); });

    if (iterator != remove_from.end()) {
        remove_from.erase(iterator);
    }
}

bool Generation::checkSameOperations(Operation op1, Operation op2)
{
    return op1.duration == op2.duration && op1.machine == op2.machine && op1.job_no == op2.job_no;
}

void Generation::printCurrentMakespans(vector<Schedule> generation)
{
    cout << "Current makespans: ";
    for (int i = 0; i < generation.size(); i++) {
        cout << generation[i].makespan << "\t";
    }
    cout << '\n';
    cout << "Min makespan: " << generation[0].makespan;
}

