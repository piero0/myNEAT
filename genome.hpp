#pragma once
#include "gene.hpp"

namespace pneat {
    class Genome {
        std::vector<NodeType> nodes;
        std::vector<Gene> gens;
        ushort sensorNum;
        ushort outputNum;

        public:
            void testSize();
            void addGene(Gene g);
            void addNode(NodeType nt);
            void print();
            std::vector<Gene>& getGenes();
    };
}
