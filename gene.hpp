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
        Nodes<T>& operator=(Nodes<T> const& nds) {
            sensorNum = nds.getSensorNum(); 
            outputNum = nds.getOutputNum(); 
            hiddenNum = nds.getHiddenNum();

            return *this;
        }

        void setup(T sens, T out, T hidd) {
            sensorNum = sens;
            outputNum = out;
            hiddenNum = hidd;
        }

        void addSensor() { sensorNum++; }
        void addOutput() { outputNum++; }
        void addNode() { hiddenNum++; }

        const T& getSensorNum() const { return sensorNum; }
        const T& getOutputNum() const { return outputNum; }
        const T& getHiddenNum() const { return hiddenNum; }

        const T getCount() const { return sensorNum+outputNum+hiddenNum; }
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
        void print() const;
};
}
