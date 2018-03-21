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

template<class outT, class distT> class Rnd {
    std::mt19937& gen;
    distT dist;

    public:
        Rnd(std::mt19937& genref, outT start, outT end):
            gen(genref) {
            dist = distT(start, end);
            std::cout << "My: " << &gen << " Passed: " << &genref << std::endl;
        }

        outT next() { return dist(gen); }
};

class Util {
    private:
        std::mt19937 gen;
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

        Rnd<ushort, ushortDist> getSRndGen(ushort begin, ushort end) { return Rnd<ushort, ushortDist>(gen, begin, end); }
        Rnd<float, floatDist> getFRndGen(float begin, float end) { return Rnd<float, floatDist>(gen, begin, end); }

        std::pair<Config, Genome> parseConfig(std::string filename);
};
}
