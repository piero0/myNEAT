#include "population.hpp"

using namespace pneat;

Population::Population(): 
    masterGenome(MasterGenome::getInstance()) 
    {
}

void Population::initPopulation(std::shared_ptr<Config> cfg, Genome& baseGenome) {
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
        sp.doCrossover();

        if(cfg->dumpGraphs) {
            Log::get()->info("Epoch dump");
            for(auto& org: sp.getOrganisms()) {
                auto& gnm = org.getGenome();
                Log::get()->info(gnm.printGenes());
                Log::get()->info(gnm.printWeights());
            }
            Log::get()->info("Epoch dump end");
        }
    }

    if(cfg->dumpGraphs) { 
        Log::get()->info("Master genome");
        auto& mg = MasterGenome::getInstance();
        auto& nds = mg.getNodes();
        Log::get()->info("|{0}|{1}|",nds.getSensorNum(), nds.getOutputNum());
        for(auto& g: mg.getGenes()) {
            Log::get()->info(g.print());
        }
        Log::get()->info("Master genome end");
    }
}

void Population::dump() {
    Log::get()->debug("Population::dump");

    for(auto& el: species) {
        el.dump();
    }

    //species[0].getOrganisms()[0].dumpGraph("test");
}