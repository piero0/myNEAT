#include "species.hpp"

using namespace pneat;

Species::Species() {
    totalFitness = 0.0;
}

void Species::updateFitness(Organism& organism) {

}

void Species::prepareFitness() {
    //clamp fitness to  0.0-1.0 range
    totalFitness = std::accumulate(orgs.begin(), orgs.end(), 0.0, Organism::adder);

    float lastThresh = 0.0;
    for(auto it = orgs.begin(); it != orgs.end(); it++) {
        lastThresh += (*it).setThresh(lastThresh, totalFitness);
    }
}

Organism& Species::randomPick(float probability) {
    auto it = std::lower_bound(orgs.begin(), orgs.end(), probability, Organism::compareThresh);
    if(it == orgs.end()) std::cerr << "randomPick returned END" << std::endl;
    return *it;
}

void Species::doCrossover() {
    /*
        0. prepare a probability list for crossover
        1. pick pairs
        2. do the crossover
        3. mutate outcomes
    */
    
}

void Species::getOutcasts() {

}   

void Species::calcCompatibilityDistans() {
    compabilityDistance = 0.0;
}

void Species::dump() {
    for(auto& el: orgs) el.getGenome().print();
}
