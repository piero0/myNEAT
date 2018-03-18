#include "util.hpp"

using namespace pneat;

Util::Util() {
    gen = std::mt19937(Util::getTime());
}

Util::Util(std::size_t seed) {
    if(seed == 0) seed = Util::getTime();
    gen = std::mt19937(seed);
}

Util& Util::getInstance(std::size_t seed) {
    static Util u(seed);
    return u;
}

long Util::getTime() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

const ushortDist& Util::setIntDist(ushort begin, ushort end) {
    udist = ushortDist(begin, end);
    return udist;
}

const floatDist& Util::setFloatDist(float begin, float end) {
    fdist = floatDist(begin, end);
    return fdist;
}

bool Util::openJson(std::string filename) {
    bool loaded = false;
    try {
        read_json(filename, this->pt);
        loaded = true;
    } catch(const json_parser_error& er) {
        std::cout << er.what() << std::endl;            
    }
    return loaded;
}

Config Util::json2Config() {
    Config cfg;
    std::cout << "Config:" << std::endl;
    ushort population = pt.get("config.population", 0);
    std::cout << "\tPopulation: " << population << std::endl;
    cfg.population = population;
    return cfg;
}

Genome Util::json2Genome() {
    Genome g;
    auto& n = g.getNodes();

    std::cout << "Genome: " << std::endl;

    ushort sens = pt.get("genome.sensors", 1);
    ushort outp = pt.get("genome.outputs", 1);
    ushort hid = pt.get("genome.hidden", 0);

    for(int a=0; a<sens; a++) n.addSensor();
    for(int a=0; a<outp; a++) n.addOutput();
    for(int a=0; a<hid; a++) n.addNode();

    std::cout << "\tsensors: " << sens << std::endl;
    std::cout << "\toutputs: " << outp << std::endl;
    std::cout << "\thidden: " << hid << std::endl;

    ptree::key_type key = "genome.genes.";
    ushort idx = 0;
    for(auto& el: pt.get_child(key)) {
        auto it = el.second.begin();
        ushort from = (it++)->second.get_value<ushort>();
        ushort to = (it++)->second.get_value<ushort>();
        float wg = it->second.get_value<float>();
        g.addGene(Gene(from, to, wg, idx));
        std::cout << from << " - " << to << " - " << wg << std::endl;
        idx++;
    }

    return g;
}

std::pair<Config, Genome> Util::parseConfig(std::string filename) {
    bool jsonLoaded = this->openJson(filename);
    Config cfg;
    Genome g;

    if(jsonLoaded) {
        cfg = this->json2Config();
        g = this->json2Genome();
    } else {
        std::cerr << "JSON not loaded" << std::endl;
    }

    return std::make_pair<>(cfg, g);
}
