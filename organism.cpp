#include <fstream>
#include "organism.hpp"

using namespace pneat;

Organism::Organism(const Genome& g) {
    this->g = g;
    fitness = 0.0;
}

void Organism::dump() {
    std::cout << "Organism" << std::endl;
    std::cout << "Fitness: " << fitness << std::endl;
    g.print();
}

void Organism::dumpGraph(std::string name) {
    std::ofstream of(name + ".gv", std::ios_base::out);

    of << "digraph " << name << "{" << std::endl;
    of << "rankdir=BT" << std::endl;
    of << "{rank=source;";

    int sensNum = g.getNodes().getSensorNum();

    for(int a=0; a<sensNum; a++) of << a << ";";

    of << "}" << std::endl;

    of << "{rank=sink;"; 

    for(int a=sensNum; a<g.getNodes().getOutputNum()+sensNum; a++) of << a << ";";

    of << "}" << std::endl;

    for(auto& gen: g.getGenes()) {
        if(!gen.enabled) continue;
        of << gen.fromIdx << "->" << gen.toIdx << "[label="<< std::setprecision(2) << gen.weight << "];" << std::endl;
    }

    of << "}" << std::endl;
    of.close();
}