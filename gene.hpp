#pragma once
#include <vector>
#include <iostream>
#include <random>

namespace pneat {

typedef unsigned int uint;
typedef unsigned short ushort;

enum NodeType: char {
    SENSOR,
    HIDDEN,
    OUTPUT
}; 

class Gene {
        //4 2 2 2 1
        float weight;
        ushort fromIdx;
        ushort toIdx;
        ushort innovationIdx; 
        bool enabled;

        public:
            Gene(ushort from, ushort to, float weight);
            void print();
            void setInnov(ushort innov);
            void mutateWeight();
};
}
