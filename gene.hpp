#pragma once
#include <vector>
#include <iostream>
#include <random>

#include "util.hpp"

namespace pneat {

typedef unsigned int uint;
typedef unsigned short ushort;

enum NodeType: char {
    SENSOR,
    HIDDEN,
    OUTPUT
}; 

template<class T> class Nodes {
    std::vector<T> nodeIdx;
    T sensorNum;
    T outputNum;
    T hiddenNum;

    public:
        Nodes() { sensorNum=0; outputNum=0; hiddenNum=0; }

        Nodes<T>& operator=(Nodes<T> const& nds) {
            nodeIdx = nds.nodeIdx;
            sensorNum = nds.getSensorNum(); 
            outputNum = nds.getOutputNum(); 
            hiddenNum = nds.getHiddenNum();

            return *this;
        }

        void setup(T sens, T out) {
            sensorNum = sens;
            outputNum = out;
            
            for(T a=0; a<sensorNum; a++) nodeIdx.push_back(a);
            for(T a=sensorNum; a<sensorNum+outputNum; a++) nodeIdx.push_back(a);
        }

        void addNode(T idx) { nodeIdx.push_back(idx); hiddenNum++; }

        void addNodes(std::initializer_list<T> l) {
            hiddenNum += l.size();
            nodeIdx.insert(nodeIdx.end(), l.begin(), l.end());
        }

        const T& getSensorNum() const { return sensorNum; }
        const T& getOutputNum() const { return outputNum; }
        const T& getHiddenNum() const { return hiddenNum; }

        const T getCount() const { return nodeIdx.size(); }
        std::vector<T>& getIdxs() { return nodeIdx; }

        const T getIdx(T idx) const { 
            if(idx < 0 || idx >= nodeIdx.size()) {
                Log::get()->error("Node::getIdx, wrong idx: {0}", idx);
                idx = 0;
            }
            return nodeIdx[idx]; 
        }

        const T getNextNodeIdx() const {
            return nodeIdx[getCount()-1]+1;
        }

        void print() const {
            std::string txt = "|";
            for(std::size_t a=0; a<nodeIdx.size(); a++) txt += std::to_string(nodeIdx[a]) + "|";
            Log::get()->debug("Nodes: {0}", txt);
        }
};

//template <class intT, class realT>
class Gene {
    public:
        //4 2 2 2 1
        float weight;
        ushort fromIdx;
        ushort toIdx;
        ushort innovationIdx; 
        ushort childIdx; //used in mutateAddNode
        bool enabled;

        Gene(ushort from, ushort to, float weight, ushort innov);
        std::string print() const;

        friend bool operator<(const Gene& g1, const Gene& g2) { 
            if(g1.fromIdx < g2.fromIdx) { return true; }
            else if(g1.fromIdx == g2.fromIdx && g1.toIdx < g2.toIdx) { return true; }
            return false;
        }
};
}
