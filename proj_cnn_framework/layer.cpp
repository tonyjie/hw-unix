#include "layer.h"
#include "utils.h"
#include "json/json.h"
#include "jsoncpp.cpp"
#include <cassert>
using namespace std;

#define assertm(exp, msg) assert(((void)msg, exp))
#define MAX(x, y) (((x)>(y))?(x):(y))

// Conv_Layer
Conv_Layer::Conv_Layer(int in_c, int out_c, int kernel_x_size, int kernel_y_size, int in_x_size, int in_y_size, string layer_name) {
    in_channel = in_c;
    out_channel = out_c;
    kx_size = kernel_x_size;
    ky_size = kernel_y_size;
    in_x = in_x_size;
    in_y = in_y_size;

    const string weight_str = ".weight";
    const string bias_str = ".bias";
    name = layer_name;
    weight_name = name + weight_str;
    bias_name = name + bias_str;

    out_x = in_x + 1 - kx_size;
    out_y = in_y + 1 - ky_size;

    output = new_mem_3d<float>(out_c, out_x, out_y);
    weight = new_mem_4d<float>(in_c, out_c, kx_size, ky_size);
    bias = new_mem_1d<float>(out_c);
}

Conv_Layer::~Conv_Layer() {
    delete_mem_3d<float>(output, out_channel, out_x, out_y);
    delete_mem_4d<float>(weight, in_channel, out_channel, kx_size, ky_size);
    delete_mem_1d<float>(bias);
}

void Conv_Layer::load_param(string json_file_name) {
    Json::Reader reader;
    Json::Value obj;
    ifstream ifs(json_file_name);
    reader.parse(ifs, obj);

    // Load Weight
    int out_c_weight_json = obj[weight_name].size();
    assertm((out_channel == out_c_weight_json), "JSON file: output channel error");
    int in_c_weight_json = obj[weight_name][0].size();
    assertm((in_channel == in_c_weight_json), "JSON file: input channel error");
    int kx_weight_json = obj[weight_name][0][0].size();
    assertm((kx_size == kx_weight_json), "JSON file: kx size error");
    int ky_weight_json = obj[weight_name][0][0][0].size();
    assertm((ky_size == ky_weight_json), "JSON file: ky size error");

    for (int i = 0; i < out_channel; i++) {
        for (int j = 0; j < in_channel; j++) {
            for (int kx = 0; kx < kx_size; kx++) {
                for (int ky = 0; ky < ky_size; ky++) {
                    weight[j][i][kx][ky] = obj[weight_name][i][j][kx][ky].asFloat();
                }
            }
        }
    }

    // Load Bias
    int out_c_bias_json = obj[bias_name].size();
    assertm((out_channel == out_c_bias_json), "JSON file: output channel error");

    for (int i = 0; i < out_channel; i++) {
        bias[i] = obj[bias_name][i].asFloat();
    }
}

void Conv_Layer::forward(float ***input) {
    for (int i = 0; i < out_channel; i++) {
        for (int jx = 0; jx < out_x; jx++) {
            for (int jy = 0; jy < out_y; jy++) {
                output[i][jx][jy] = bias[i];
                for (int m = 0; m < in_channel; m++) {
                    for (int kx = 0; kx < kx_size; kx++) {
                        for (int ky = 0; ky < ky_size; ky++) {
                            output[i][jx][jy] += weight[m][i][kx][ky] * input[m][jx + kx][jy + ky];
                        }
                    }
                }
            }
        }
    }
}

void Conv_Layer::display_shape() {
    printf("Input Shape: (in_channel, in_x, in_y) = (%d, %d, %d)\n", in_channel, in_x, in_y);
    printf("Output Shape: (out_channel, out_x, out_y) = (%d, %d, %d)\n", out_channel, out_x, out_y);
    printf("Conv Layer Shape: (in_channel, out_channel, kx_size, ky_size) = (%d, %d, %d, %d)\n\n", in_channel, out_channel, kx_size, ky_size);
}

// FC_Layer
FC_Layer::FC_Layer(int in_c, int out_c, string layer_name) {
    in_channel = in_c;
    out_channel = out_c;

    const string weight_str = ".weight";
    const string bias_str = ".bias";
    name = layer_name;
    weight_name = name + weight_str;
    bias_name = name + bias_str;
    
    output = new_mem_1d<float>(out_c);
    weight = new_mem_2d<float>(in_c, out_c);
    bias = new_mem_1d<float>(out_c);
}

FC_Layer::~FC_Layer() {
    delete_mem_1d<float>(output);
    delete_mem_2d<float>(weight, in_channel, out_channel);
    delete_mem_1d<float>(bias);
}

