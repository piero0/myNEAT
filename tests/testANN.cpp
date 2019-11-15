#include "catch.hpp"
#include "../ANNFactory.hpp"

using namespace pneat;

TEST_CASE("ANN", "[ann]") {
    ANN anf;

    SECTION("Index Map") {
        std::vector<ushort> v = {1,5,8};
        auto out = anf.createIndexMap(v);
        REQUIRE(out[1] == 0);
        REQUIRE(out[5] == 1);
        REQUIRE(out[8] == 2);
    }

    SECTION("Index Map out of order") {
        //but with the highest index at the end
        std::vector<ushort> v = {1,5,3,8};
        auto out = anf.createIndexMap(v);
        REQUIRE(out[1] == 0);
        REQUIRE(out[3] == 2);
        REQUIRE(out[5] == 1);
        REQUIRE(out[8] == 3);
    }
}
