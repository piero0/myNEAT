#include "population.hpp"

using namespace pneat;

Population::Population(): 
    masterGenome(MasterGenome::getInstance()) 
    {
        bestFitness = 0.0;
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

void Population::loop() {
    uint epochNum = 0;

    while(cfg->maxEpoch > 0 && cfg->maxEpoch < epochNum) {

        epochNum++;

        for(auto& sp: species) {
            Log::get()->debug("Epoch: {0}", epochNum);

            this->updateFitness(sp);
            sp.doCrossover();

            if(cfg->dumpGraphs) this->dumpEpoch(sp);
        }
    }

    if(cfg->dumpGraphs) this->dumpMasterGenome();
}

void Population::updateFitness(Species& sp) {
    sp.updateFitness();
}

void Population::dumpEpoch(Species& sp) {
    Log::get()->info("Epoch dump");
    for(auto& org: sp.getOrganisms()) {
        auto& gnm = org.getGenome();
        Log::get()->info(gnm.printGenes());
        Log::get()->info(gnm.printWeights());
    }
    Log::get()->info("Epoch dump end");
}

void Population::dumpMasterGenome() {
    Log::get()->info("Master genome");
    auto& mg = MasterGenome::getInstance();
    auto& nds = mg.getNodes();
    Log::get()->info("|{0}|{1}|", nds.getSensorNum(), nds.getOutputNum());
    for(auto& g: mg.getGenes()) Log::get()->info(g.print());
    Log::get()->info("Master genome end");
}

void Population::dump() {
    Log::get()->debug("Population::dump");
    for(auto& el: species) el.dump();
}
