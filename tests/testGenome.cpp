#include "catch.hpp"
#include "../genome.hpp"

using namespace pneat;

TEST_CASE("Genome", "[genome]") {
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

    SECTION("mutateAddLink") {
        Gene g1(0, 2, 0.5, 0), g2(1, 2, 0.7, 1), g3(2, 3, 0.1, 2);

        auto& nodes = gnm.getNodes();
        nodes.addNode();
        nodes.addSensor();
        nodes.addSensor();
        nodes.addOutput();
        gnm.addGene(g1); gnm.addGene(g2); gnm.addGene(g3);
        
        Genome master = gnm;

        gnm.mutateAddLink();

        Gene mut = gnm.getGenes()[3];
        REQUIRE(mut.fromIdx != 2);
        REQUIRE(mut.toIdx > 1);
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
        n.addNode();
        n.addNode();
        n.addNode();

        REQUIRE(gnm.getNodes().getCount() == 3);

        SECTION("init from Genome") {
            mg.initFromGenome(gnm);
            auto& nds = mg.getNodes();
            
            REQUIRE(gnm.getNodes().getCount() == 3);
            REQUIRE(gnm.getGenes().size() == mg.getGenes().size());
            REQUIRE((gnm.getNodes().getCount()) == (mg.getNodes().getCount()));

            SECTION("Check link exist") {
                REQUIRE(mg.checkLinkExist(3,4) == true);
                REQUIRE(mg.checkLinkExist(4,5) == false);
            }
        }
    }
}
