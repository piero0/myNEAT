#include "gene.hpp"
#include "util.hpp"

using namespace pneat;

Gene::Gene(ushort from, ushort to, float weight, ushort innov) {
    fromIdx = from;
    toIdx = to;
    this->weight = weight;
    innovationIdx = innov;
    enabled = true;    
    childIdx = 0;
}

void Gene::print() const {
    Log::get()->trace("{0} {1}-{2} {3}", innovationIdx, fromIdx, toIdx, weight);
}
