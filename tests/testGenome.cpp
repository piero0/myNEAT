#include "catch.hpp"
#include "../genome.hpp"

using namespace pneat;

TEST_CASE("Genome", "[genome]") {
    Log::initLog();
    Log::set_level("trace");
    
    auto& rnd = Util::getInstance();
    //rnd.init(1234); //make sure we mutate the same way for each test
    Genome gnm;

    auto& g = gnm.getGenes();
    REQUIRE(g.size() == 0);

    SECTION("Check Nodes Count is 0") {
        REQUIRE(gnm.getNodes().getCount() == 0);
    }

    SECTION("Add Genes") {
        Gene g1(1,2,0.5,0), g2(2,3,0.7,1), g3(3,4,0.1,2);
        gnm.addGene(g1);
        gnm.addGene(g2); 
        gnm.addGene(g3);
        REQUIRE(gnm.getGenes().size() == 3);
        
        SECTION("Mutate gene's weights") {
            gnm.mutateWeights();
            Genes& gns = gnm.getGenes();

            REQUIRE(gns[0].weight != 0.5);
            REQUIRE(gns[1].weight != 0.7);
            REQUIRE(gns[2].weight != 0.1);
        }

        SECTION("clone genome") {
            Genome gnm2 = gnm;
            gnm2.mutateWeights();

            Genes& gns1 = gnm.getGenes();
            Genes& gns2 = gnm2.getGenes();

            REQUIRE(gns1[0].weight != gns2[0].weight);
            REQUIRE(gns1[1].weight != gns2[1].weight);
            REQUIRE(gns1[2].weight != gns2[2].weight);
        }
    }

    SECTION("Innovation Present") {
        Gene g1(0,0,0.0,0),
            g2(0,0,0.0,1),
            g3(0,0,0.0,2),
            g4(0,0,0.0,4),
            g5(0,0,0.0,6);

        Genome gnm2;
        gnm2.addGene(g1);
        gnm2.addGene(g2);
        gnm2.addGene(g3);
        gnm2.addGene(g4);
        gnm2.addGene(g5);

        auto it = gnm2.findInnovation(3);
        REQUIRE(it->innovationIdx != 3); //got next one

        it = gnm2.findInnovation(7);
        REQUIRE(it == gnm2.getGenes().end()); 

        it = gnm2.findInnovation(2);
        REQUIRE(it->innovationIdx == 2); 
    }

    SECTION("mutateAddLink") {
        Config cfg;
        cfg.AddLinkMaxTries = 1;
        Gene g1(0, 3, 0.5, 0), g2(1, 3, 0.7, 1), g3(3, 2, 0.1, 2);

        auto& nodes = gnm.getNodes();
        nodes.setup(2, 1);
        nodes.addNodes({3});
        gnm.addGene(g1); gnm.addGene(g2); gnm.addGene(g3);

        auto& mg = MasterGenome::getInstance();
        mg.initFromGenome(gnm);

        gnm.mutateAddLink(&cfg);

        Gene mut = gnm.getGenes()[3];
        REQUIRE(mut.fromIdx == 1);
        REQUIRE(mut.toIdx == 2);

        gnm.mutateAddLink(&cfg);
        mut = gnm.getGenes()[4];
        REQUIRE(mut.fromIdx == 0);
        REQUIRE(mut.toIdx == 0);
    }

    SECTION("mutateAddNode") {
        Gene g1(0, 3, 0.5, 0), g2(1, 3, 0.7, 1), g3(3, 2, 0.1, 2);
        Genome gnm1;

        gnm1.addGene(g1);
        gnm1.addGene(g2);
        gnm1.addGene(g3);

        gnm1.getNodes().setup(2, 1);
        gnm1.addNode(3);

        auto& mg = MasterGenome::getInstance();
        mg.initFromGenome(gnm1);

        ushort nodet0 = gnm1.getNodesCount();

        gnm1.mutateAddNode();

        auto& genes = gnm1.getGenes();
        Gene& g4 = genes[genes.size()-2],
        &g5 = genes[genes.size()-1];
        ushort nodet1 = gnm1.getNodesCount();

        REQUIRE(nodet0 != nodet1);
        REQUIRE(g4.fromIdx == 3);
        REQUIRE(g4.toIdx == 4);
        REQUIRE(g5.fromIdx == 4);
        REQUIRE(g5.toIdx == 2);
    }

    SECTION("crossover") {
        auto& mg = MasterGenome::getInstance();
        std::array<Gene, 9> gar = {
            Gene(0, 2, 0.5, 0), 
            Gene(1, 2, 0.7, 1), 
            Gene(2, 3, 0.1, 2),
            Gene(2, 4, 0.1, 3),
            Gene(4, 3, 0.1, 4),
            Gene(1, 3, 0.1, 5),
            //same like first 3 but with weight += 0.1
            //to check random peek of matching genes
            Gene(0, 2, 0.6, 0), 
            Gene(1, 2, 0.8, 1), 
            Gene(2, 3, 0.2, 2)
        };

        Genome gnm1, gnm2;

        Genes& gns = mg.getGenes();
        //gns.insert(gns.end(), gar.begin(), gar.end());

        for(auto& el: gar) {
            mg.addGene(el);
        }

        mg.getNodes().setup(2, 1);
        gnm1.getNodes().setup(2, 1);
        gnm2.getNodes().setup(2, 1);

        gnm1.addGene(gar[6]);
        gnm1.addGene(gar[7]);
        gnm1.addGene(gar[8]);
        gnm1.addGene(gar[5]);
        
        gnm2.addGene(gar[0]);
        gnm2.addGene(gar[1]);
        gnm2.addGene(gar[2]);
        gnm2.addGene(gar[3]);
        gnm2.addGene(gar[4]);

        Genome child = gnm1.crossover(gnm2);

        child.print();

        REQUIRE(1 == 0);
    }
}

TEST_CASE("MasterGenome", "[mastergenome]") {
    MasterGenome& mg = MasterGenome::getInstance();
    Genome gnm;
    Gene g1(1,2,0.5,0), g2(2,3,0.7,1), g3(3,4,0.1,2);
    gnm.addGene(g1);
    gnm.addGene(g2);
    gnm.addGene(g3);

    REQUIRE(gnm.getNodes().getCount() == 0);

    auto& n = gnm.getNodes();

    SECTION("Add nodes") {
        n.addNodes({1,2,3});

        REQUIRE(gnm.getNodes().getCount() == 3);

        SECTION("init from Genome") {
            mg.initFromGenome(gnm);
            auto& nds = mg.getNodes();
            
            REQUIRE(gnm.getNodes().getCount() == 3);
            REQUIRE(gnm.getGenes().size() == mg.getGenes().size());
            REQUIRE((gnm.getNodes().getCount()) == (mg.getNodes().getCount()));

            SECTION("Check link exist") {
                REQUIRE(mg.checkLinkExist(3,4) != nullptr);
                REQUIRE(mg.checkLinkExist(4,5) == nullptr);
            }
        }
    }
}
