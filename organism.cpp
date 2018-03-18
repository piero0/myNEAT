#include "organism.hpp"

using namespace pneat;

Organism::Organism(const Genome& g) {
    this->g = g;
    fitness = 0.0;
}
