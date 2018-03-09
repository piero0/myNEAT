#pragma once
#include <tuple>
#include <chrono>
#include <algorithm>
#include "gene.hpp"
#include "util.hpp"

namespace pneat {
    using Genes = std::vector<Gene>;

    class Genome {
        Genes genes;
        Genome* masterGenome;
        Nodes<ushort> nodes;
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
            Nodes<ushort>& getNodes() { return nodes; }
            void addGene(Gene g) { genes.push_back(g); }
            void print() const;
            
            void mutateWeights();

            void mutateAddNode();
            void mutateAddLink();

            bool checkLinkExist(ushort from, ushort to);
            ushort getNextInnov() const { return genes.size(); }
    };

    class MasterGenome: public Genome {
        //or no inheritance but composition
        //Genome gnm; ??
        //need some more time to design
        //how I want to use it
        public:
            bool checkLinkExist();
            ushort getNextInnov();
            //add a gene to master and return
            //with innov set
            Gene& addGene(Gene& g); 
    };
}
