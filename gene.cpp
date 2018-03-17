#include "gene.hpp"

using namespace pneat;

Gene::Gene(ushort from, ushort to, float weight, ushort innov) {
    fromIdx = from;
    toIdx = to;
    this->weight = weight;
    innovationIdx = innov;
    enabled = true;    
}

void Gene::print() const {
    std::cout << innovationIdx << " " << fromIdx << "-" << toIdx << " " << weight << std::endl;
}
