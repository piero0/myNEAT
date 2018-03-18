#include "organism.hpp"

using namespace pneat;

Organism::Organism(const Genome& g) {
    this->g = g;
    fitness = 0.0;
}

void Organism::dump() {
    std::cout << "Organism" << std::endl;
    std::cout << "Fitness: " << fitness << std::endl;
    g.print();
}