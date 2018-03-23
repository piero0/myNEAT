#pragma once
#include <tuple>
#include <chrono>
#include <algorithm>
#include <set>
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
            void addNode(ushort nodeIdx) { nodes.addNode(nodeIdx); }
            ushort getNodesCount() { return nodes.getCount(); }

            void addLinkedNode(Gene& from, Gene& to, ushort nodeIdx) {
                addNode(nodeIdx); addGene(from); addGene(to);
            }

            bool linkExist(ushort from, ushort to) {
                return std::binary_search(genes.begin(), genes.end(), Gene(from, to, 0.0, 0));
            }

            void print() const;
            
            std::string printGenes() {
                std::string txt = "|";
                for(auto& el: genes) txt += std::to_string(el.innovationIdx) + "|";
                return txt;
            }
            
            void mutateWeights();
            void mutateAddNode();
            void mutateAddLink(Config* cfg);

            Genome crossover(Genome& gnm);
    };

    class MasterGenome: public Genome {
            std::set<Gene> genesSet;
            MasterGenome() {}
            MasterGenome(MasterGenome const& m) = delete;
            void operator=(MasterGenome const& m) = delete;
        public:
            static MasterGenome& getInstance();
            void initFromGenome(Genome& gnm);
            std::shared_ptr<Gene> checkLinkExist(ushort from, ushort to);
            ushort getNextInnovation() { return genes.size()+1; }
            void addGene(Gene& g) { genes.push_back(g); genesSet.insert(g); }
    };
}
