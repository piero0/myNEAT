#include "catch.hpp"
#include "../util.hpp"

using namespace pneat;

TEST_CASE("Util", "[util]") {
    auto& g1 = Util::getInstance();
    auto& g2 = Util::getInstance();
    
    REQUIRE(&g1 == &g2);
    
    SECTION("random int") {
        g1.setIntDist(0, 1);
        ushort num = g1.nextInt();

        REQUIRE((num == 0 || num == 1));
    }

    SECTION("json config") {
        std::string filename = "testConfig.json";
        auto resp = g1.parseConfig(filename);

        REQUIRE(resp.first.population == 150);
        REQUIRE(resp.second.getGenes().size() == 5);
    }
}
