#include "lenet.h"
#include "utils.h"
#include "mnist.h"
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

#define MNIST_X_SIZE 28
#define MNIST_Y_SIZE 28

const string train_Image_Path = "./data/MNIST/raw/train-images-idx3-ubyte"; 
const string train_Label_Path = "./data/MNIST/raw/train-labels-idx1-ubyte";
const string test_Image_Path = "./data/MNIST/raw/t10k-images-idx3-ubyte";
const string test_Label_Path = "./data/MNIST/raw/t10k-labels-idx1-ubyte";

float ***read_image(string image_path, int x_size, int y_size) {
    Mat img = imread(image_path, IMREAD_GRAYSCALE);
    if (img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        exit(1);
    }
    float ***input_image = new_mem_3d<float>(1, x_size, y_size);
    for (int c = 0; c < 1; c++) {
        for (int x = 0; x < x_size; x++) {
            for (int y = 0; y < y_size; y++) {
                input_image[c][x][y] = (float)img.at<uchar>(x, y) / 256;
            }
        }
    }
    return input_image;
}

void show_image(string image_path) { // can't display on vscode ssh / can display on MobaXterm
    Mat img = imread(image_path, IMREAD_GRAYSCALE);
    imshow("image", img);
    waitKey(0);
}

float test_acc(LeNet &lenet) {
    float acc;
    Mat testData;
    Mat testLabel;
    testData = read_mnist_image(test_Image_Path);
    testLabel = read_mnist_label(test_Label_Path);
    int total_num = testData.rows; // 10000
    int correct = 0;

    float ***input_image = new_mem_3d<float>(1, MNIST_X_SIZE, MNIST_Y_SIZE);
    for (int i = 0; i < total_num; i++) {
        // opencv Mat -> float*** input_image
        for (int c = 0; c < 1; c++) {
            for (int x = 0; x < MNIST_X_SIZE; x++) {
                for (int y = 0; y < MNIST_Y_SIZE; y++) {
                    input_image[c][x][y] = testData.at<float>(i, x * MNIST_Y_SIZE + y);
                }
            }
        }
        lenet.forward(input_image);
        int predict_result = lenet.predict;
        int label = testLabel.at<unsigned int>(i, 0);
        
        if (label == predict_result) {
            correct += 1;
        }

        if ((i % 1000 == 0) && (i != 0)) {
            float temp_acc = (float)correct / (float)i;
            printf("[%5d / %5d]   Accuracy: %10f\n", i, total_num, temp_acc);
        }
    }
    acc = (float)correct / (float)total_num;
    return acc;
}

float train_acc(LeNet &lenet) {
    float acc;
    Mat trainData;
    Mat trainLabel;
    trainData = read_mnist_image(train_Image_Path);
    trainLabel = read_mnist_label(train_Label_Path);
    int total_num = trainData.rows; // 10000
    int correct = 0;

    float ***input_image = new_mem_3d<float>(1, MNIST_X_SIZE, MNIST_Y_SIZE);
    for (int i = 0; i < total_num; i++) {
        // opencv Mat -> float*** input_image
        for (int c = 0; c < 1; c++) {
            for (int x = 0; x < MNIST_X_SIZE; x++) {
                for (int y = 0; y < MNIST_Y_SIZE; y++) {
                    input_image[c][x][y] = trainData.at<float>(i, x * MNIST_Y_SIZE + y);
                }
            }
        }
        lenet.forward(input_image);
        int predict_result = lenet.predict;
        int label = trainLabel.at<unsigned int>(i, 0);
        
        if (label == predict_result) {
            correct += 1;
        }

        if ((i % 1000 == 0) && (i != 0)) {
            float temp_acc = (float)correct / (float)i;
            printf("[%5d / %5d]   Accuracy: %10f\n", i, total_num, temp_acc);
        }
    }
    acc = (float)correct / (float)total_num;
    return acc;
}

int main() {
    const string json_file_name = "param.json";

    // Test on single input image
    cout << "====== Test on single input image =======" << endl;
    string image_2_path = "./data/2.png";
    string image_5_path = "./data/5.png";
    string image_6_path = "./data/6.png";
    string image_7_path = "./data/7.png";

    // show_image(image_2_path);

    float*** input_image_2 = read_image(image_2_path, MNIST_X_SIZE, MNIST_Y_SIZE);
    float*** input_image_5 = read_image(image_5_path, MNIST_X_SIZE, MNIST_Y_SIZE);
    float*** input_image_6 = read_image(image_6_path, MNIST_X_SIZE, MNIST_Y_SIZE);
    float*** input_image_7 = read_image(image_7_path, MNIST_X_SIZE, MNIST_Y_SIZE);

    // init Neural Network
    cout << "Init Neural Network......" << endl;
    LeNet lenet(json_file_name, MNIST_X_SIZE, MNIST_Y_SIZE);
    
    // Predict image_2
    lenet.forward(input_image_2);
    int result_2 = lenet.predict;
    printf("Image Path: %s, Predict Result: %d\n", image_2_path.c_str(), result_2);

    // Predict image_5
    lenet.forward(input_image_5);
    int result_5 = lenet.predict;
    printf("Image Path: %s, Predict Result: %d\n", image_5_path.c_str(), result_5);

    // Predict image_6
    lenet.forward(input_image_6);
    int result_6 = lenet.predict;
    printf("Image Path: %s, Predict Result: %d\n", image_6_path.c_str(), result_6);

    // Predict image_7
    lenet.forward(input_image_7);
    int result_7 = lenet.predict;
    printf("Image Path: %s, Predict Result: %d\n", image_7_path.c_str(), result_7);    

    // compute the accuracy on test set
    cout << "\n======= Compute the accuracy on the test set =========" << endl;
    float test_accuracy = test_acc(lenet);
    cout << "Accuracy on MNIST test set: " << test_accuracy << endl;
    
    // compute the accuracy on training set
    cout << "\n======= Compute the accuracy on the training set =========" << endl;
    float train_accuracy = train_acc(lenet);
    cout << "Accuracy on MNIST train set: " << train_accuracy << endl;

    return 0;
}