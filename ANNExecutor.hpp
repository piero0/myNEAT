#pragma once

#include <memory>
#include <vector>

using data = std::vector<float>;

extern data getData();
extern data sendData(std::shared_ptr<data> output);
extern float getFitness();

class ANN {
public:
    data runOnce(data input);
};

class ANNExecutor {
    std::shared_ptr<ANN> ann;
public:
    void setAnn(std::shared_ptr<ANN> ann);
    float evalLoop() {
        //create once, assing by index
        data input;
        data output;

        while(true){
            input = getData();
            if(input.size() > 0) {
                output = ann->runOnce(input);
                sendData(std::make_shared<data>(output));
            }
            return getFitness();    
        }
        
    }

    //void evalOnce(); not needed, getData controls when the loop stops
};
