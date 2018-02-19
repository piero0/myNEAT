#include "genome.hpp"

using namespace pneat;

void Genome::testSize() {
    std::cout << "Gens " << sizeof(gens) << std::endl;
    std::cout << "Nodes " << sizeof(nodes) << std::endl;
}

void Genome::addGene(Gene g) {
    g.setInnov(gens.size());
    gens.push_back(g);
}

void Genome::addNode(NodeType nt) {
    nodes.push_back(nt);
}

void Genome::print() {
    ushort idx = 0;
    std::cout << "Nodes" << std::endl;
    for(auto& n: nodes) std::cout << idx++ << n << std::endl;
    std::cout << "Genes" << std::endl;
    for(auto& g: gens) g.print();
}

std::vector<Gene>& Genome::getGenes() {
    return gens;
}
