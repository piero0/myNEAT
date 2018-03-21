#include "genome.hpp"

using namespace pneat;

Genome::Genome() {
}

void Genome::print() const {
    std::cout << "Genome" << std::endl;
    std::cout << "Nodes:" << std::endl;
    std::cout << "Sensor: " << nodes.getSensorNum() 
    << " Output: " << nodes.getOutputNum()
    << " Hidden: " << nodes.getHiddenNum() << std::endl;
    std::cout << "Genes:" << std::endl;
    for(const auto& g: genes) g.print();
}

void Genome::mutateWeights() {
    std::cout << "mutateWeights" << std::endl;
    Util& u = Util::getInstance();
    auto gen = u.getFRndGen(-0.5, 0.5);

    for(auto& el: genes) el.weight += gen.next();
}

void Genome::mutateAddNode() {
    std::cout << "mutateAddNode" << std::endl;
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
    auto gen = rnd.getSRndGen(0, genes.size()-1);
    ushort geneIdx = 0;
     
    do { //If link's disabled it's probably been mutated already
        geneIdx = gen.next();
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
    Gene newTo = Gene(newNodeIdx, g.toIdx, 1.0f, masterGenome.getNextInnovation()+1);

    this->addLinkedNode(newFrom, newTo);
    masterGenome.addLinkedNode(newFrom, newTo);
}

void Genome::mutateAddLink() {
    std::cout << "mutateAddLink" << std::endl;
    Util& rnd = Util::getInstance();
    MasterGenome& masterGenome = MasterGenome::getInstance();
    ushort fromIdx = 0, toIdx = 0,
        sensNum = nodes.getSensorNum(),
        hidNum = nodes.getHiddenNum(),
        outNum = nodes.getOutputNum(),
        cnt = 0;
    const ushort tryLimit = 5;

    auto gen1 = rnd.getSRndGen(0, sensNum+hidNum-1);
    auto gen2 = rnd.getSRndGen(0, outNum+hidNum-1);

    do {
        //fromIdx - non-output node
        fromIdx = gen1.next();
        if(fromIdx >= sensNum) fromIdx += outNum;
        //toIdx - non-sensor node
        toIdx = gen2.next() + sensNum;
        
        cnt++;
    } while( masterGenome.checkLinkExist(fromIdx, toIdx) && (cnt <= tryLimit));

    if(cnt <= tryLimit) {
        auto gen = rnd.getFRndGen(0.0, 1.0);
        Gene g(fromIdx, toIdx, gen.next(), masterGenome.getNextInnovation());

        masterGenome.addGene(g);
        this->addGene(g);
    } else {
        std::cerr << "AddLink limit reached" << std::endl;
    }
}

Genome Genome::crossover(Genome& gnm) {
    std::cout << "crossover" << std::endl;
    auto& utl = Util::getInstance();
    auto gen = utl.getFRndGen(0.0, 1.0);

    Genome child;

    auto& Par2Genes = gnm.getGenes();

    auto pit1 = genes.begin();
    auto pit2 = Par2Genes.begin();

    //add matching and disjoint genes
    while(pit1 != genes.end() && pit2 != Par2Genes.end()) {
        if(pit1->innovationIdx < pit2->innovationIdx) {
            child.addGene(*pit1); pit1++;
        } else if(pit1->innovationIdx == pit2->innovationIdx){
            child.addGene((gen.next() < 0.5) ? *pit1 : *pit2);
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
    std::cout << "checkLinkExist: " << from << " " << to << std::endl;
    //O(log n)
    if(genesSet.find(Gene(from, to, 0.0, 0)) != genesSet.end()) return true;
    return false;
}

void MasterGenome::initFromGenome(Genome& gnm) {
    this->genes = gnm.getGenes();
    this->nodes = gnm.getNodes();
    for(auto& el: genes) genesSet.insert(el);
}
