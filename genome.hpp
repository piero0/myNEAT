#pragma once
#include <vector>
#include <iostream>

namespace pneat {

    typedef unsigned int uint;
    typedef unsigned short ushort;

    enum NodeType: char {
        SENSOR,
        HIDDEN,
        OUTPUT
    }; 

/*
    class Node { 
        NodeType type;
    };
*/

    class Gene {
        //4 4 4 2 1
        uint fromIdx;
        uint toIdx;
        float weight;
        ushort innovationIdx; 
        bool enabled;
    };

    class Genome {
        std::vector<NodeType> nodes;
        std::vector<Gene> gens;
        ushort sensorNum;
        ushort outputNum;

        public:
            void testSize();
    };

}
