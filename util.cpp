#include "util.hpp"
#include "genome.hpp"

using namespace pneat;

logT Log::logger = nullptr;
std::mt19937 Random::gen = std::mt19937();

void Random::init(std::size_t seed) {
    if(seed == 0) seed = Random::getTime();
    Log::get()->info("Random Seed: {0}", seed);
    Random::gen = std::mt19937(seed);
}

long Random::getTime() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

bool ConfigLoader::openJson(std::string filename) {
    bool loaded = false;
    try {
        read_json(filename, this->pt);
        loaded = true;
    } catch(const json_parser_error& er) { 
       Log::get()->error("JSON reader {0}" , er.what());      
    }
    return loaded;
}

Config ConfigLoader::json2Config() {
    Config cfg;
    Log::get()->debug("Reading Config:");

    cfg.population = pt.get("config.population", 0);
    Log::get()->debug("Population: {0}", cfg.population);

    cfg.maxEpoch = pt.get("config.maxEpoch", 0);
    Log::get()->debug("maxEpoch: {0}", cfg.maxEpoch);

    cfg.AddLinkChance = pt.get("config.AddLinkChance", 0.0);
    Log::get()->debug("AddLinkChance: {0}", cfg.AddLinkChance);

    cfg.AddNodeChance = pt.get("config.AddNodeChance", 0.0);
    Log::get()->debug("AddNodeChance: {0}", cfg.AddNodeChance);

    cfg.MutateWeightsChance = pt.get("config.MutateWeightsChance", 0.0);
    Log::get()->debug("MutateWeightsChance: {0}", cfg.MutateWeightsChance);

    cfg.AddLinkMaxTries = pt.get("config.AddLinkMaxTries", 0);
    Log::get()->debug("AddLinkMaxTries: {0}", cfg.AddLinkMaxTries);

    cfg.loglevel = pt.get("system.loglevel", "info");
    Log::get()->debug("loglevel: {0}", cfg.loglevel);

    cfg.dumpGraphs = pt.get("system.dumpGraphs", false);
    Log::get()->debug("dumpGraphs: {0}", cfg.dumpGraphs);

    cfg.randomseed = pt.get("system.randomseed", 0);
    Log::get()->debug("randomseed: {0}", cfg.randomseed);

    cfg.isValid = true;

    return cfg;
}

Genome ConfigLoader::json2Genome() {
    Genome g;
    auto& n = g.getNodes();

    Log::get()->debug("Genome: ");

    ushort sens = pt.get("genome.sensors", 1);
    ushort outp = pt.get("genome.outputs", 1);
    //ushort hid = pt.get("genome.hidden", 0);

    Log::get()->debug("sensors: {0} outputs: {1}", sens, outp);
    n.setup(sens, outp);

    ptree::key_type key = "genome.genes.";
    ushort idx = 0;
    for(auto& el: pt.get_child(key)) {
        auto it = el.second.begin();
        ushort from = (it++)->second.get_value<ushort>();
        if(from >= sens+outp) n.addNode(from);
        ushort to = (it++)->second.get_value<ushort>();
        if(to >= sens+outp) n.addNode(to);
        float wg = it->second.get_value<float>();
        g.addGene(Gene(from, to, wg, idx));
        Log::get()->debug("Gene: from: {0} to: {1} wg: {2}", from, to, wg);
        idx++;
    }

    return g;
}

std::pair<Config, Genome> ConfigLoader::parseConfig(std::string filename) {
    bool jsonLoaded = this->openJson(filename);
    Config cfg;
    Genome g;

    if(jsonLoaded) {
        cfg = this->json2Config();
        g = this->json2Genome();
    } else {
        Log::get()->error("JSON config not loaded");
    }

    Log::get()->info("Config loaded");
    return std::make_pair<>(cfg, g);
}
