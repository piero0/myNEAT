#include "util.hpp"

using namespace pneat;

Util::Util() {
    gen = std::mt19937(Util::getTime());
}

Util& Util::getRandomGen() {
    static Util u;
    return u;
}

long Util::getTime() {
    return std::chrono::time_point_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now()
    ).time_since_epoch().count();
}

ushortDist& Util::setIntDist(ushort begin, ushort end) {
    udist = ushortDist(begin, end);
    return udist;
}

floatDist& Util::setFloatDist(float begin, float end) {
    fdist = floatDist(begin, end);
    return fdist;
}

ushort Util::nextInt() {
    return udist(gen);
}

float Util::nextFloat() {
    return fdist(gen);
}
