#pragma once
#include <chrono>
#include <random>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "genome.hpp"

namespace pneat {

class Genome;

using ushortDist = std::uniform_int_distribution<ushort>;
using floatDist = std::uniform_real_distribution<float>;

using boost::property_tree::ptree;
using boost::property_tree::json_parser_error;

struct Config {
    ushort population;
};

class Util {
    private:
        std::mt19937 gen;
        ushortDist udist;
        floatDist fdist;
        ptree pt;

        Util();
        Util(std::size_t seed);

        bool openJson(std::string filename);
        Config json2Config();
        Genome json2Genome();

    public:
        Util(Util const& u) = delete;
        void operator=(Util const& u) = delete;

        static Util& getInstance(std::size_t seed = 0);
        static long getTime();

        const ushortDist& setIntDist(ushort begin, ushort end);
        const floatDist& setFloatDist(float begin, float end);

        ushort nextInt() { return udist(gen); }
        float nextFloat() { return fdist(gen); }

        std::pair<Config, Genome> parseConfig(std::string filename);
};
}
