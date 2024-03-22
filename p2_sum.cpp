#include <mpi/mpi.h>
#include <cstdio>
#include <cassert>

#include <random>
#include <vector>

std::vector<int> create_array(int n) {
    std::default_random_engine rng(0xdeadbeef);
    std::uniform_int_distribution<int> dist(-1000, 1000);

    std::vector<int> v(n);
    for (int &i: v)
        i = dist(rng);

    return v;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    assert(argc == 2);
    const int n = atoi(argv[1]);
    const int block_size = n / world_size;
    assert(n > 0);
    assert(n % world_size == 0);

    std::vector<int> v_all, v(block_size);
    std::vector<long long> sub_sums(world_rank == 0 ? world_size : 0);

    if (world_rank == 0)
        v_all = create_array(n);

    double start = MPI_Wtime();

    MPI_Scatter(v_all.data(), block_size, MPI_INT, v.data(), block_size, MPI_INT, 0, MPI_COMM_WORLD);

    long long sub_sum = std::accumulate(v.begin(), v.end(), 0ll);

    MPI_Gather(&sub_sum, 1, MPI_LONG_LONG, sub_sums.data(), 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (world_rank == 0) {
        long long sum = std::accumulate(sub_sums.begin(), sub_sums.end(), 0);
        printf("result % 8lld took %.6fs\n", sum, end - start);
    }


    MPI_Finalize();
}

