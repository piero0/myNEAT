#include "population.hpp"

using namespace pneat;

void RunNEAT(std::string configFile) {
    if(!Log::initLog()) return;

    Random::init(1234);

    auto cfg = ConfigLoader().parseConfig(configFile);

    if(!cfg.first.isValid) {
        std::cerr << "ERROR: Missing or invalid JSON configuration" << std::endl;
        return;
    }
    Log::set_level(cfg.first.loglevel);

    Population pop;
    pop.initPopulation(&cfg.first, cfg.second);
    pop.dump();
    pop.testLoop();
    pop.dump();
}

int main(int argc, char* argv[]) {
    if(argc == 2) RunNEAT(argv[1]);
    else std::cout << "Run: " << argv[0] << " config.json" << std::endl;
    return 0;
}
