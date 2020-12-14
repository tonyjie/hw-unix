# 大作业：卷积神经网络运行框架
C++实现卷积神经网络框架的前向推理功能
1. 加载模型参数
2. 读取、展示图片
3. 实现基本运算单元并运行网络：卷积层、全连接层、Pooling层、激活函数
4. 实现MNIST数据集的识别Demo

## Prerequisites
### JsonCpp C++ library
https://github.com/open-source-parsers/jsoncpp  
已经安装并将必要的依赖文件 (1个.cpp文件和2个.h文件) 生成并放在`./dist` 下，编译时通过`-I  ${PATH_jsoncpp_dist}` 链接即可调用JsonCPP lib   

### OpenCV C++ library
https://docs.opencv.org/master/index.html   
最快捷的方法：通过apt-get安装
```
sudo apt-get install libopencv-dev
```

### Pytorch Python library
```
pip3 install torch
pip3 install torchvision
```

## 编译运行方法
1. 在Python下用Pytorch进行神经网络训练，保存模型在 `Net.pth`
2. 通过脚本`model_converter.py`将模型参数保存为JSON格式的 `param.json` 中
3. 在C++中读取JSON文件，并构建模型，进行神经网络前向测试。通过Makefile编译执行。
```
# step 1
python3 train_pytorch.py
# step 2
python3 model_converter.py
# step 3
make
# step 4: clean the executables
make clean
```

## 功能细节
### 以JSON格式进行参数保存/加载
1. `model_converter.py` ：将pytorch模型各个有参数的层以 `Dict {layer_name : parameter}` 的形式存储，再用python的json库dump为 `.json` 文件。        
2. `layer.cpp` : `Layer` 类的 `load_param` 函数利用 `JsonCPP` 库加载模型参数   

### 读取、展示图片
1. `main.cpp` ：利用OpenCV的imread函数读取为cv::Mat，再转化为数组存储
2. `main.cpp` ：利用OpenCV的imshow函数实现

### 实现基本运算单元
在 `layer.h` `layer.cpp` 中定义和实现

#### 卷积层
class Conv_Layer

#### 全连接层
class FC_Layer

#### Pooling层
class MaxPool

#### ReLU层
class ReLU_3d, ReLU_1d

### Demo: 实现手写数字（Mnist数据集）的识别
在 `main.cpp` 中进行demo实现，完成对单张图片的读取、展示、分类预测，对MNIST训练/测试数据集的读取和准确率计算。  
其中，对MNIST binary dataset的读取在 `mnist.h` `mnist.cpp` 中实现。  
C++中实现的前向准确率与Python-Pytorch中结果一致。   

## 参考
tiny-dnn project: https://github.com/tiny-dnn/tiny-dnn  
