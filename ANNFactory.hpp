#pragma once
#include "genome.hpp"
#include "util.hpp"

namespace pneat {
    class annNode {
        public:
            std::vector<ushort> links;
            std::vector<float> weights;
            std::vector<float> inputs;
            std::vector<float> outputs;

        ushort idx;
        //type input/output/hidden?
        //vector<float> weights??
            annNode() {idx=0;}
            annNode(ushort index) {idx = index;}
            void addLink(ushort idx) {
                links.push_back(idx);
            }
    };

    class ANN {
        std::vector<annNode> nodes;

        void initNodes(size_t num) {
            nodes = std::vector<annNode>(num);
        }
        
        public:
            std::vector<ushort> createIndexMap(std::vector<ushort>& idxs);
            void setupFromGenome(pneat::Genome& gnm);
            void activate(std::vector<float>& input);
    };

    // class ANNFactory {
    // public:
    //     ANN createFromGenome(std::shared_ptr<pneat::Genome> gnm);
    //     ANN createFromConfig(std::string filename);
    // };
}