#ifndef LENET_H  
#define LENET_H
#include "layer.h"
#include <string>
using namespace std;
class LeNet
{
    public:
        LeNet(string json_file_name, int x_size, int y_size);
        void forward(float ***image);
        int predict;
        string json_file_name;

    private:
        Conv_Layer conv1;
        ReLU_3d relu1;
        MaxPool maxpool1;
        Conv_Layer conv2;
        ReLU_3d relu2;
        MaxPool maxpool2;
        Flatten_3d flat;
        FC_Layer fc1;
        ReLU_1d relu3;
        FC_Layer fc2;
        ReLU_1d relu4;
        FC_Layer fc3;
};
#endif // LENET_H