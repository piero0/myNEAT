#include "ANNFactory.hpp"

using namespace pneat;

std::vector<ushort> ANN::createIndexMap(std::vector<ushort>& idxs) {
    auto nodesMap = std::vector<ushort>(idxs[idxs.size()-1]);
    for(size_t i=0; i<idxs.size(); i++)
        nodesMap[idxs[i]] = i;
    return nodesMap;
}
 
void ANN::setupFromGenome(Genome& gnm) {
    Genes gns = gnm.getGenes();
    auto idxs = gnm.getNodes().getIdxs();

    auto idx2node = this->createIndexMap(idxs);

    //now we can use vector indecies instead of nodeIdx
    //as there can be holes between nodeIdxs

    this->initNodes(idxs.size());

    for(auto& g: gns) {
        //check if 'from' exist already
        //create or add link
        //and so on...

        auto& netNode = nodes[idx2node[g.fromIdx]];

        netNode.idx = g.toIdx;
        netNode.links.push_back(g.fromIdx);
        netNode.weights.push_back(g.weight);

    };

}

void ANN::activate(std::vector<float>& input) {
    //put inputs to input nodes and go on...
    //run a function using picked algorithm

    // this->setupInputNodes(input);

    // auto output = this->recurentActivate();
    //this->smartActivate();
    //return output
}
