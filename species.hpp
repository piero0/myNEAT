#pragma once
#include "organism.hpp"

using namespace pneat;

class Species {
    std::vector<Organism> orgs;
    float compabilityDistance;

    public:
        Species();
        void updateFitness(Organism& organism);
        void doCrossover();
        void getOutcasts();
        
        void addOrganism(Organism& org) { orgs.push_back(org); }
        void calcCompatibilityDistans();
        void dump();
};
