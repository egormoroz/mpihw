#include <cstdio>

#include <vector>
#include <random>
#include <chrono>

using clk_t = std::chrono::steady_clock;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("expected 3 arguments: initial size, final size and multiplicative step\n");
        printf("usage: <binary> <init_size> <final_size> <mult_step>\n");
        return 1;
    }

    long long init_size = atoll(argv[1]);
    long long final_size = atoll(argv[2]);
    long long step = atoll(argv[3]);

    if (init_size <= 0) {
        printf("initial size must be a positive integer\n");
        return 1;
    }
    if (init_size >= final_size) {
        printf("initial size must be lower than final size\n");
        return 1;
    }
    if (step <= 0) {
        printf("step must be a positive integer\n");
        return 1;
    }

    if (final_size > INT64_MAX / step) {
        printf("integer overflow possible, consider reducing final size or step\n");
        return 1;
    }

    long long gbs = final_size / (1024*1024*1024) * 4;
    if (gbs >= 16) {
        printf("you probably don\'t want to allocate %lld GB\n", gbs);
        return 1;
    }

    std::vector<int> a(final_size);
    std::default_random_engine rng(1337);
    std::uniform_int_distribution<int> dist(-10000, 10000);

    printf("generating random array with %lld elements...\n", final_size);
    for (auto &i: a)
        i = dist(rng);

    long long size = init_size;
    while (true) {
        auto start = clk_t::now();

        volatile long long s = 0;
        for (long long i = 0; i < size; ++i)
            s += a[i];

        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(
                clk_t::now() - start).count() / 1000.f;
        printf("size %lld time %.3fms\n", size, delta);

        if (size >= final_size)
            break;
        size = std::min(size * step, final_size);
    }

    return 0;
}

