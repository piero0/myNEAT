#pragma once
#include "species.hpp"

namespace pneat {
    class Population {
        std::vector<Species> species;
        MasterGenome& masterGenome;
        Config* cfg;

        public:
            Population();
            void initPopulation(Config* cfg, Genome& baseGenome);
            void dump();
            void testLoop();
    };
}