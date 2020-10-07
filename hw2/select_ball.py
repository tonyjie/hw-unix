import random

# 对12个球进行编码，每个球的编码共3位，分别代表第1、2、3次的称重操作：0代表放在天平左边，1代表放在天平右边，2代表没有进行称重
# 每次12个球都分为3组（4,4,4），其中两组放上天平进行称重。因此对应于编码，12个编码的每一位中都应该'0', '1', '2', 各出现4次。
# 编码在'0', '1' 同时取反后，反编码不存在与序列之中 -> 否则总有两个球同时出现在天平的两侧 -> 无法判断
# 共有 (3^3 + 1) / 2 = 14 种编码方式 > 12 球的数量

def Ball_Init():
    ball_code = [[0,0,1], [1,1,2], [2,2,0], [0,1,0], [1,2,1], [2,0,2], \
                 [0,1,1], [1,2,2], [2,0,0], [0,1,2], [1,2,0], [2,0,1]]
    ball = []
    for i in range(len(ball_code)):
        ball.append([i, 5, ball_code[i]]) # [index, weight, ball_code]
    return ball
    
def Set_Weight(ball):
    n = len(ball)
    select = input("手动选择(Y)或随机生成(N)重量不同的球：")
    while select not in 'YyNn':
        select = input("输入错误！请重新输入(Y / N)：")
    if select == 'Y' or select == 'y':
        index = int(input("请在0~%d中选择一个球，改变其质量："%(n-1)))
        while index not in range(0, n):
            index = int(input("输入错误！请重新选择："))
        weight = int(input("请选择增加质量(1)或减少质量(-1)："))
        while weight != 1 and weight != -1:
            weight = int(input("输入错误！请重新输入(1 / -1)："))
        ball[index][1] += weight
    elif select == 'N' or select == 'n':
        ball[random.randint(0, n-1)][1] += random.choice([1, -1])
    return ball

def Compare_Weight(ball):
    n = len(ball)
    
    light_code = [] # ball code if the special ball is lighter
    heavy_code = [] # ball code if the special ball is heavier
    
    for num in range(3): # 三次称重
        left = 0
        right = 0
        left_index = []
        right_index = []
        for i in range(n):
            if ball[i][2][num] == 0:
                left += ball[i][1]
                left_index.append(ball[i][0])
            elif ball[i][2][num] == 1:
                right += ball[i][1]
                right_index.append(ball[i][0])
        print("天平左侧球编号：", left_index,"; 天平右侧球编号：", right_index)
        if left == right:
            light_code.append(2)
            heavy_code.append(2)
            print("第%d次称重结果：天平平衡\n"%num)
        elif left < right:
            light_code.append(0)
            heavy_code.append(1)
            print("第%d次称重结果：天平右重\n"%num)
        elif left > right:
            light_code.append(1)
            heavy_code.append(0)
            print("第%d次称重结果：天平左重\n"%num)
    return light_code, heavy_code

def Find_Ball(light_code, heavy_code, ball):
    n = len(ball)
    for i in range(n):
        if light_code == ball[i][2]:
            special_index = i
            print("特殊球编号为 %d ，较轻\n"%ball[i][0])
        if heavy_code == ball[i][2]:
            special_index = i
            print("特殊球编号为 %d ，较重\n"%ball[i][0])
    return special_index

def Print_Weight(ball):
    n = len(ball)
    print("================小球的实际质量 (编号:质量)===================")
    for i in range(n):
        print(ball[i][0]," : ", ball[i][1])

def main():
    # 对12个球进行编码，规定了其称重方式
    ball_init = Ball_Init()

    # 规定球的重量和特殊球的编号
    ball = Set_Weight(ball_init)

    # 比较重量
    light_code, heavy_code = Compare_Weight(ball)
    print("若质量不同的球较轻，其编码应为：", light_code)
    print("若质量不同的球较重，其编码应为：", heavy_code, "\n")

    # 比较球的编码，找出特殊球
    special_index = Find_Ball(light_code, heavy_code, ball)

    # 打印所有球的质量
    Print_Weight(ball)

if __name__ == "__main__":
    main()