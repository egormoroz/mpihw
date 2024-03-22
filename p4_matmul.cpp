#include <mpi/mpi.h>
#include <cstdio>
#include <cassert>
#include <vector>
#include <random>

#include "matview.hpp"

void matmul(const MatrixView &a, const MatrixView &b, MatrixView &c) {
    constexpr int a_unroll = 8;
    float a_chunk[a_unroll];
    assert(a.n_cols % a_unroll == 0);

    for (int k = 0; k < a.n_cols; k += a_unroll) {
        for (int i = 0; i < a.n_rows; ++i) {
            memcpy(a_chunk, &a(i, k), sizeof(a_chunk));

            for (int j = 0; j < b.n_cols; ++j)
                for (int k0 = 0; k0 < a_unroll; ++k0)
                    c(i, j) += a_chunk[k0] * b(k + k0, j);
        }
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

    std::vector<float> a_data, b_data(n*n), c_data;
    std::vector<float> a_slice(block_size);
    std::vector<float> c_slice(block_size);
    if (world_rank == 0) {
        a_data.resize(n * n);
        c_data.resize(n * n, 0);

        std::default_random_engine rng(0xdeadbeef);
        std::uniform_real_distribution<float> dist(-1.f, 1.f);
        for (float &i: a_data) i = dist(rng);
        for (float &i: b_data) i = dist(rng);
    }

    double start = MPI_Wtime();


    MPI_Bcast(b_data.data(), n * n, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Scatter(a_data.data(), block_size, MPI_FLOAT, 
                a_slice.data(), block_size, MPI_FLOAT,
                0, MPI_COMM_WORLD);

    MatrixView c_slice_view(c_slice.data(), n_block_rows, n);
    matmul(MatrixView(a_slice.data(), n_block_rows, n), MatrixView(b_data.data(), n, n), c_slice_view);

    MPI_Gather(c_slice.data(), block_size, MPI_FLOAT, 
                c_data.data(), block_size, MPI_FLOAT,
                0, MPI_COMM_WORLD);


    double end = MPI_Wtime();

    if (world_rank == 0) {
        float trace = 0.f;
        MatrixView c(c_data.data(), n, n);
        for (int i = 0; i < n; ++i)
            trace += c(i, i);
        printf("trace % 6.4f took %.6fs\n", trace, end - start);
    }

    MPI_Finalize();
}

