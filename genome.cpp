#include "genome.hpp"

using namespace pneat;

void Genome::testSize() {
    std::cout << "Gens " << sizeof(gens) << std::endl;
    std::cout << "Nodes " << sizeof(nodes) << std::endl;
}
