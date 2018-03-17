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

void Population::dump() {
    for(auto& el: species) {
        el.dump();
    }
}