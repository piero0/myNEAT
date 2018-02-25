#include "catch.hpp"
#include "../organism.hpp"
#include "../util.hpp"

/*
    //What I want API

    std::string filename = "bleble.json"
    ushort popSize = 150;

    std::pair<Genome, Config> cnfg = Helper::initFromJSON(filename);
    Genome baseGenome = cnfg.first;
    Config cfg = cnfg.second;

    or pair<ushort, Genome> cnfg = ...
    pop = cnfg.first;
    gen = cnfg.second;

    Population population;
    
    population.init(popSize, baseGenome);

    // while(something)
    population.evolve();
*/
using namespace pneat;

TEST_CASE("Genes", "[gene]") {
    Gene g(1, 2, 0.5, 0);

    REQUIRE(g.fromIdx == 1);
    REQUIRE(g.toIdx == 2);
    REQUIRE(g.weight == 0.5);
    
    REQUIRE(g.enabled == true);
    REQUIRE(g.innovationIdx == 0);

    SECTION("Set innovation") {
        g.setInnov(15);
        REQUIRE(g.innovationIdx == 15);
    }

    SECTION("Mutating Weights") {
        g.mutateWeight();
        REQUIRE(g.weight != 0.5);
    }
}

TEST_CASE("Genome", "[genome]") {
    Genome gnm;

    auto& g = gnm.getGenes();
    REQUIRE(g.size() == 0);

    auto tpl = gnm.getNodeNum();

    REQUIRE(std::get<0>(tpl) == 0);
    REQUIRE(std::get<1>(tpl) == 0);
    REQUIRE(std::get<2>(tpl) == 0);

    SECTION("Add Nodes") {
        gnm.addNode(NodeType::OUTPUT);
        tpl = gnm.getNodeNum();
        REQUIRE(std::get<1>(tpl) == 1);

        gnm.addNode(NodeType::SENSOR);
        tpl = gnm.getNodeNum();
        REQUIRE(std::get<0>(tpl) == 1);

        gnm.addNode(NodeType::HIDDEN);
        tpl = gnm.getNodeNum();
        REQUIRE(std::get<2>(tpl) == 1);
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

    SECTION("Check link exist") {
        Gene g1(1,2,0.5,0), g2(2,3,0.7,1), g3(3,4,0.1,2);
        gnm.addGene(g1);
        gnm.addGene(g2);
        gnm.addGene(g3);
        REQUIRE(gnm.checkLinkExist(3,4) == true);
        REQUIRE(gnm.checkLinkExist(4,5) == false);
    }

    SECTION("mutateAddLink") {
        Gene g1(0, 2, 0.5, 0), g2(1, 2, 0.7, 1), g3(2, 3, 0.1, 2);

        gnm.addNode(NodeType::SENSOR);
        gnm.addNode(NodeType::SENSOR);
        gnm.addNode(NodeType::OUTPUT);
        gnm.addNode(NodeType::HIDDEN);
        gnm.addGene(g1); gnm.addGene(g2); gnm.addGene(g3);
        
        Genome master = gnm;

        gnm.setMaster(&master);
        gnm.mutateAddLink();

        Gene mut = gnm.getGenes()[3];
        REQUIRE(gnm.getNextInnov() == 4);
        REQUIRE(mut.fromIdx != 2);
        REQUIRE(mut.toIdx > 1);
    }
}

TEST_CASE("Organism", "[organism]") {
    Genome gnm;
    Gene g1(1,2,0.5,0), g2(2,3,0.7,1), g3(3,4,0.1,2);
    gnm.addGene(g1);
    gnm.addGene(g2);
    gnm.addGene(g3);

    Organism o1(gnm);

    gnm.addGene(g1);
    Genome g = o1.getGenome();
    REQUIRE(&g != &gnm);
    REQUIRE(g.getGenes().size() != gnm.getGenes().size());
}

TEST_CASE("Util", "[util]") {
    auto& g1 = Util::getRandomGen();
    auto& g2 = Util::getRandomGen();
    REQUIRE(&g1 == &g2);
    
    g1.setIntDist(0, 1);
    ushort num = g1.nextInt();
    REQUIRE((num == 0 || num == 1));
}
