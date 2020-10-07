#ifndef _COVID19_HH_
#define _COVID19_HH_

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

#define X_MIN 0
#define X_MAX 35
#define Y_MIN 0
#define Y_MAX 35
#define INFECT_DIS 1
#define B_NUM 16
#define P_NUM 500
#define DAY_NUM 60

struct person_info
{
    int num; // 编号
    float pos_x, pos_y;
    bool sick;
    int day;
    person_info(int n, float x, float y, bool s, int d) {
        num = n;
        pos_x = x;
        pos_y = y;
        sick = s;
        day = d;
    }
};


// 人
class Person 
{
    public:
        Person(int n = 0, float x_init = 0, float y_init = 0, bool sick_init = false, bool disease_progress = false);

        void move(); // move randomly (指定一个 x方向的random值，和y方向的random值) 同时规定地图边界
        void infect(Person& p); // 若自己患病，判别与其他人的位置，若距离小于阈值则感染其他人
        void progress(); // 病情发展：被传染后一天，患者具备传染能力


    // private:
        int num; // 编号
        bool sick; // 是否患病
        bool disease_progress; // （病情发展）是否有传染能力：刚刚被传染时，还不能传染其他人；被传染一天后，可以传染其他人
        // 否则一个人患病，可以在当天就可能顺着人流 串联 地传播到很远的地方
        float pos_x;
        float pos_y;    
};

// 簇节点
class Base
{
    public:
        Base(float x, float y);
        void detect(Person p, int day); // 发现2km内有Person对象，则记录; 之后可以改成Person数组
        vector<person_info> search(int num); // 根据Person编号，返回Base中记录的该人员相应信息
        int get_covid_num(int day);

        float pos_x; // 簇节点位置 (pos_x, pos_y), 单位: km
        float pos_y;

        vector<person_info> info_list;
};


#endif // _COVID19_HH_