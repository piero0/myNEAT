#include "genome.hpp"
#include "util.hpp"

using namespace pneat;

Genome::Genome() {
    sensorNum = 0;
    outputNum = 0;
    hiddenNum = 0;
}

void Genome::testSize() {
    std::cout << "Gens " << sizeof(genes) << std::endl;
    //std::cout << "Nodes " << sizeof(nodes) << std::endl;
}

void Genome::addGene(Gene g) {
    g.setInnov(genes.size());
    genes.push_back(g);
}

void Genome::addNode(NodeType nt) {
    switch(nt) {
        case NodeType::SENSOR:
            sensorNum++; break;
        case NodeType::OUTPUT:
            outputNum++; break;
        case NodeType::HIDDEN:
            hiddenNum++; break;    
    };
}

void Genome::print() {
    std::cout << "Nodes:" << std::endl;
    std::cout << "Sensor: " << sensorNum << " Output: "
    << outputNum << " Hidden: " << hiddenNum << std::endl;
    std::cout << "Genes:" << std::endl;
    for(auto& g: genes) g.print();
}

void Genome::mutateWeights() {
    Util& u = Util::getRandomGen();
    u.setFloatDist(-0.5, 0.5);

    for(auto& el: genes) el.weight += u.nextFloat();
}

short3 Genome::getNodeNum() {
    return std::make_tuple(sensorNum, outputNum, hiddenNum);
}

void Genome::mutateAddNode() {

}

void Genome::mutateAddLink() {
    //from non-output
    //idx = rand % all - outNum
    //if idx > sensNum
    // idx += sensNum + outNum
    
    //to non-sensor
    //rand() + sensNum % all nodes
}