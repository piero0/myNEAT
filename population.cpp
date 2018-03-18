#include "population.hpp"

using namespace pneat;

Population::Population(): 
    masterGenome(MasterGenome::getInstance()) 
    {
}

void Population::initPopulation(ushort size, Genome& baseGenome) {
    masterGenome.initFromGenome(baseGenome);
    Species sp; 
    //Genome g = dynamic_cast<Genome&>(masterGenome);
    for(int a=0; a<size; a++) {
        Organism org(baseGenome);
        org.getGenome().mutateWeights();
        sp.addOrganism(org);
    }
    species.push_back(sp);
}

void Population::testLoop() {
    auto& sp = species[0];

    std::size_t iterNum = 10;

    for(std::size_t a=0; a<iterNum; a++) {
        std::cout << "Epoch: " << a << std::endl;
        sp.updateFitness();
        sp.prepareFitness();
        sp.doCrossover();
    }
}

void Population::dump() {
    for(auto& el: species) {
        el.dump();
    }
}