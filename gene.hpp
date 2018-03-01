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

template<class T> class Nodes {
    T sensorNum;
    T outputNum;
    T hiddenNum;

    public:
        Nodes() { sensorNum=0; outputNum=0; hiddenNum=0; }
        void addSensor() { sensorNum++; }
        void addOutput() { outputNum++; }
        void addNode() { hiddenNum++; }

        T getSensorNum() { return sensorNum; }
        T getOutputNum() { return outputNum; }
        T getHiddenNum() { return hiddenNum; }

        T getCount() { return sensorNum+outputNum+hiddenNum; }
};

class Gene {
    public:
        //4 2 2 2 1
        float weight;
        ushort fromIdx;
        ushort toIdx;
        ushort innovationIdx; 
        bool enabled;

        Gene(ushort from, ushort to, float weight, ushort innov);
        void print();
        void setInnov(ushort innov);
        void mutateWeight();
};
}
