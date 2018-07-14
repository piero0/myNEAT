#pragma once
#include <chrono>
#include <random>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "spdlog/spdlog.h"

namespace pneat {

class Genome;

using boost::property_tree::ptree;
using boost::property_tree::json_parser_error;

using logT = std::shared_ptr<spdlog::logger>;

struct Config {
    ushort population;
    ushort maxEpoch;
    float AddLinkChance;
    float AddNodeChance;
    float MutateWeightsChance;
    ushort AddLinkMaxTries;
    std::string loglevel;
    std::size_t randomseed;
    bool dumpGraphs;
    bool isValid;

    Config(): isValid(false) {}
};

template<class> class RndGen;

class Random {
    public:
        static std::mt19937 gen;

        static long getTime();
        static void init(std::size_t seed);

        template<class T> static RndGen<T> get(T begin, T end) {
            return RndGen<T>(begin, end);
        }
};

template<class T> class RndGen {
    typedef typename std::conditional<std::is_integral<T>::value, T, int>::type safeInt;
    typedef typename std::conditional<std::is_floating_point<T>::value, T, float>::type safeReal;

    union {
        std::uniform_int_distribution<safeInt> intDist;
        std::uniform_real_distribution<safeReal> realDist;
    };
    bool isInt;

    public:
        RndGen(T begin, T end): isInt(false) {
            if(std::is_integral<T>::value) {
                isInt = true;
                intDist = std::uniform_int_distribution<safeInt>(begin, end);
            } else if(std::is_floating_point<T>::value) {
                realDist = std::uniform_real_distribution<safeReal>(begin, end);
            } else {
                throw std::invalid_argument("Random generator type has to be converible to int or real");
            }
        }

        T next() { return (isInt) ? intDist(Random::gen) : realDist(Random::gen); }
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

}
