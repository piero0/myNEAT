#include "population.hpp"

using namespace pneat;

void Test(std::size_t iterNum) {
    Util& u = Util::getInstance();

    auto configFile = "config.json";
    auto cfg = u.parseConfig(configFile);

    Population pop;
    pop.initPopulation(cfg.first.population, cfg.second);
    pop.dump();

    pop.testLoop(iterNum);

    pop.dump();
}

int main(int argc, char* argv[]) {
    std::size_t iterNum = 10;
    if(argc > 1) iterNum = std::atoi(argv[1]);
    Test(iterNum);
    return 0;
}
