#pragma once
#include <tuple>
#include <chrono>
#include <algorithm>
#include "gene.hpp"
#include "util.hpp"

namespace pneat {
    using Genes = std::vector<Gene>;

    class Genome {
        protected:
            Genes genes;
            Nodes<ushort> nodes;
        /* Maintain only a count of sensor,output and hidden nodes
            calculate indexes based on these numbers:
            Sensors: from 0 to sensNum-1
            Outputs: from sensNum to sensNum+outputNum-1
            Hidden: from sensNum+outputNum to the end (sum of all)
        */

        public:
            Genome();
            Genes& getGenes() { return genes; }
            Nodes<ushort>& getNodes() { return nodes; }

            void addGene(Gene g) { genes.push_back(g); }
            void addNode() { nodes.addNode(); }
            ushort getNodesCount() { return nodes.getCount(); }

            void addLinkedNode(Gene& from, Gene& to) {
                addNode(); addGene(from); addGene(to);
            }

            void print() const;
            
            void mutateWeights();
            void mutateAddNode();
            void mutateAddLink();

            Genome crossover(Genome& gnm);
    };

    class MasterGenome: public Genome {
        MasterGenome() {}
        MasterGenome(MasterGenome const& m) = delete;
        void operator=(MasterGenome const& m) = delete;
        public:
            static MasterGenome& getInstance();
            void initFromGenome(Genome& gnm);
            bool checkLinkExist(ushort from, ushort to);
            ushort getNextInnovation() { return genes.size()+1; }
            //add a gene to master and return
            //with innov set
            //Gene& addGene(Gene& g); 
    };
}
