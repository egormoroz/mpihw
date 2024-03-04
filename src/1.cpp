#include <thread>
#include <mutex>
#include <vector>
#include <cstdio>

static std::mutex g_io_mtx;

void thread_routine(int thread_idx, int n_threads) {
    {
        std::lock_guard<std::mutex> lck(g_io_mtx);
        printf("hello from thread %d / %d\n", thread_idx + 1, n_threads);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("expected 1 argument: number of threads\n");
        printf("usage: <binary> <num_threads>\n");
        return 1;
    }

    int n_threads = atol(argv[1]);
    if (n_threads <= 0) {
        printf("number of threads must be a positive integer\n");
        return 1;
    }

    std::vector<std::thread> threads(n_threads);
    for (int i = 0; i < n_threads; ++i) {
        threads[i] = std::thread([i, n_threads] { thread_routine(i, n_threads); });
    }

    for (auto &th: threads)
        th.join();

    printf("all threads finished working\n");

    return 0;
}

