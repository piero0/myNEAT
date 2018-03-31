#include "population.hpp"

using namespace pneat;

void RunNEAT(std::string configFile) {
    if(!Log::initLog()) return;

    Log::get()->info("Starting Experiment");
    auto cfg = ConfigLoader().parseConfig(configFile);

    if(!cfg.first.isValid) {
        std::cerr << "ERROR: Missing or invalid JSON configuration" << std::endl;
        return;
    }
    Log::set_level(cfg.first.loglevel);
    Random::init(cfg.first.randomseed);

    Population pop;
    pop.initPopulation(std::make_shared<Config>(cfg.first), cfg.second);
    pop.dump();
    pop.testLoop();
    pop.dump();
    Log::get()->info("Experiment done");
}

int main(int argc, char* argv[]) {
    if(argc == 2) RunNEAT(argv[1]);
    else std::cout << "Run: " << argv[0] << " config.json" << std::endl;
    return 0;
}
