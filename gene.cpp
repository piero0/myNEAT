#include "gene.hpp"

using namespace pneat;

Gene::Gene(ushort from, ushort to, float weight, ushort innov) {
    fromIdx = from;
    toIdx = to;
    this->weight = weight;
    innovationIdx = innov;
    enabled = true;    
}

void Gene::print() {
    std::cout << innovationIdx << " " << fromIdx << "-" << toIdx << " " << weight << std::endl;
}

void Gene::setInnov(ushort innov) {
    this->innovationIdx = innov;
}

void Gene::mutateWeight() {
    // TODO: move this into a better place
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<> dis(-0.5, 0.5);
    weight += dis(gen);
}