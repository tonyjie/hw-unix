#include "lenet.h"
#include <string>
using namespace std;

int argmax(float* output, int length) {
    int max_index = 0;
    float max_data = output[0];
    for (int i = 1; i < length; i++) {
        if (output[i] > max_data) {
            max_index = i;
            max_data = output[i];
        }
    }
    return max_index;
}

LeNet::LeNet(string json_file, int x_size, int y_size) : \
    conv1(1, 6, 5, 5, x_size, y_size, "conv1"), \
    relu1(conv1.out_channel, conv1.out_x, conv1.out_y), \
    maxpool1(relu1.channel, 2, 2, relu1.x_size, relu1.y_size), \
    conv2(maxpool1.channel, 16, 3, 3, maxpool1.out_x, maxpool1.out_y, "conv2"), \
    relu2(conv2.out_channel, conv2.out_x, conv2.out_y), \
    maxpool2(relu2.channel, 2, 2, relu2.x_size, relu2.y_size), \
    flat(maxpool2.channel, maxpool2.out_x, maxpool2.out_y), \
    fc1(flat.out_channel, 120, "fc1"), \
    relu3(fc1.out_channel), \
    fc2(relu3.channel, 84, "fc2"), \
    relu4(fc2.out_channel), \
    fc3(relu4.channel, 10, "fc3") \
{
    json_file_name = json_file;
    conv1.load_param(json_file_name);
    conv2.load_param(json_file_name);
    fc1.load_param(json_file_name);
    fc2.load_param(json_file_name);
    fc3.load_param(json_file_name);
}

void LeNet::forward(float ***image) {
    conv1.forward(image);
    relu1.forward(conv1.output);
    maxpool1.forward(relu1.output);
    conv2.forward(maxpool1.output);
    relu2.forward(conv2.output);
    maxpool2.forward(relu2.output);
    flat.forward(maxpool2.output);
    fc1.forward(flat.output);
    relu3.forward(fc1.output);
    fc2.forward(relu3.output);
    relu4.forward(fc2.output);
    fc3.forward(relu4.output);
    predict = argmax(fc3.output, fc3.out_channel);
}