void FC_Layer::load_param(string json_file_name) {
    Json::Reader reader;
    Json::Value obj;
    ifstream ifs(json_file_name);
    reader.parse(ifs, obj);

    // Load Weight
    int out_c_weight_json = obj[weight_name].size();
    assertm((out_channel == out_c_weight_json), "JSON file: output channel error");
    int in_c_weight_json = obj[weight_name][0].size();
    assertm((in_channel == in_c_weight_json), "JSON file: input channel error");
    
    for (int i = 0; i < out_channel; i++) {
        for (int j = 0; j < in_channel; j++) {
            weight[j][i] = obj[weight_name][i][j].asFloat();
        }
    }

    // Load Bias
    int out_c_bias_json = obj[bias_name].size();
    assertm((out_channel == out_c_bias_json), "JSON file: output channel error");

    for (int i = 0; i < out_channel; i++) {
        bias[i] = obj[bias_name][i].asFloat();
    }
}

void FC_Layer::forward(float *input) {
    for (int i = 0; i < out_channel; i++) {
        output[i] = bias[i];
        for (int j = 0; j < in_channel; j++) {
            output[i] += input[j] * weight[j][i];
        }
    }
}

void FC_Layer::display_shape() {
    printf("Input Shape: (in_channel) = (%d)\n", in_channel);
    printf("Output Shape: (out_channel) = (%d)\n\n", out_channel);
}

// MaxPool
MaxPool::MaxPool(int ch, int kx_s, int ky_s, int in_x_size, int in_y_size) {
    channel = ch;
    kx_size = kx_s;
    ky_size = ky_s;
    in_x = in_x_size;
    in_y = in_y_size;

    out_x = in_x / kx_size;
    out_y = in_y / ky_size;
    output = new_mem_3d<float>(channel, out_x, out_y);
}

MaxPool::~MaxPool() {
    delete_mem_3d<float>(output, channel, out_x, out_y);
}

void MaxPool::forward(float*** input) {
    for (int c = 0; c < channel; c++) {
        for (int x = 0; x < out_x; x++) { // change
            for (int y = 0; y < out_y; y++) { // change
                output[c][x][y] = input[c][x * kx_size][y * ky_size];
                for (int kx = 0; kx < kx_size; kx++) {
                    for (int ky = 0; ky < ky_size; ky++) {
                        output[c][x][y] = MAX(output[c][x][y], input[c][x * kx_size + kx][y * ky_size + ky]);
                    }
                }
            }
        }
    }
}

void MaxPool::display_shape() {
    printf("Input Shape: (channel, in_x, in_y) = (%d, %d, %d)\n", channel, in_x, in_y);
    printf("Output Shape: (channel, out_x, out_y) = (%d, %d, %d)\n", channel, out_x, out_y);
    printf("MaxPool Kernel Shape: (kx_size, ky_size) = (%d, %d)\n\n", kx_size, ky_size);    
}


// ReLU_1d
ReLU_1d::ReLU_1d(int ch) {
    channel = ch;
    output = new_mem_1d<float>(channel);
}

ReLU_1d::~ReLU_1d() {
    delete_mem_1d<float>(output);
}

void ReLU_1d::forward(float *input) {
    for (int c = 0; c < channel; c++) {
        if (input[c] > 0) {
            output[c] = input[c];
        } else {
            output[c] = 0;
        }
    }
}

void ReLU_1d::display_shape() {
    printf("Input & Output Shape: (channel) = (%d)\n\n", channel);
}

// ReLU_3d
ReLU_3d::ReLU_3d(int ch, int xs, int ys) {
    channel = ch;
    x_size = xs;
    y_size = ys;

    output = new_mem_3d<float>(channel, x_size, y_size);
}

ReLU_3d::~ReLU_3d() {
    delete_mem_3d<float>(output, channel, x_size, y_size);
}

void ReLU_3d::forward(float ***input) {
    for (int c = 0; c < channel; c++) {
        for (int x = 0; x < x_size; x++) {
            for (int y = 0; y < y_size; y++) {
                if (input[c][x][y] > 0) {
                    output[c][x][y] = input[c][x][y];
                } else {
                    output[c][x][y] = 0;
                }
            }
        }
    }
}

void ReLU_3d::display_shape() {
    printf("Input & Output Shape: (channel, x_size, y_size) = (%d, %d, %d)\n\n", channel, x_size, y_size); 
}

// Flatten_3d
Flatten_3d::Flatten_3d(int in_ch, int in_x_size, int in_y_size) {
    in_channel = in_ch;
    in_x = in_x_size;
    in_y = in_y_size;
    out_channel = in_channel * in_x * in_y;

    output = new_mem_1d<float>(out_channel);
}

Flatten_3d::~Flatten_3d() {
    delete_mem_1d<float>(output);
}

void Flatten_3d::forward(float ***input) {
    for (int c = 0; c < in_channel; c++) {
        for (int x = 0; x < in_x; x++) {
            for (int y = 0; y < in_y; y++) {
                output[c*in_x*in_y + x*in_y + y] = input[c][x][y];
            }
        }
    }
}

void Flatten_3d::display_shape() {
    printf("Input Shape: (in_channel, x_size, y_size) = (%d, %d, %d)\n", in_channel, in_x, in_y); 
    printf("Output Shape: (out_channel) = (%d)\n\n", out_channel);
}