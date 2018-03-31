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

std::string Gene::print() const {
    return std::to_string(innovationIdx) + " " + std::to_string(fromIdx) + "-" + std::to_string(toIdx) + " " + std::to_string(weight);
}
