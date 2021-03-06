# C/UNIX 程序设计 作业
## hw1 - 模拟监控covid-19的物联网
### 编译运行方式
```
cd hw1
make
```
### 参数设置
设定的环境为一个二维地图    
在covid19.hh中通过宏定义规定了一些参数，其含义为：  
X_MIN, X_MAX, Y_MIN, Y_MAX 规定了地图的大小[X_MIN, X_MAX] * [Y_MIN, Y_MAX] = [0, 35] * [0, 35]  
INFECT_DIS 规定 两个人在同一天的距离小于 INFECT_DIS 时，可以互相感染COVID-19    
B_NUM 为簇节点的个数    
P_NUM 为人的数量    
DAY_NUM 为仿真总天数    



## hw2 - 用天平称3次，在12个颜色体积相同小球中选出重量不同的小球
### 编译运行方式
```
cd hw2
python3 select_ball.py
```
可以手动选择或随机生成一个质量不同的球，此后程序模拟天平称重三次的结果，根据算法得出结果，并展示所有小球的实际质量。比对可见，称重判别结果正确。    

## hw3 - 实现分数Struct的四则运算操作、约简和转化到浮点型的函数
### 编译运行方式
```
cd hw3
make
```

##  hw4 - 杯子分水问题
### 编译运行方式
```
cd hw4
make
```

## hw5 - CSV与二进制文件转换
### 编译运行方式
```
cd hw5
python3 csv2bin.py // 将csv文件(info.csv)进行格式转换，存储在二进制文件中 (info.bin)
python3 loadbin.py // 从二进制文件中读取表，打印出来
```

利用Python struct.pack和struct.unpack函数，通过fmt的设置规定了二进制存储格式。  
大端字节序，表头以“2字节字符 + 4字节字符 + 10字节字符 + 3字节字符” 格式存储，后续一个学生的信息以“4字节整型+20字节字符+5字节字符+4字节整型” 格式存储。

## hw6 - 多进程-数据表格式转换
### 编译运行方式
```
cd hw6
python3 csv2bin_multiprocess.py
python3 loadbin.py
```
利用Python multiprocessing.Pool 实现进程池  

## hw7 - 进程通信-数据表格式转换
### 编译运行方式
```
cd hw7
python3 csv2bin_process_communicate.py
python3 load_mmap.py
```
利用Python multiprocessing.Pipe 实现进程间通信  
利用mmap文件映射方式读二进制文件    

## hw8 - 分布式转码-Socket网络通信
### 编译运行方式
开4个命令行，分别跑3个服务器和1个客户端 
```
cd hw8
python3 server1.py
```

```
cd hw8
python3 server2.py
```

```
cd hw8
python3 server3.py
```

```
cd hw8
python3 client.py
```

生成info.bin二进制文件，可以进行验证：
```
cd hw8
python3 loadbin.py
```