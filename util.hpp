#pragma once
#include <chrono>
#include <random>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "spdlog/spdlog.h"

namespace pneat {

class Gene;
class Genome;

using ushortDist = std::uniform_int_distribution<ushort>;
using floatDist = std::uniform_real_distribution<float>;

using boost::property_tree::ptree;
using boost::property_tree::json_parser_error;

using logT = std::shared_ptr<spdlog::logger>;

struct Config {
    ushort population;
    ushort epochNum;
    float AddLinkChance;
    float AddNodeChance;
    float MutateWeightsChance;
    ushort AddLinkMaxTries;
    std::string loglevel;
    std::size_t randomseed;
    bool isValid;

    Config(): isValid(false) {}
};

template<class outT, class distT> class Rnd {
    std::mt19937& gen;
    distT dist;

    public:
        Rnd(std::mt19937& genref, outT start, outT end):
            gen(genref) {
            dist = distT(start, end);
        }

        outT next() { return dist(gen); }
};

class Log {
    static logT logger; 

    public:
        static logT get() { return logger; }
        static bool initLog() {
            bool stat = false;
            if(!logger) {
                try {
                    logger = spdlog::basic_logger_st("log1", "neat.log");
                    stat = true;
                } catch(spdlog::spdlog_ex& ex) {
                    std::cerr << "Could not open log file" << std::endl;
                }
            }
            logger->set_level(spdlog::level::info);
            logger->set_pattern("[%X.%e] [%l] %v");
            return stat;
        }

        static void set_level(std::string level) {
            if(level == "trace") logger->set_level(spdlog::level::trace);
            else if(level == "debug") logger->set_level(spdlog::level::debug);
            else if(level == "info") logger->set_level(spdlog::level::info);
            else if(level == "warn") logger->set_level(spdlog::level::warn);
            else if(level == "error") logger->set_level(spdlog::level::err);
            else if(level == "off") logger->set_level(spdlog::level::off);
        }
};

class ConfigLoader {
    ptree pt;
    bool openJson(std::string filename);
    Config json2Config();
    Genome json2Genome();

    public:
        std::pair<Config, Genome> parseConfig(std::string filename);
};

class Util {
    std::mt19937 gen;
    Util();

    public:
        Util(Util const& u) = delete;
        void operator=(Util const& u) = delete;

        static Util& getInstance();
        static long getTime();
        void initRandomGen(std::size_t seed);

        std::pair<Config, Genome> parseConfig(std::string filename);

        Rnd<ushort, ushortDist> getSRndGen(ushort begin, ushort end) { return Rnd<ushort, ushortDist>(gen, begin, end); }
        Rnd<float, floatDist> getFRndGen(float begin, float end) { return Rnd<float, floatDist>(gen, begin, end); }   
};
}
