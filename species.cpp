#include "species.hpp"

using namespace pneat;

Species::Species() {
    totalFitness = 0.0;
}

void Species::updateFitness() {
    //fake update
    auto& utl = Util::getInstance();
    utl.setFloatDist(-0.1, 0.1);

    for(auto it = orgs.begin(); it != orgs.end(); it++) {
        float fit = it->getFitness();
        it->setFitness(fit + utl.nextFloat());
    }
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
    if(it == orgs.end()) {
        std::cerr << "randomPick returned END: " << probability << std::endl;
    }
    return *it;
}

void Species::doCrossover() {
    /*
        0. prepare a probability list for crossover
        1. pick pairs
        2. do the crossover
        3. mutate outcomes
    */
    const float ADD_NODE_THRESH = 0.03;
    const float ADD_LINK_THRESH = 0.05;
    const float MUTATE_WEIGHTS_THRESH = 0.10;

    std::vector<Organism> newOrgs;

    auto& utl = Util::getInstance();

    this->prepareFitness();

    auto& dist = utl.setFloatDist(0.0, 1.0);
    //std::cout << dist.a() << " : " << dist.b() << std::endl;
    //std::cout << dist.min() << " : " << dist.max() << std::endl;

    for(std::size_t a=0; a<orgs.size(); a++) {
        auto& p1org = this->randomPick(utl.nextFloat());
        auto& p2org = this->randomPick(utl.nextFloat());

        float f1 = p1org.getFitness();
        float f2 = p2org.getFitness();

        auto& parent1 = p1org.getGenome();
        auto& parent2 = p2org.getGenome();

        Genome child = parent1.crossover(parent2);

        if(utl.nextFloat() < ADD_NODE_THRESH) child.mutateAddNode();
        if(utl.nextFloat() < ADD_LINK_THRESH) child.mutateAddLink();
        if(utl.nextFloat() < MUTATE_WEIGHTS_THRESH) child.mutateWeights();

        auto org = Organism(child);
        org.setFitness((f1 > f2) ? f1 : f2);
        newOrgs.push_back(org);
    }

    orgs = newOrgs;
}

void Species::getOutcasts() {

}   

void Species::calcCompatibilityDistans() {
    compabilityDistance = 0.0;
}

void Species::dump() {
    for(auto& el: orgs) el.dump();
    //el.getGenome().print();
}
