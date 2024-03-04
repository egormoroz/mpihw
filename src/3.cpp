#include <cstdio>
#include <vector>
#include <cmath>
#include <cassert>
#include <chrono>

constexpr int rm_idx(int n_cols, int row, int col) {
    return n_cols * row + col;
}

struct Matrix {
    Matrix(int rows, int cols)
        : data_(rows*cols, 0), rows_(rows), cols_(cols)
    {}

    int num_rows() const { return rows_; }
    int num_cols() const { return cols_; }

    float& operator()(int row, int col) { return data_[rm_idx(cols_, row, col)]; }
    const float& operator()(int row, int col) const { return data_[rm_idx(cols_, row, col)]; }

private:
    std::vector<float> data_;
    int rows_, cols_;
};

void estimate_dfdy(const Matrix &f, Matrix &dfdy) {
    int n_rows = f.num_rows(), n_cols = f.num_cols();
    assert(n_rows == dfdy.num_rows() && n_cols == dfdy.num_cols());

    for (int i = 0; i < n_rows; ++i)
        dfdy(i, 0) = (f(i, 1) - f(i, 0)) / 1;

    for (int i = 0; i < n_rows; ++i)
        dfdy(i, n_rows - 1) = (f(i, n_rows - 1) - f(i, n_rows - 2)) / 1;

    for (int i = 0; i < n_rows; ++i)
        for (int j = 1; j < n_cols - 1; ++j)
            dfdy(i, j) = (f(i, j + 1) - f(i, j - 1)) / 2;
}

float f(float x, float y) {
    return sin(y * exp(-x));
}

// float dfdy(float x, float y) {
//     float t = exp(-x);
//     return t * cos(y * t);
// }

// template<typename Grid>
// void print_grid(const Grid &grid, int n_rows, int n_cols) {
//     for (int i = 0; i < n_rows; ++i) {
//         for (int j = 0; j < n_cols; ++j)
//             printf("% 6.4f ", grid(i, j));
//         printf("\n");
//     }
//     printf("\n");
// }

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

    if (final_size > 100000) {
        printf("please choose a smaller final matrix size\n");
        return 1;
    }

    if (init_size >= final_size) {
        printf("initial size must be lower than final size\n");
        return 1;
    }

    using clk_t = std::chrono::steady_clock;
    int size = init_size;
    while (true) {
        Matrix f_vals(size, size), dfdy_vals(size, size);

        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                f_vals(i, j) = f(i, j);

        auto start = clk_t::now();
        estimate_dfdy(f_vals, dfdy_vals);
        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(
                clk_t::now() - start).count() / 1000.f;

        volatile float s = 0;
        for (int i =0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                s += dfdy_vals(i, j);

        printf("size %d time %.3fms\n", size, delta);

        if (size >= final_size)
            break;
        size = std::min(size * step, final_size);
    }

    // print_grid(f, N, M);
    // print_grid(dfdy_vals, N, M);
    // print_grid(dfdy, N, M);
}


