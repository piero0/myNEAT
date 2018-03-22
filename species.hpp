#pragma once
#include "organism.hpp"
#include "util.hpp"

namespace pneat {
    class Species {
        std::vector<Organism> orgs;
        Config* cfg;

        float compabilityDistance,
            totalFitness;

        public:
            Species(Config* cfg);
            void updateFitness();
            void doCrossover();
            void getOutcasts();
            
            void addOrganism(Organism& org) { orgs.push_back(org); }
            std::vector<Organism>& getOrganisms() { return orgs; }
            Organism& randomPick(float probabilty);
            void calcCompatibilityDistans();
            void dump();

            void prepareFitness();
    };
}
