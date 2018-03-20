#pragma once
#include "species.hpp"

namespace pneat {
    class Population {
        std::vector<Species> species;
        MasterGenome& masterGenome;

        public:
            Population();
            void initPopulation(ushort size, Genome& baseGenome);
            void dump();
            void testLoop(std::size_t iterNum);
    };
}