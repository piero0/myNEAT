#include "genome.hpp"
#include "organism.hpp"
#include "species.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace pneat;

void nextTest() {
    Species sp;
    MasterGenome& mg = MasterGenome::getInstance();
}

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cout << "Hello" << std::endl;
    std::cout << "NodeType: " << sizeof(pneat::NodeType) << std::endl;
    std::cout << "Gene: " << sizeof(pneat::Gene) << std::endl;
    std::cout << "Int: " << sizeof(uint) << std::endl;

    std::vector<pneat::NodeType> nv(1024);

    nv.push_back(pneat::NodeType());
    std::cout << nv.capacity() << std::endl;
    std::cout << nv.size() << std::endl;

    pneat::Genome g;

    using boost::property_tree::ptree;
    using boost::property_tree::json_parser_error;
    ptree pt;
    bool jsonLoaded = false;

    std::string filename;

    if(argc > 1) {
        std::cout << argv[0] << std::endl;
        std::cout << argv[1] << std::endl;

        filename = argv[1];

        try {
            read_json(filename, pt);
            jsonLoaded = true;
        } catch(const json_parser_error& er) {
            std::cout << er.what() << std::endl;            
        }
        
    }

    if(jsonLoaded) {
        ushort sens = pt.get("sensors", 1);
        ushort outp = pt.get("outputs", 1);
        ushort hid = pt.get("hidden", 0);

        std::cout << "sensors: " << sens << std::endl;
        std::cout << "outputs: " << outp << std::endl;
        std::cout << "hidden: " << hid << std::endl;

        std::cout << "get child" << std::endl;

        ptree::key_type key = "gens.";
        // ushort idx = 1;
        for(auto& el: pt.get_child(key)) {
            /*for(auto& el: pt.get_child(key+"."+el2.first)) {
                if(idx%3 == 0) {
                    std::cout << el.first << " - " << el.second.get_value<float>() << std::endl;
                } else {
                    std::cout << el.first << " - " << el.second.get_value<int>() << std::endl;
                }
                idx++;
            }*/
                auto it = pt.get_child(key+el.first).begin();
                ushort from = (it++)->second.get_value<ushort>();
                ushort to = (it++)->second.get_value<ushort>();
                float wg = it->second.get_value<float>();
                std::cout << from << " - " << to << " - " << wg << std::endl;
            }

        //std::cout << "gv: " << gv << std::endl;
    }

    /* test copy vector */
    using pneat::Genome;
    using pneat::Gene;
    using pneat::NodeType;
    using pneat::Organism;

    Genome gnm;
    auto& nodes = gnm.getNodes();
    nodes.addSensor();
    nodes.addSensor();
    nodes.addNode();
    nodes.addOutput();
    
    gnm.addGene(Gene(0, 3, 0.25, 0));
    gnm.addGene(Gene(1, 3, 0.27, 1));
    gnm.addGene(Gene(0, 2, 0.35, 2));
    gnm.addGene(Gene(1, 2, 0.38, 3));
    gnm.addGene(Gene(2, 3, 0.55, 4));

    gnm.print();

    std::cout << "New organism" << std::endl;

    Organism org(gnm);
    // org.cloneAndMutate(gnm);
    // org.print();

    //Make a population

    return 0;
}
