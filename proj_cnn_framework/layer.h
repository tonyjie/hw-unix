#ifndef _LAYER_HH_
#define _LAYER_HH_

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Conv_Layer
{
    public:
        float ***output; // (input_x, input_y, out_channel)
        float ****weight; // (k_x, k_y, in_channel, out_channel)
        float *bias; // (out_channel)
        
        int in_channel, out_channel, kx_size, ky_size, in_x, in_y, out_x, out_y; 
        // input shape: (in_channel, in_x, in_y)
        // output shape: (out_channel, out_x, out_y) = (out_channel, in_x + 1 - kx_size, in_y + 1 - ky_size)
        // (stride = (1,1) by default)
        string name; // "conv1" / "conv2"
        string weight_name; // "conv1.weight" / "conv2.weight"
        string bias_name; // "conv1.bias" / "conv2.bias"

        Conv_Layer(int in_channel, int out_channel, int kx_size, int ky_size, int in_x, int in_y, string name);
        ~Conv_Layer();
        void load_param(string json_file_name);
        void forward(float ***input);
        void display_shape();

    private:

};

class FC_Layer
{
    public:
        float *output;
        float **weight; // (in_channel, out_channel)
        float *bias; // (out_channel)
        

        int in_channel, out_channel;
        string name; // "fc1" / "fc2" / "fc3"
        string weight_name; // "fc1.weight" / "fc2.weight" / "fc3.weight"
        string bias_name; // "fc1.bias" / "fc2.bias" / "fc3.bias"


        FC_Layer(int in_channel, int out_channel, string name); // allocate space for output, weight, bias
        ~FC_Layer(); // delete space for output, weight, bias
        void load_param(string json_file_name); // json_file_name = "para.json"
        void forward(float *input);
        
        void display_shape();


    private:

};

class MaxPool
{
    public:
        float ***output;
        int channel, kx_size, ky_size, in_x, in_y, out_x, out_y;

        MaxPool(int channel, int kx_size, int ky_size, int in_x, int in_y);
        ~MaxPool();
        
        void forward(float ***input);
        void display_shape();

    private:

};


class ReLU_1d
{
    public:
        float *output;
        int channel;
        
        ReLU_1d(int channel);
        ~ReLU_1d();
        void forward(float *input);
        void display_shape();

    private:

};

class ReLU_3d
{
    public:
        float ***output;
        int channel, x_size, y_size;
        
        ReLU_3d(int channel, int x_size, int y_size);
        ~ReLU_3d();
        void forward(float ***input);
        void display_shape();

    private:

};

class Flatten_3d
{
    public:
        float *output;
        int in_channel, in_x, in_y, out_channel;

        Flatten_3d(int in_channel, int in_x, int in_y);
        ~Flatten_3d();
        void forward(float ***input);
        void display_shape();

    private:

};

#endif // _LAYER_HH_