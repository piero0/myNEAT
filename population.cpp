#include "population.hpp"

using namespace pneat;

Population::Population(): 
    masterGenome(MasterGenome::getInstance()) 
    {
}

void Population::initPopulation(Config* cfg, Genome& baseGenome) {
    Log::get()->debug("initPopulation");

    this->cfg = cfg;
    masterGenome.initFromGenome(baseGenome);
    Species sp(cfg); 

    for(int a=0; a<cfg->population; a++) {
        Organism org(baseGenome);
        org.getGenome().mutateWeights();
        sp.addOrganism(org);
    }
    species.push_back(sp);
}

void Population::testLoop() {
    auto& sp = species[0];

    for(std::size_t a=0; a<cfg->epochNum; a++) {
        Log::get()->debug("Epoch: {0}", a);
        sp.updateFitness();
        sp.prepareFitness();
        sp.doCrossover();

        int idx = 0;
        for(auto& org: species[0].getOrganisms()) {
            std::string filename = "graph/e" + std::to_string(a);
            filename += "o" + std::to_string(idx);
            Log::get()->debug("Saving {0}", filename);
            org.dumpGraph(filename);
            idx++;
        }
    }
}

void Population::dump() {
    Log::get()->debug("Population::dump");

    for(auto& el: species) {
        el.dump();
    }

    //species[0].getOrganisms()[0].dumpGraph("test");
}