#include "catch2/catch.hpp"
#include "../util.hpp"
#include "../genome.hpp"

using namespace pneat;

TEST_CASE("Util", "[util]") {
    //use const seed to always get the same results (it's mt19937)
    Random::init(1234);
    
    SECTION("random int") {
        auto gen = Random::get<ushort>(0, 1);
        ushort num = gen.next();

        REQUIRE((num == 0 || num == 1));
    }

    SECTION("random gen is not restarted") {
        auto gen1 = Random::get<ushort>(0, 100);
        auto x1 = gen1.next();

        auto gen2 = Random::get<ushort>(0, 100);
        auto x2 = gen2.next();

        REQUIRE(x1 != x2);
    }

    SECTION("json config") {
        std::string filename = "testConfig.json";
        auto cl = ConfigLoader();
        auto resp = cl.parseConfig(filename);

        REQUIRE(resp.first.population == 150);
        REQUIRE(resp.second.getGenes().size() == 5);
    }

    SECTION("new Rnd") {
        RndGen<float> r(0.0, 1.0);
        RndGen<float> r2(2.0, 3.0);
        float w, w2;

        for(int a=0; a<100; a++) {
            w = r.next();
            w2 = r2.next();
            REQUIRE((w >= 0.0 && w <= 1.0));
            REQUIRE((w2 >= 2.0 && w2 <= 3.0));
        }
        
    }
}
