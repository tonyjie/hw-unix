#include "covid19.hh"

// whole map: [0, 35], [0, 35]


float l2d(float x1, float y1, float x2, float y2) {
   return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

float bound(float min, float max, float input) {
   float output;
   if (input > max) {
      output = max;
   } else if (input < min) {
      output = min;
   } else {
      output = input;
   }
   return output;
}

float RandomFloat(float a, float b) {
   float random = ((float) rand()) / (float) RAND_MAX;
   float diff = b - a;
   float r = random * diff;
   return a + r;
}

// Person
Person::Person(int n, float x, float y, bool sick_init, bool d_prog) {
   num = n;
   pos_x = x;
   pos_y = y;
   sick = sick_init;
   disease_progress = d_prog;
}

// 需要确定地图边界
void Person::move() {
   float move_x = RandomFloat(-5, 5);
   float move_y = RandomFloat(-5, 5);
   pos_x += move_x;
   pos_y += move_y;
   pos_x = bound(X_MIN, X_MAX, pos_x);
   pos_y = bound(Y_MIN, Y_MAX, pos_y);
}

void Person::infect(Person& p) {
   if (sick == true && disease_progress == true && p.sick == false) {
      float dis = l2d(pos_x, pos_y, p.pos_x, p.pos_y);
      if (dis < INFECT_DIS) {
         p.sick = true;
      }
   } else {
      return;
   }
}

void Person::progress() {
   if (sick == true && disease_progress == false) {
      disease_progress = true;
   }
}


// Base
Base::Base(float x, float y) {
   pos_x = x; 
   pos_y = y;
}

void Base::detect(Person p, int day) {
   float dist = l2d(pos_x, pos_y, p.pos_x, p.pos_y);
   if (dist > 2) { // 通信距离2km
      return;
   } else {
      struct person_info p_info = person_info(p.num, p.pos_x, p.pos_y, p.sick, day);
      info_list.push_back(p_info);
   }
}

vector<person_info> Base::search(int num) {
   vector<person_info> specific_info; // 指定人的信息
   vector<person_info>::iterator it;
   for (it = info_list.begin(); it != info_list.end(); it++) {
      if (it->num == num) {
         specific_info.push_back(*it);
      }
   }
   return specific_info;
}

int Base::get_covid_num(int day) {
   int covid_num = 0;
   vector<person_info>::iterator it;
   for (it = info_list.begin(); it != info_list.end(); it++) {
      if (it->day == day) { // this day
         int dis = l2d(pos_x, pos_y, it->pos_x, it->pos_y);
         if (dis <= 1) { // 1km 范围内
            covid_num ++;
         }
      }
   }
   return covid_num;
}

bool DaySort (person_info a, person_info b) {
   return (a.day < b.day);
}


// main()
int main()
{
   // srand((unsigned)time(NULL)); 

   Base b_array[B_NUM] = {
      Base(2, 2), Base(12, 2), Base(22, 2), Base(32, 2),
      Base(2, 12), Base(12, 12), Base(22, 12), Base(32, 12),
      Base(2, 22), Base(12, 22), Base(22, 22), Base(32, 22),
      Base(2, 32), Base(12, 32), Base(22, 32), Base(32, 32)
   }; 


   // 每天一个循环, 首先所有Person move, 然后所有Base进行detect(对所有的Person, 可以汇总为一个Person数组)，对于detect到的Person，将其信息记录在Base中。
   // 最后想得到一个人的所有信息的话，就从所有Base中调出这个Person的所有信息，汇总起来
   // 以上 完成task 1

   Person p_array[P_NUM];
   // p_array initialization
   for (int pi = 0; pi < P_NUM; pi++) {
      p_array[pi].num = pi + 1;
      p_array[pi].pos_x = RandomFloat(0, 35);
      p_array[pi].pos_y = RandomFloat(0, 35);
   }
   p_array[0].sick = true; // 设定 编号1的人员患病 且 具备传染能力
   p_array[0].disease_progress = true;



   for (int day = 0; day < DAY_NUM; day++) { // every day
      // Covid-19 spreading
      for (int pi = 0; pi < P_NUM; pi++) {
         for (int pj = 0; pj < P_NUM; pj++) {
            p_array[pi].infect(p_array[pj]);
         }
      }
      // Bases are detecting
      for (int bi = 0; bi < B_NUM; bi++) {
         for (int pi = 0; pi < P_NUM; pi++) {
            b_array[bi].detect(p_array[pi], day);
         }
      }
      // People are moving (randomly)
      for (int pi = 0; pi < P_NUM; pi++) {
         p_array[pi].move();
         p_array[pi].progress();
      }
   }

   // DEBUG
   // for (int bi = 0; bi < B_NUM; bi++) {
   //    for  (int j = 0; j < b_array[bi].info_list.size(); j++) {
   //       cout << b_array[bi].info_list[j].num << ":" << b_array[bi].info_list[j].day << "\t";   
   //    }
   //    cout << endl;
   // }

   // Task 1: 显示指定人在簇节点能观察到的范围内，云端获得的其所有活动轨迹。其中没有记录的行踪表示，在当天指定人没有处在被簇节点覆盖到的通信范围内（以簇节点为中心，半径为2km的圆）

   // Information Aggregation
   int p_locate; // the person we need to locate
   cout << "================ Task 1: 显示指定人的活动轨迹 ==============" << endl;
   cout << "输入你想查找的人的编号(1-" << P_NUM << ")：";
   cin >> p_locate;

   vector<person_info> global_info;
   for (int bi = 0; bi < B_NUM; bi++) {
      vector<person_info> local_info = b_array[bi].search(p_locate);
      global_info.insert(global_info.end(), local_info.begin(), local_info.end());
   }

   sort(global_info.begin(), global_info.end(), DaySort); // 按照day 从小到大排列
   cout << "时间; 位置(pos_x, pos_y); 是否患病" << endl;
   for (int i = 0; i < global_info.size(); i++) {
      cout << global_info[i].day << ": (" << global_info[i].pos_x << "," << global_info[i].pos_y << ") Sick: " << global_info[i].sick << endl;
   }

   // Task 2：展示指定某天、指定簇节点周围1km的COVID-19感染人数 (因为在本环境中，云端知道的信息只来自于簇节点周围2km的圆范围内，因此从任意指定位置中统计感染人数并不科学，所以这里规定“指定位置”为簇节点，直接显示所有簇节点周围1km的感染人数)
   cout << endl;
   cout << "================ Task 2: 展示指定某天、簇节点1km范围内的Covid-19感染人数 =========" << endl;
   cout << "输入你想查找的日期(0-" << DAY_NUM - 1 << "): ";
   int day_locate;
   cin >> day_locate;
   
   for (int bi = 0; bi < B_NUM; bi++) {
      int covid_num = b_array[bi].get_covid_num(day_locate);
      cout << "Base " << bi << " 节点(" << b_array[bi].pos_x << ", " << b_array[bi].pos_y << ") " << "的1km范围内感染人数：" << covid_num << endl;
   }




   return 0;
}