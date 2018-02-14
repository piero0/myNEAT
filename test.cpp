#include "genome.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cout << "Hello" << std::endl;
    std::cout << sizeof(pneat::NodeType) << std::endl;
    std::cout << sizeof(pneat::Gene) << std::endl;
    std::cout << sizeof(uint) << std::endl;

    std::vector<pneat::NodeType> nv(1024);

    nv.push_back(pneat::NodeType());
    std::cout << nv.capacity() << std::endl;
    std::cout << nv.size() << std::endl;

    pneat::Genome g;
    g.testSize();

    using boost::property_tree::ptree;
    using boost::property_tree::json_parser_error;
    ptree pt;
    bool notloadet = false;

    std::string filename;

    if(argc > 1) {
        std::cout << argv[0] << std::endl;
        std::cout << argv[1] << std::endl;

        filename = argv[1];

        try {
            read_json(filename, pt);
        } catch(const json_parser_error& er) {
            std::cout << er.what() << std::endl;
            notloadet = true;
        }
        
    }

    if(!notloadet) {
        int val = pt.get("config.sensors", 0);
        std::cout << val << std::endl;
    }

    return 0;
}
