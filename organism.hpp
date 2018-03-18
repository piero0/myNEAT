#pragma once
#include "genome.hpp"

namespace pneat {
    class Organism {
        Genome g;
        float fitness;
        float thresh;
        float normFitness;

        public:
            Organism(const Genome& g);
            Genome& getGenome() { return g; }

            void setFitness(float fitness) { this->fitness = fitness; }
            float getFitness() { return fitness; }
            float getThresh() { return thresh; }

            float setThresh(float lastThresh, float totalFitness) { 
                normFitness = fitness/totalFitness; 
                thresh = lastThresh + normFitness;
                return normFitness; 
            }

            static float adder(const float& sum, const Organism& org) {
                return sum + org.fitness;
            }

            static bool compareThresh(const Organism& o, const float& th) {
                return o.thresh <= th;
            }
    };
}
