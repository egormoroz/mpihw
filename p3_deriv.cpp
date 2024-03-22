#include <mpi/mpi.h>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <vector>

#include "matview.hpp"

float f(float x, float y) {
    return sin(y * exp(-x));
}

float _dfdy(float x, float y) {
    float t = exp(-x);
    return t * cos(y * t);
}

void estimate_dfdy(const MatrixView &f, MatrixView &dfdy) {
    int n_rows = f.n_rows, n_cols = f.n_cols;

    for (int i = 0; i < n_rows; ++i)
        dfdy(i, 0) = (f(i, 1) - f(i, 0)) / 1;
    for (int i = 0; i < n_rows; ++i)
        dfdy(i, n_cols - 1) = (f(i, n_cols - 1) - f(i, n_cols - 2)) / 1;

    for (int i = 0; i < n_rows; ++i)
        for (int j = 1; j < n_cols - 1; ++j)
            dfdy(i, j) = (f(i, j + 1) - f(i, j - 1)) / 2;
}

void print_matrix(const MatrixView &m) {
    for (int i = 0; i < m.n_rows; ++i) {
        for (int j = 0; j < m.n_cols; ++j)
            printf("% 6.4f ", m(i, j));
        printf("\n");
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


    assert(argc == 2);
    const int n = atoi(argv[1]);
    const int n_block_rows = n / world_size;
    const int block_size = n_block_rows * n;
    assert(n > 0);
    assert(n % world_size == 0);

    std::vector<float> f_buf, dfdy_buf;
    std::vector<float> f_block_buf(block_size);
    std::vector<float> dfdy_block_buf(block_size);

    if (world_rank == 0) {
        f_buf.resize(n * n);
        MatrixView view(f_buf.data(), n, n);
        for (int x = 0; x < n; ++x)
            for (int y = 0; y < n; ++y)
                view(x, y) = f(x, y);

        dfdy_buf.resize(n * n);
    }

    double start = MPI_Wtime();

    MPI_Scatter(f_buf.data(), block_size, MPI_FLOAT, 
                f_block_buf.data(), block_size, MPI_FLOAT,
                0, MPI_COMM_WORLD);

    MatrixView f_slice(f_block_buf.data(), n_block_rows, n);
    MatrixView dfdy_slice(dfdy_block_buf.data(), n_block_rows, n);
    estimate_dfdy(f_slice, dfdy_slice);

    MPI_Gather(dfdy_block_buf.data(), block_size, MPI_FLOAT, 
                dfdy_buf.data(), block_size, MPI_FLOAT,
                0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (world_rank == 0) {
        /* MatrixView f_view(f_buf.data(), n, n), df_view(dfdy_buf.data(), n, n); */

        /* for (int x = 0; x < n; ++x) */
        /*     for (int y = 0; y < n; ++y) */
        /*         f_view(x, y) = _dfdy(x, y); */

        /* print_matrix(f_view); */
        /* printf("\n"); */
        /* print_matrix(df_view); */

        printf("took %fs\n", end - start);
    }

    MPI_Finalize();
}

