#include "population.hpp"

using namespace pneat;

void Test() {
    Util& u = Util::getInstance();

    auto configFile = "config.json";
    auto cfg = u.parseConfig(configFile);

    Population pop;
    pop.initPopulation(cfg.first.population, cfg.second);
    pop.dump();

    pop.testLoop();

    pop.dump();
}

int main(int argc, char* argv[]) {
    Test();
    return 0;
}
