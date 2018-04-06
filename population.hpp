#pragma once
#include "species.hpp"

namespace pneat {
    class Population {
        std::vector<Species> species;
        MasterGenome& masterGenome;
        std::shared_ptr<Config> cfg;
        float bestFitness;

        void dumpEpoch(Species& sp);
        void dumpMasterGenome();

        public:
            Population();
            void initPopulation(std::shared_ptr<Config> cfg, Genome& baseGenome);
            void dump();
            void loop();
            void updateFitness(Species& sp);
    };
}