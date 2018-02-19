#pragma once
#include "genome.hpp"

namespace pneat {
    class Organism {
        std::vector<Gene> gens;

        public:
            Organism();
            void cloneAndMutate(Genome& genome);
            void print();
    };
}
