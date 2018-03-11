#pragma once
#include "genome.hpp"

namespace pneat {
    class Organism {
        Genome g;

        public:
            Organism(const Genome& g);
            Genome& getGenome() { return g; }
    };
}
