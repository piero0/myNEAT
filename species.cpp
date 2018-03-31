#include "species.hpp"

using namespace pneat;

Species::Species(std::shared_ptr<Config> cfg) {
    totalFitness = 0.0;
    this->cfg = cfg;
}

void Species::updateFitness() {
    //fake update
    auto gen = Random::get<float>(-0.1, 0.1);

    float fit, rnd;

    for(auto it = orgs.begin(); it != orgs.end(); it++) {
        fit = it->getFitness();
        rnd = gen.next();
        if(fit + rnd < 0.0) fit = 0.0;
        it->setFitness(fit + rnd);
    }
}
 
void Species::calcOffstringProbability() {
    //clamp fitness to  0.0-1.0 range
    totalFitness = std::accumulate(orgs.begin(), orgs.end(), 0.0, Organism::adder);

    float lastThresh = 0.0;
    for(auto it = orgs.begin(); it != orgs.end(); it++) {
        lastThresh += (*it).setThresh(lastThresh, totalFitness);
    }
}
  
Organism& Species::randomPick(float probability) {
    // p < 0 -> return 1st
    // p > 1 -> return last
    auto it = std::lower_bound(orgs.begin(), orgs.end(), probability, Organism::compareThresh);
    if(it == orgs.end()) {
        Log::get()->warn("randomPick returned END: {0}", probability);
        it--;
    }
    return *it;
}

void Species::doCrossover() {
    Log::get()->debug("Species::doCrossover");
    /*
        0. prepare a probability list for crossover
        1. pick pairs
        2. do the crossover
        3. mutate outcomes
    */
    std::vector<Organism> newOrgs;

    this->calcOffstringProbability();

    auto gen = Random::get<float>(0.0, 1.0);

    float f1,f2;

    for(std::size_t a=0; a<orgs.size(); a++) {
        Log::get()->debug("New Child");
        auto& p1org = this->randomPick(gen.next());
        auto& p2org = this->randomPick(gen.next());

        auto& parent1 = p1org.getGenome();
        auto& parent2 = p2org.getGenome();

        Log::get()->debug("Parent1: {0}", parent1.printGenes());
        Log::get()->debug("Parent2: {0}", parent2.printGenes());

        Genome child = parent1.crossover(parent2);

        Log::get()->debug("Child:   {0}", child.printGenes());

        if(gen.next() < cfg->AddNodeChance) child.mutateAddNode();
        if(gen.next() < cfg->AddLinkChance) child.mutateAddLink(cfg->AddLinkMaxTries);
        if(gen.next() < cfg->MutateWeightsChance) child.mutateWeights();

        Log::get()->debug("ChildMT: {0}", child.printGenes());

        auto org = Organism(child);

        f1 = p1org.getFitness();
        f2 = p2org.getFitness();
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
