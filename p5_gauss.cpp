#include <mpi/mpi.h>
#include <cstdio>
#include <cassert>
#include <numeric>
#include <vector>
#include "matview.hpp"


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
    assert(n > 0);
    assert(n % world_size == 0);

    const int n_rows = n;
    const int n_cols = n + 1;
    const int n_block_rows = n_rows / world_size;
    const int block_size = n_block_rows * n_cols;

    const int start_row = n_block_rows * world_rank;
    const int end_row   = n_block_rows * (world_rank + 1);

    std::vector<float> a_buffer;
    if (world_rank == 0) {
        a_buffer.resize(n_rows * n_cols, 1);
        MatrixView a(a_buffer.data(), n_rows, n_cols);
        for (int i = 0; i < n_rows; ++i) {
            a(i, i) = n;
            a(i, n_cols - 1) = i;
        }

        /* print_matrix(MatrixView(a_buffer.data(), n_rows, n_cols)); */
    }

    std::vector<float> pivot_row(n_cols);
    std::vector<float> block_buffer(block_size);

    double start = MPI_Wtime();

    MPI_Scatter(a_buffer.data(), block_size, MPI_FLOAT,
                block_buffer.data(), block_size, MPI_FLOAT, 
                0, MPI_COMM_WORLD);

    MatrixView a_block(block_buffer.data(), n_block_rows, n_cols);

    std::vector<MPI_Request> requests(world_size);

    for (int row = 0; row < end_row; ++row) {
        const int mapped_rank = row / n_block_rows;

        if (mapped_rank == world_rank) {
            const int local_row = row % n_block_rows;
            const float pivot = a_block(local_row, row);
            for (int col = row; col < n_cols; ++col)
                a_block(local_row, col) /= pivot;

            for (int i = mapped_rank + 1; i < world_size; ++i)
                MPI_Isend(&a_block(local_row, 0), n_cols, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &requests[i]);

            for (int elim_row = local_row + 1; elim_row < n_block_rows; ++elim_row) {
                const float t = a_block(elim_row, row);
                for (int col = row; col < n_cols; ++col)
                    a_block(elim_row, col) -= t * a_block(local_row, col);
            }
        } else {
            MPI_Recv(pivot_row.data(), n_cols, MPI_FLOAT, mapped_rank, 0, 
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int elim_row = 0; elim_row < n_block_rows; ++elim_row) {
                const float t = a_block(elim_row, row);
                for (int col = row; col < n_cols; ++col)
                    a_block(elim_row, col) -= t * pivot_row[col];
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    for (int row = n_rows - 1; row >= start_row; --row) {
        const int mapped_rank = row / n_block_rows;

        if (mapped_rank == world_rank) {
            const int local_row = row % n_block_rows;

            for (int i = mapped_rank - 1; i >= 0; --i)
                MPI_Isend(&a_block(local_row, n_cols - 1), 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &requests[i]);

            for (int sub_row = 0; sub_row < local_row; ++sub_row) {
                a_block(sub_row, n_cols - 1) -= a_block(sub_row, row) * a_block(local_row, n_cols - 1);
                a_block(sub_row, row) = 0;
            }

        } else {
            float x;
            MPI_Recv(&x, 1, MPI_FLOAT, mapped_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int sub_row = 0; sub_row < n_block_rows; ++sub_row) {
                a_block(sub_row, n_cols - 1) -= a_block(sub_row, row) * x;
                a_block(sub_row, row) = 0;
            }
        }
    }

    double end = MPI_Wtime();

    MPI_Gather(block_buffer.data(), block_size, MPI_FLOAT, 
               a_buffer.data(), block_size, MPI_FLOAT, 
               0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        float sum = std::accumulate(a_buffer.begin(), a_buffer.end(), 0.f);

        /* print_matrix(MatrixView(a_buffer.data(), n_rows, n_cols)); */

        printf("sum % 6f took %fs\n", sum, end - start);
    }

    MPI_Finalize();
}

