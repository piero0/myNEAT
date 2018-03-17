#include "catch.hpp"
#include "../util.hpp"

using namespace pneat;

TEST_CASE("Util", "[util]") {
    //use const seed to always get the same results (it's mt19937)
    auto& g1 = Util::getInstance(1234);

    auto& g2 = Util::getInstance();
    
    REQUIRE(&g1 == &g2);
    
    SECTION("random int") {
        g1.setIntDist(0, 1);
        ushort num = g1.nextInt();

        REQUIRE((num == 0 || num == 1));
    }

    SECTION("random gen is not restarted") {
        auto& gen1 = Util::getInstance();
        gen1.setIntDist(0, 100);
        auto x1 = gen1.nextInt();

        auto& gen2 = Util::getInstance();
        gen2.setIntDist(0, 100);
        auto x2 = gen2.nextInt();

        REQUIRE(x1 != x2);
    }

    SECTION("json config") {
        std::string filename = "testConfig.json";
        auto resp = g1.parseConfig(filename);

        REQUIRE(resp.first.population == 150);
        REQUIRE(resp.second.getGenes().size() == 5);
    }
}
