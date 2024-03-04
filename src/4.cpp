#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <random>
#include <chrono>

// row major index
constexpr int rm_idx(int n_cols, int row, int col) {
    return n_cols * row + col;
}

// column major index
constexpr int cm_idx(int n_rows, int row, int col) {
    return n_rows * col + row;
}

template<bool b_col_major, typename T>
void matmul(
        const T *a, int a_rows, int a_cols,
        const T *b, int b_rows, int b_cols,
              T *c, int c_rows, int c_cols)
{
    assert(a_cols == b_rows);
    assert(c_rows == a_rows);
    assert(c_cols == b_cols);

    for (int i = 0; i < c_rows; ++i) {
        for (int j = 0; j < c_cols; ++j) {
            T acc = 0;
            for (int k = 0; k < a_cols; ++k) {
                if constexpr (b_col_major)
                    acc += a[rm_idx(a_cols, i, k)] * b[cm_idx(b_cols, k, j)];
                else
                    acc += a[rm_idx(a_cols, i, k)] * b[rm_idx(b_cols, k, j)];
            }
            c[rm_idx(c_cols, i, j)] = acc;
        }
    }
}

void test() {
    int a[3*2] = { 
        -3, -2, 
        -1, 0, 
        1, 2 
    };
    int b[2*4] = { 
        1, 2, 3, 4, 
        5, 6, 7, 8 
    };
    int c[3*4];
    int c_expected[3*4] = { 
        -13, -18, -23, -28,  
         -1,  -2,  -3,  -4,  
         11,  14,  17,  20 
    };
    matmul<false>(a, 3, 2, b, 2, 4, c, 3, 4);
    assert(memcmp(c, c_expected, sizeof(c)) == 0);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("expected 3 arguments: initial size, final size and multiplicative step\n");
        printf("usage: <binary> <init_size> <final_size> <mult_step>\n");
        return 1;
    }

    int init_size = atol(argv[1]);
    int final_size = atol(argv[2]);
    int step = atol(argv[3]);

    if (init_size <= 0) {
        printf("initial size must be a positive integer\n");
        return 1;
    }

    // naive matmul algorithm is very slow...
    if (final_size > 10000) {
        printf("please choose a smaller final matrix size\n");
        return 1;
    }

    if (init_size >= final_size) {
        printf("initial size must be lower than final size\n");
        return 1;
    }

    std::vector<float> a(final_size*final_size);
    std::vector<float> b(final_size*final_size);
    std::vector<float> c(final_size*final_size);


    std::default_random_engine rng(1337);
    std::uniform_real_distribution<float> dist(-1.f, 1.f);

    printf("generating random matrices A and B...\n");
    for (float &i: a) i = dist(rng);
    for (float &i: b) i = dist(rng);

    using clk_t = std::chrono::steady_clock;
    int size = init_size;
    while (true) {
        auto start = clk_t::now();

        matmul<false>(a.data(), size, size, 
                b.data(), size, size, 
                c.data(), size, size);


        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(
                clk_t::now() - start).count() / 1000.f;
        printf("size %d time %.3fms\n", size, delta);

        // make sure the matmul doesn't get optimized away
        volatile float s = 0;
        for (int i = 0; i < size*size; ++i)
            s += c[i];

        if (size >= final_size)
            break;
        size = std::min(size * step, final_size);
    }
}


