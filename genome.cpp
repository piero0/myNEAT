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

bool Genome::isMatch(geneIt it, ushort innov) {
    if(it != genes.end() && it->innovationIdx == innov) {
        Log::get()->debug("Innovation in current genome");
        return true;
    }
    else {
        Log::get()->debug("Innovation not in current genome");
        return false;
    }
}

void Genome::mutateWeights() {
    Log::get()->trace("Genome::mutateWeights");

    Util& u = Util::getInstance();
    auto gen = u.getFRndGen(-0.5, 0.5);

    for(auto& el: genes) el.weight += gen.next();
}

void Genome::insertExistingGenes(ushort innov) {
    Log::get()->trace("Genome::insertExistingGenes");
    auto& mg = MasterGenome::getInstance();

    Gene& g1 = mg.getGenes()[innov];
    Gene& g2 = mg.getGenes()[innov+1];

    Log::get()->debug("Genes to insert: {0}, {1}", g1.innovationIdx, g2.innovationIdx);

    auto it = this->findInnovation(innov);
    Log::get()->debug("Inserting before: {0}", it->innovationIdx);

    genes.insert(it, {g1, g2});
}

void Genome::createNewNodeFrom(Gene& g) {
    Log::get()->trace("Genome::createNewNodeFrom");
    auto& mg = MasterGenome::getInstance();

    auto newNodeIdx = mg.getNodes().getNextNodeIdx();
    auto newInnov = mg.getNextInnovation();

    Log::get()->debug("newNodeIdx {0}, newInnov {1}", newNodeIdx, newInnov);

    g.childIdx = newInnov;
    Gene newFrom = Gene(g.fromIdx, newNodeIdx, g.weight, newInnov);
    Gene newTo = Gene(newNodeIdx, g.toIdx, 1.0f, newInnov+1);

    newFrom.print();
    newTo.print();
 
    this->addLinkedNode(newFrom, newTo, newNodeIdx);
    mg.addLinkedNode(newFrom, newTo, newNodeIdx);
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

    while(true) {
        geneIdx = gen.next();
        if(!genes[geneIdx].enabled) continue;
        
        Gene& g = genes[geneIdx];

        Log::get()->debug("Picked gene at idx {0}", geneIdx);
        Log::get()->debug("Gene: {0}-{1} innov {2}", g.fromIdx, g.toIdx, g.innovationIdx);
        
        //Check in master
        Gene& mg = masterGenome.getGenes()[g.innovationIdx];

        Log::get()->debug("From master: innov {0}, child {1}", mg.innovationIdx, mg.childIdx);

        if(mg.innovationIdx != g.innovationIdx) {
            Log::get()->error("Wrong Innovation index: {0} != {1}", mg.innovationIdx, g.innovationIdx);
            return;
        }

        if(mg.childIdx != 0) {
            auto it = this->findInnovation(mg.childIdx);
            if(this->isMatch(it, mg.childIdx)) {
                Log::get()->debug("Genes in current genome, next try");
                continue;
            }
        }

        g.enabled = false;

        if(mg.childIdx != 0) {
            this->insertExistingGenes(mg.childIdx);
        } else {
            this->createNewNodeFrom(mg);
        }
        break;
    }
}

void Genome::mutateAddLink(Config* cfg) {
    Log::get()->debug("Genome::mutateAddLink");

    Util& rnd = Util::getInstance();
    MasterGenome& masterGenome = MasterGenome::getInstance();

    ushort fromIdx = 0, toIdx = 0,
        sensNum = nodes.getSensorNum(),
        hidNum = nodes.getHiddenNum(),
        outNum = nodes.getOutputNum(),
        cnt = -1;

    auto gen1 = rnd.getSRndGen(0, sensNum+hidNum-1);
    auto gen2 = rnd.getSRndGen(0, outNum+hidNum-1);

    std::shared_ptr<Gene> genePtr;
    geneIt it;

    do {
        cnt++;
        //fromIdx - non-output node
        fromIdx = gen1.next();
        if(fromIdx >= sensNum) fromIdx += outNum;
        //toIdx - non-sensor node
        toIdx = gen2.next() + sensNum;

        Log::get()->debug("Idxs: {0}-{1}", fromIdx, toIdx);

        fromIdx = nodes.getIdx(fromIdx);
        toIdx = nodes.getIdx(toIdx);

        Log::get()->debug("Nodes: {0}-{1}", fromIdx, toIdx);
        
        //If new gene not in master then add it
        genePtr = masterGenome.checkLinkExist(fromIdx, toIdx);
        if(genePtr == nullptr) {
            Log::get()->debug("Link not in master");
            break;
        }
        
        //If in master it still can be in the current genome
        //find using innov num
        it = this->findInnovation(genePtr->innovationIdx);
        if(this->isMatch(it, genePtr->innovationIdx)) continue;
        else break;
 
    } while(cnt <= cfg->AddLinkMaxTries);

    if(cnt <= cfg->AddLinkMaxTries) {
        if(genePtr != nullptr) { //gene exist, insert into current genome
            Log::get()->debug("Insert existing gene before pos {0}", it-genes.begin());
            genes.insert(it, *genePtr);
        } else { //doesn't exist, create and add at the end
            Log::get()->debug("Creating new link gene");
            auto gen = rnd.getFRndGen(0.0, 1.0);
            auto g = Gene(fromIdx, toIdx, gen.next(), masterGenome.getNextInnovation());
            masterGenome.addGene(g);
            this->addGene(g);
        }
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
    genesSet.clear();
    for(auto& el: genes) genesSet.insert(el);
}

void MasterGenome::addLinkedNode(Gene& from, Gene& to, ushort nodeIdx) {
    addNode(nodeIdx); 
    addGene(from); 
    addGene(to);
    genesSet.insert(from);
    genesSet.insert(to);
}
