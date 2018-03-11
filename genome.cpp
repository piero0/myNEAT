#include "genome.hpp"

using namespace pneat;

Genome::Genome() {
    masterGenome = this;
}

void Genome::print() const {
    std::cout << "Nodes:" << std::endl;
    std::cout << "Sensor: " << nodes.getSensorNum() 
    << " Output: " << nodes.getOutputNum()
    << " Hidden: " << nodes.getHiddenNum() << std::endl;
    std::cout << "Genes:" << std::endl;
    for(const auto& g: genes) g.print();
}

void Genome::mutateWeights() {
    Util& u = Util::getRandomGen();
    u.setFloatDist(-0.5, 0.5);

    for(auto& el: genes) el.weight += u.nextFloat();
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
    ushort newNodeIdx = nodes.getCount() + 1;

    //Create new links
    Gene newFrom = Gene(g.fromIdx, newNodeIdx, g.weight, masterGenome->getNextInnov());
    Gene newTo = Gene(newNodeIdx, g.toIdx, g.weight, masterGenome->getNextInnov());

    nodes.addNode();
    this->addGene(newFrom);
    this->addGene(newTo);

    masterGenome->getNodes().addNode();
    masterGenome->addGene(newFrom);
    masterGenome->addGene(newTo);
}

void Genome::mutateAddLink() {
    Util& rnd = Util::getRandomGen();
    ushort fromIdx = 0, toIdx = 0,
        sensNum = nodes.getSensorNum(),
        hidNum = nodes.getHiddenNum(),
        outNum = nodes.getOutputNum();
    do {
        //fromIdx - non-output node
        rnd.setIntDist(0, sensNum+hidNum-1);
        fromIdx = rnd.nextInt();
        if(fromIdx >= sensNum) fromIdx += outNum;
        //toIdx - non-sensor node
        rnd.setIntDist(0, outNum+hidNum-1);
        toIdx = rnd.nextInt() + sensNum;
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

MasterGenome& MasterGenome::getInstance() {
    static MasterGenome mg;
    return mg;
}

void MasterGenome::initFrom(Genome& gnm) {
    this->genes = gnm.getGenes();
    this->nodes = gnm.getNodes();
}
