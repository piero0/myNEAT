#include "genome.hpp"

using namespace pneat;

Genome::Genome() {
    sensorNum = 0;
    outputNum = 0;
    hiddenNum = 0;
    masterGenome = this;
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
    /*
    1) pick a link
    2) disable it
    3) add a new node between from and to
        a) create a link from fromIdx to new node
        b) -||-          from new node to toIdx
        c) add new node
    */
    //Randomly pick a link
    Util& rnd = Util::getRandomGen();
    rnd.setIntDist(0, genes.size()-1);
    ushort geneIdx = 0;
     
    do { //If link's disabled it's been probably mutate already
        geneIdx = rnd.nextInt();
    } while(!genes[geneIdx].enabled);

    Gene& g = genes[geneIdx];
    g.enabled = false;

    //Does the number is the same for the master and here?
    ushort newNodeIdx = sensorNum+outputNum+hiddenNum+1;

    //Create new links
    Gene newFrom = Gene(g.fromIdx, newNodeIdx, g.weight, masterGenome->getNextInnov());
    Gene newTo = Gene(newNodeIdx, g.toIdx, g.weight, masterGenome->getNextInnov());

    this->addNode(NodeType::HIDDEN);
    this->addGene(newFrom);
    this->addGene(newTo);

    masterGenome->addNode(NodeType::HIDDEN);
    masterGenome->addGene(newFrom);
    masterGenome->addGene(newTo);
}

void Genome::mutateAddLink() {
    Util& rnd = Util::getRandomGen();
    ushort fromIdx = 0, toIdx = 0;
    do {
        //fromIdx - non-output node
        rnd.setIntDist(0, sensorNum+hiddenNum-1);
        fromIdx = rnd.nextInt();
        if(fromIdx >= sensorNum) fromIdx += outputNum;
        //toIdx - non-sensor node
        rnd.setIntDist(0, outputNum+hiddenNum-1);
        toIdx = rnd.nextInt() + sensorNum;
    } while( masterGenome->checkLinkExist(fromIdx, toIdx) );

    rnd.setFloatDist(0.0, 1.0);
    Gene g(fromIdx, toIdx, rnd.nextFloat(), masterGenome->getNextInnov());

    masterGenome->addGene(g);
    this->addGene(g);
}

bool Genome::checkLinkExist(ushort from, ushort to) {
    //naive version (O(n)*orgNum)
    for(auto& g: genes) {
        if(g.fromIdx == from && g.toIdx == to) return true;
    }
    return false;
}
