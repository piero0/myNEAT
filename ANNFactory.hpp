#pragma once
#include "genome.hpp"
#include <util.hpp>

namespace pneat {
    template<class T> class annNode {
        std::vector<T> links;
        T idx;
        //type input/output/hidden?

        public:
            annNode(T index) {idx = index;}
            void addLink(T idx) {
                links.push_back(idx);
            }
    };

    class ANN {
        std::vector<annNode<ushort>> nodes;
    };

    class ANNFactory {
    public:
        ANN createFromGenome(std::shared_ptr<pneat::Genome> gnm);
        ANN createFromConfig(std::string filename);
    };
}