#pragma once
#include "genome.hpp"

namespace pneat {
    class Organism {
        Genome g;

        public:
            Organism(Genome& g);
            Genome& getGenome() { return g; }
    };
}
