#ifndef _UTILS_HH_
#define _UTILS_HH_

template <class T>
T *new_mem_1d(int c) {
    T *mem = new T[c];
    return mem;
}

template <class T>
T **new_mem_2d(int in_c, int out_c) {
    T **mem = new T *[in_c];
    for (int i = 0; i < in_c; i++) {
        mem[i] = new T[out_c];
    }
    return mem;
}

template <class T>
T ***new_mem_3d(int c, int x, int y) {
    T ***mem = new T **[c];
    for (int i = 0; i < c; i++) {
        mem[i] = new T *[x];
        for (int j = 0; j < x; j++) {
            mem[i][j] = new T[y];
        }
    }
    return mem;
}

template <class T>
T ****new_mem_4d(int in_c, int out_c, int k_x, int k_y) {
    T ****mem = new T ***[in_c];
    for (int i = 0; i < in_c; i++) {
        mem[i] = new T **[out_c];
        for (int j = 0; j < out_c; j++) {
            mem[i][j] = new T *[k_x];
            for (int k = 0; k < k_x; k++) {
                mem[i][j][k] = new T[k_y];
            }
        }
    }
    return mem;
}

template<class T>
void delete_mem_1d(T *mem) {
    delete [] mem;
}

template<class T>
void delete_mem_2d(T **mem, int in_c, int out_c) {
    for (int i = 0; i < in_c; i++) {
        delete [] mem[i];
    }
    delete [] mem;
}

template<class T>
void delete_mem_3d(T ***mem, int c, int x, int y) {
    for (int i = 0; i < c; i++) {
        for (int j = 0; j < x; j++) {
            delete [] mem[i][j];
        }
        delete [] mem[i];
    }
    delete [] mem;
}

template<class T>
void delete_mem_4d(T ****mem, int in_c, int out_c, int k_x, int k_y) {
    for (int i = 0; i < in_c; i++) {
        for (int j = 0; j < out_c; j++) {
            for (int k = 0; k < k_x; k++) {
                delete [] mem[i][j][k];
            }
            delete [] mem[i][j];
        }
        delete [] mem[i];
    }
    delete [] mem;
}

#endif // _UTILS_HH_