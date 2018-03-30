#include "catch.hpp"
#include "../util.hpp"
#include "../genome.hpp"

using namespace pneat;

TEST_CASE("Util", "[util]") {
    //use const seed to always get the same results (it's mt19937)
    auto& g1 = Util::getInstance();
    g1.initRandomGen(1234);

    auto& g2 = Util::getInstance();
    
    REQUIRE(&g1 == &g2);
    
    SECTION("random int") {
        auto gen = g1.getSRndGen(0, 1);
        ushort num = gen.next();

        REQUIRE((num == 0 || num == 1));
    }

    SECTION("random gen is not restarted") {
        auto& gen1 = Util::getInstance();
        auto gen11 = g1.getSRndGen(0, 100);
        auto x1 = gen11.next();

        auto& gen2 = Util::getInstance();
        auto gen21 = g1.getSRndGen(0, 100);
        auto x2 = gen21.next();

        REQUIRE(x1 != x2);
    }

    SECTION("json config") {
        std::string filename = "testConfig.json";
        auto resp = g1.parseConfig(filename);

        REQUIRE(resp.first.population == 150);
        REQUIRE(resp.second.getGenes().size() == 5);
    }

    SECTION("new Rnd") {
        std::mt19937 gen(1234);
        Rnd<float, floatDist> r(gen, 0.0, 1.0);
        Rnd<float, floatDist> r2(gen, 2.0, 3.0);
        float w, w2;

        for(int a=0; a<100; a++) {
            w = r.next();
            w2 = r2.next();
            REQUIRE((w >= 0.0 && w <= 1.0));
            REQUIRE((w2 >= 2.0 && w2 <= 3.0));
        }
        
    }
}
