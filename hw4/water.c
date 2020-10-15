#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) ((a)>(b)?(a):(b))
#define MIN(a, b) ((a)<(b)?(a):(b))

typedef struct Water {
    int a; // 8ml 杯子中水的体积
    int b; // 5ml 杯子中水的体积
    int c; // 3ml 杯子中水的体积
    struct Water *parent; // 父节点
    int op; // 1-6: 分别表示a->b, b->a, a->c, c->a, b->c, c->b
} Water;

#define ElementType Water* //存储数据元素的类型
#define MAXSIZE 10000000 //存储数据元素的最大个数, 6^7 = 279936

typedef struct {
    ElementType data[MAXSIZE];
    int front; //记录队列头元素位置
    int rear; //记录队列尾元素位置
    int size; //存储数据元素的个数
} Queue;
 
Queue* CreateQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) {
        printf("空间不足\n");
        return NULL;
    }
    q->front = -1;
    q->rear = -1;
    q->size = 0;
    return q;
}
 
int IsFullQ(Queue* q) {
    return (q->size == MAXSIZE);
}
 
void AddQ(Queue* q, ElementType item) {
    if (IsFullQ(q)) {
        printf("队列已满\n");
        return;
    }
    q->rear++;
    q->rear %= MAXSIZE;
    q->size++;
    q->data[q->rear] = item;
}
 
int IsEmptyQ(Queue* q) {
    return (q->size == 0);
}
 
ElementType DeleteQ(Queue* q) {
    q->front++;
    q->front %= MAXSIZE; //0 1 2 3 4 5
    q->size--;
    return q->data[q->front];
}

// Test whether the water volume is divided into 4ml and 4ml
bool Success(Water* state) {
    if ((state->a == 4 && state->b == 4) || (state->a == 4 && state->c == 4) || (state->b == 4 && state->c == 4)) {
        return true;
    } else {
        return false;
    }
}

Water* a2b(Water* input) { // a->b
    Water* output = (Water*)malloc(sizeof(Water));
    output->c = input->c;
    int drop = MIN(5 - input->b, input->a);
    output->a = input->a - drop;
    output->b = input->b + drop;
    output->parent = input;
    output->op = 1;
    return output;
}

Water* b2a(Water* input) { // b->a
    Water* output = (Water*)malloc(sizeof(Water));
    output->c = input->c;
    int drop = MIN(8 - input->a, input->b);
    output->a = input->a + drop;
    output->b = input->b - drop;
    output->parent = input;
    output->op = 2;
    return output;
}

Water* a2c(Water* input) { // a->c
    Water* output = (Water*)malloc(sizeof(Water));
    output->b = input->b;
    int drop = MIN(3 - input->c, input->a);
    output->a = input->a - drop;
    output->c = input->c + drop;
    output->parent = input;
    output->op = 3;
    return output;
}

Water* c2a(Water* input) { // c->a
    Water* output = (Water*)malloc(sizeof(Water));
    output->b = input->b;
    int drop = MIN(8 - input->a, input->c);
    output->a = input->a + drop;
    output->c = input->c - drop;
    output->parent = input;
    output->op = 4;
    return output;
}

Water* b2c(Water* input) { // b->c
    Water* output = (Water*)malloc(sizeof(Water));
    output->a = input->a;
    int drop = MIN(3 - input->c, input->b);
    output->b = input->b - drop;
    output->c = input->c + drop;
    output->parent = input;
    output->op = 5;
    return output;
}

Water* c2b(Water* input) { // c->b
    Water* output = (Water*)malloc(sizeof(Water));
    output->a = input->a;
    int drop = MIN(5 - input->b, input->c);
    output->b = input->b + drop;
    output->c = input->c - drop;
    output->parent = input;
    output->op = 6;
    return output;
}

Water* BFS(Water* init) {
    Queue* q = CreateQueue();
    Water* v;
    // visit init node
    if (Success(init) == true) {
        return init; 
    }
    AddQ(q, init);
    
    while (!IsEmptyQ(q)) {
        v = DeleteQ(q);
        // 把v的所有子节点都添加到Q里, 六叉树
        Water* next[6] = {a2b(v), b2a(v), a2c(v), c2a(v), b2c(v), c2b(v)};
        for (int i = 0; i < 6; i++) {
            if (Success(next[i]) == true) {
                return next[i];
            }
            AddQ(q, next[i]);
        }
    }
}

void print_water(Water* w) {
    printf("(%d, %d, %d)\n", w->a, w->b, w->c);
    switch (w->op)
    {
    case 1:
        printf("A -> B\n");
        break;
    case 2:
        printf("B -> A\n");
        break;
    case 3:
        printf("A -> C\n");
        break;
    case 4:
        printf("C -> A\n");
        break;
    case 5:
        printf("B -> C\n");
        break;
    case 6:
        printf("C -> B\n");
        break;    
    default:
        printf("Start!\n");
        break;
    }
}

int main() {
    Water* water_init = (Water*)malloc(sizeof(Water));
    water_init->a = 8; water_init->b = 0; water_init->c = 0;
    water_init->parent = NULL; water_init->op = 0; 

    Water* final = BFS(water_init);

    printf("倒水方法（从后往前表示）: \n");
    int step = 0;
    Water* temp = final;
    while (temp != NULL) {
        step ++;
        print_water(temp);
        temp = temp->parent;
    }
    printf("最小倒水次数：%d\n", step-1);

    return 0;
}