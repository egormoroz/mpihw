#include <cstdio>
#include <vector>
#include <chrono>


// row major index
constexpr int rm_idx(int n_cols, int row, int col) {
    return n_cols * row + col;
}

// creates diagonally dominant matrix
void gen_square_matrix(float *a, int n) {
    for (int i = 0; i < n*n; ++i)
        a[i] = 1;

    for (int i = 0; i < n; ++i)
        a[rm_idx(n, i, i)] = float(n);
}

void gauss_solve(float *a, float *b, int n) {
    for (int i = 0; i < n - 1; ++i) {
        // a[i] /= a[i, i]
        float t = 1 / a[rm_idx(n, i, i)];
        for (int k = 0; k < n; ++k)
            a[rm_idx(n, i, k)] *= t;
        b[i] *= t;

        // a[j] -= a[j, i] * a[i] => a[j, i] == 0
        for (int j = i + 1; j < n; ++j) {
            float t = a[rm_idx(n, j, i)];
            for (int k = 0; k < n; ++k)
                a[rm_idx(n, j, k)] -= t * a[rm_idx(n, i, k)];
            b[j] -= b[i] * t;
        }
    }

    b[n - 1] /= a[rm_idx(n, n - 1, n - 1)];
    a[rm_idx(n, n - 1, n - 1)] = 1;

    for (int i = n - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            float t = a[rm_idx(n, j, i)];
            a[rm_idx(n, j, i)] -= t; // == 0
            b[j] -= b[i] * t;
        }
    }
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

    if (final_size > 10000) {
        printf("please choose a smaller final size\n");
        return 1;
    }

    if (init_size >= final_size) {
        printf("initial size must be lower than final size\n");
        return 1;
    }

    std::vector<float> a(final_size*final_size);
    std::vector<float> b(final_size);

    using clk_t = std::chrono::steady_clock;
    int size = init_size;
    while (true) {
        gen_square_matrix(a.data(), size);
        for (int i = 0; i < size; ++i) b[i] = 1.f;

        auto start = clk_t::now();

        gauss_solve(a.data(), b.data(), size);

        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(
                clk_t::now() - start).count() / 1000.f;
        printf("size %d time %.3fms\n", size, delta);

        volatile float s = 0;
        for (int i = 0; i < size; ++i)
            s += b[i];

        if (size >= final_size)
            break;
        size = std::min(size * step, final_size);
    }
}

