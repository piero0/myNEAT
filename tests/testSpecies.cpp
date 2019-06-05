#include "catch2/catch.hpp"
#include "../species.hpp"

using namespace pneat;

TEST_CASE("Species", "[species]") {
    Config cfg;
    Species spc(std::make_shared<Config>(cfg));
    Genome gnm1;

    Organism o1(gnm1), o2(gnm1), o3(gnm1); 

    o1.setFitness(0.3);
    o2.setFitness(0.5);
    o3.setFitness(0.7);

    spc.addOrganism(o1);
    spc.addOrganism(o2);
    spc.addOrganism(o3);

    auto& orgs = spc.getOrganisms();

    REQUIRE(orgs[0].getFitness() == o1.getFitness());
    REQUIRE(orgs[1].getFitness() == o2.getFitness());
    REQUIRE(orgs[2].getFitness() == o3.getFitness());

    SECTION("prepare fitness") {
        spc.calcOffstringProbability();

        REQUIRE(orgs[0].getThresh() <= 0.20f);
        REQUIRE(orgs[1].getThresh() <= 0.54f);
        REQUIRE(orgs[2].getThresh() <= 1.00f);
    }

    SECTION("randomPick") {
        spc.calcOffstringProbability();

        auto& org = spc.randomPick(0.19);
        REQUIRE(org.getFitness() == 0.3f);

        org = spc.randomPick(0.45);
        REQUIRE(org.getFitness() == 0.5f);

        org = spc.randomPick(0.75);
        REQUIRE(org.getFitness() == 0.7f);
    }
}
