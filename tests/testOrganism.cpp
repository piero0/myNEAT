#include "catch.hpp"
#include "../organism.hpp"

using namespace pneat;

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
