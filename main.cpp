#include "population.hpp"

using namespace pneat;

void Test() {
    Util& u = Util::getInstance();
    Log::initLog();

    auto configFile = "config.json";
    auto cfg = u.parseConfig(configFile);

    Log::set_level(cfg.first.loglevel);

    Population pop;
    pop.initPopulation(&cfg.first, cfg.second);
    pop.dump();

    pop.testLoop();

    pop.dump();
}

int main(int argc, char* argv[]) {
    Test();
    return 0;
}
