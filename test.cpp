#include "genome.hpp"

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

    return 0;
}
