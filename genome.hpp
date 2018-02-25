#pragma once
#include <tuple>
#include <chrono>
#include <algorithm>
#include "gene.hpp"
#include "util.hpp"

namespace pneat {
    using Genes = std::vector<Gene>;
    using short3 = std::tuple<ushort, ushort, ushort>;

    class Genome {
        Genes genes;
        Genome* masterGenome;
        ushort sensorNum;
        ushort outputNum;
        ushort hiddenNum;
        /* Maintain only a count of sensor,output and hidden nodes
            calculate indexes based on these numbers:
            Sensors: from 0 to sensNum-1
            Outputs: from sensNum to sensNum+outputNum-1
            Hidden: from sensNum+outputNum to the end (sum of all)
        */

        public:
            Genome();
            void setMaster(Genome* gnm) { masterGenome = gnm; }
            Genes& getGenes() { return genes; }
            void addGene(Gene g) { genes.push_back(g); }
            void addNode(NodeType nt);
            void print();
            
            short3 getNodeNum();
            void mutateWeights();

            void mutateAddNode();
            void mutateAddLink();

            bool checkLinkExist(ushort from, ushort to);
            ushort getNextInnov() { return genes.size(); }
    };
}
