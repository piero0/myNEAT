#include "ANNFactory.hpp"

using namespace pneat;

ANN ANNFactory::createFromGenome(std::shared_ptr<Genome> gnm) {
    Genes gns = gnm->getGenes();
    auto idxs = gnm->getNodes().getIdxs();

    //make indecies map
    auto nodesMap = std::vector<ushort>(idxs[idxs.size()-1]);
    for(size_t i=0; i<idxs.size(); i++)
        nodesMap[idxs[i]] = i;

    //now we can use vector indecies instead of nodeIdx
    //as there can be holes between nodeIdxs

    for(auto& g: gns) {
        //check if 'from' exist already
        //create or add link
        //and so on...
    };

    return ANN();
}
