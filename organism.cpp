#include "organism.hpp"

using namespace pneat;

Organism::Organism() {
}

void Organism::cloneAndMutate(Genome& genome) {
    gens = genome.getGenes();
    for(auto& el: gens) el.mutateWeight();
}

void Organism::print() {
    for(auto& el: gens) el.print();
}
