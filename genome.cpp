#include "genome.hpp"

using namespace pneat;

Genome::Genome() {
}

void Genome::print() const {
    Log::get()->debug("--Start Genome--");
    Log::get()->debug("Nodes: In: {0} Out: {1} Hid: {2}", 
        nodes.getSensorNum(), nodes.getOutputNum(), nodes.getHiddenNum()
    );
    Log::get()->trace("Genes:");
    for(const auto& g: genes) g.print();
    Log::get()->debug("--End Genome--");
}

void Genome::mutateWeights() {
    Log::get()->trace("Genome::mutateWeights");

    Util& u = Util::getInstance();
    auto gen = u.getFRndGen(-0.5, 0.5);

    for(auto& el: genes) el.weight += gen.next();
}

void Genome::mutateAddNode() {
    Log::get()->debug("Genome::mutateAddNode");
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

    this->addLinkedNode(newFrom, newTo, newNodeIdx);
    masterGenome.addLinkedNode(newFrom, newTo, newNodeIdx);
}

void Genome::mutateAddLink(Config* cfg) {
    Log::get()->debug("Genome::mutateAddLink");

    Util& rnd = Util::getInstance();
    MasterGenome& masterGenome = MasterGenome::getInstance();

    ushort fromIdx = 0, toIdx = 0,
        sensNum = nodes.getSensorNum(),
        hidNum = nodes.getHiddenNum(),
        outNum = nodes.getOutputNum(),
        cnt = 0;

    auto gen1 = rnd.getSRndGen(0, sensNum+hidNum-1);
    auto gen2 = rnd.getSRndGen(0, outNum+hidNum-1);

    std::shared_ptr<Gene> genePtr;

    do {
        //fromIdx - non-output node
        fromIdx = gen1.next();
        if(fromIdx >= sensNum) fromIdx += outNum;
        //toIdx - non-sensor node
        toIdx = gen2.next() + sensNum;

        fromIdx = nodes.getIdx(fromIdx);
        toIdx = nodes.getIdx(toIdx);
        
        cnt++;

        //If new gene not in master then add it
        genePtr = masterGenome.checkLinkExist(fromIdx, toIdx);
        if(genePtr == nullptr) break;
        
        //If in master it still can be in the current genome
        if(this->linkExist(fromIdx, toIdx)) continue;
        else break;


    } while(cnt <= cfg->AddLinkMaxTries);

    if(cnt <= cfg->AddLinkMaxTries) {
        Gene g(0,0,0.0,0);

        if(genePtr != nullptr) g = *genePtr;
        else {
            auto gen = rnd.getFRndGen(0.0, 1.0);
            g = Gene(fromIdx, toIdx, gen.next(), masterGenome.getNextInnovation());
        }

        //Genes must be monotonic by innovNum
        //need to rebuild the list if gene is in the middle
        masterGenome.addGene(g);
        this->addGene(g);
    } else {
        Log::get()->debug("AddLink tries limit reached");
    }
}

Genome Genome::crossover(Genome& gnm) {
    Log::get()->trace("Genome::crossover");
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
    nds.setup(nodes.getSensorNum(), nodes.getOutputNum()); //nodeIdx needed

    for(auto& el: nodenums) {
        nds.addNode(el);
    }

    return child;
}

MasterGenome& MasterGenome::getInstance() {
    static MasterGenome mg;
    return mg;
}

std::shared_ptr<Gene> MasterGenome::checkLinkExist(ushort from, ushort to) {
    Log::get()->debug("MasterGenome::checkLinkExist: {0}-{1}", from, to);
    //O(log n)
    auto it = genesSet.find(Gene(from, to, 0.0, 0));
    if(it != genesSet.end()) return std::make_shared<Gene>(*it);
    return nullptr;
}

void MasterGenome::initFromGenome(Genome& gnm) {
    this->genes = gnm.getGenes();
    this->nodes = gnm.getNodes();
    for(auto& el: genes) genesSet.insert(el);
}
