#pragma once
#include <chrono>
#include <random>

namespace pneat {

using ushortDist = std::uniform_int_distribution<ushort>;
using floatDist = std::uniform_real_distribution<float>;

class Util {
    private:
        std::mt19937 gen;
        ushortDist udist;
        floatDist fdist;
        Util();

    public:
        Util(Util const& u) = delete;
        void operator=(Util const& u) = delete;

        static Util& getRandomGen();
        static long getTime();

        ushortDist& setIntDist(ushort begin, ushort end);
        floatDist& setFloatDist(float begin, float end);

        ushort nextInt() { return udist(gen); }
        float nextFloat() { return fdist(gen); }
};
}
