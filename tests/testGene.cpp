#include "catch.hpp"
#include "../gene.hpp"

using namespace pneat;

TEST_CASE("Genes", "[gene]") {
    Gene g(1, 2, 0.5, 0);

    REQUIRE(g.fromIdx == 1);
    REQUIRE(g.toIdx == 2);
    REQUIRE(g.weight == 0.5);
    
    REQUIRE(g.enabled == true);
    REQUIRE(g.innovationIdx == 0);

    //prbl to be removed
    SECTION("Set innovation") {
        g.setInnov(15);
        REQUIRE(g.innovationIdx == 15);
    }
}

TEST_CASE("Nodes", "[nodes]") {
    Nodes<ushort> nodes;

    SECTION("Initialization") {
        REQUIRE(nodes.getSensorNum() == 0);
        REQUIRE(nodes.getOutputNum() == 0);
        REQUIRE(nodes.getHiddenNum() == 0);
    }

    SECTION("Adding nodes") {
        nodes.addSensor();
        REQUIRE(nodes.getSensorNum() == 1);

        nodes.addOutput();
        REQUIRE(nodes.getOutputNum() == 1);

        nodes.addNode();
        REQUIRE(nodes.getHiddenNum() == 1);

        REQUIRE(nodes.getCount() == 3);
    }
}
