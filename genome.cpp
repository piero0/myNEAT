#include "genome.hpp"

using namespace pneat;

Genome::Genome() {
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
    Util& u = Util::getInstance();
    u.setFloatDist(-0.5, 0.5);

    for(auto& el: genes) el.weight += u.nextFloat();
}

void Genome::mutateAddNode() {
    MasterGenome& masterGenome = MasterGenome::getInstance();
    /*
    1) pick a link
    2) disable it
    3) add a new node between from and to
        a) create a link from fromIdx to new node
        b) -||-          from new node to toIdx
        c) add new node
    */
    //Randomly pick a link
    Util& rnd = Util::getInstance();
    rnd.setIntDist(0, genes.size()-1);
    ushort geneIdx = 0;
     
    do { //If link's disabled it's probably been mutated already
        geneIdx = rnd.nextInt();
    } while(!genes[geneIdx].enabled);

    Gene& g = genes[geneIdx];
    g.enabled = false;

    ushort newNodeIdx = nodes.getCount() + 1,
    masterCount = masterGenome.getNodesCount() + 1;

    /* MasterGenome can have a higher idx number
        e.g. Assume this genome's link is between 3-4, newIdx can be 5
        but in master it might be for example maxIdx = 9
        and the master is always right :) 
    */
    if(newNodeIdx < masterCount) newNodeIdx = masterCount;

    //Create new links
    Gene newFrom = Gene(g.fromIdx, newNodeIdx, g.weight, masterGenome.getNextInnovation());
    Gene newTo = Gene(newNodeIdx, g.toIdx, 1.0f, masterGenome.getNextInnovation());

    this->addLinkedNode(newFrom, newTo);
    masterGenome.addLinkedNode(newFrom, newTo);
}

void Genome::mutateAddLink() {
    Util& rnd = Util::getInstance();
    MasterGenome& masterGenome = MasterGenome::getInstance();
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
    } while( masterGenome.checkLinkExist(fromIdx, toIdx) );

    rnd.setFloatDist(0.0, 1.0);
    Gene g(fromIdx, toIdx, rnd.nextFloat(), masterGenome.getNextInnovation());

    masterGenome.addGene(g);
    this->addGene(g);
}

Genome Genome::crossover(Genome& gnm) {
    auto& utl = Util::getInstance();
    utl.setFloatDist(1.0, 2.0);

    Genome child;

    auto& Par2Genes = gnm.getGenes();

    auto pit1 = genes.begin();
    auto pit2 = Par2Genes.begin();

    //add matching and disjoint genes
    while(pit1 != genes.end() && pit2 != Par2Genes.end()) {
        if(pit1->innovationIdx < pit2->innovationIdx) {
            child.addGene(*pit1); pit1++;
        } else if(pit1->innovationIdx == pit2->innovationIdx){
            child.addGene((utl.nextFloat() < 0.5) ? *pit1 : *pit2);
            pit1++; pit2++;
        } else if(pit1->innovationIdx > pit2->innovationIdx) {
            child.addGene(*pit2); pit2++;
        }
    }

    //add excess genes (if any, at least one iterator is already at end())
    for(;pit1 != genes.end(); pit1++) child.addGene(*pit1);
    for(;pit2 != Par2Genes.end(); pit2++) child.addGene(*pit2);

    //Set nodes count
    auto& nds = child.getNodes();

    ushort sensAndOut = nodes.getSensorNum() + nodes.getOutputNum();

    std::set<ushort> nodenums;

    auto checkFunc = [&](ushort idx){
        if(idx >= sensAndOut)
            if(nodenums.find(idx) == nodenums.end())
                nodenums.insert(idx);
    };

    for(auto& el: child.getGenes()) {
        checkFunc(el.fromIdx);
        checkFunc(el.toIdx);
    }

    //sensors and outputs counts do not change in my implementation
    nds.setup(nodes.getSensorNum(), nodes.getOutputNum(), nodenums.size());

    return child;
}

MasterGenome& MasterGenome::getInstance() {
    static MasterGenome mg;
    return mg;
}

bool MasterGenome::checkLinkExist(ushort from, ushort to) {
    //naive version (O(n)*orgNum)
    for(auto& g: genes) {
        if(g.fromIdx == from && g.toIdx == to) return true;
    }
    return false;
}

void MasterGenome::initFromGenome(Genome& gnm) {
    this->genes = gnm.getGenes();
    this->nodes = gnm.getNodes();
}
