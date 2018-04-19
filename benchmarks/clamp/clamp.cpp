#include <cstdio>
#include <vector>
#include <chrono>

const size_t COUNT = 5000000;

const size_t DATA_COUNT = 10000;

int startValue = 0;
size_t useDataCount = 1;
uint64_t res = 0;

std::vector<double> doubles(DATA_COUNT);
std::vector<float> floats(DATA_COUNT);
std::vector<uint64_t> ints64(DATA_COUNT);
std::vector<uint32_t> ints32(DATA_COUNT);
std::vector<int64_t> results(DATA_COUNT);

int idx(size_t i)
{
    return i % useDataCount;
}

void test() {
    double d = startValue;
    float f = startValue;
    uint64_t i64 = startValue;
    uint32_t i32 = startValue;

    for(size_t i = 0; i < COUNT; ++i) {
        doubles[idx(i)] = d += 0.25;
        floats[idx(i)] = f += 0.25f;
        ints64[idx(i)] = i64 += 1;
        ints32[idx(i)] = i32 += 1;
    }

    for(size_t i = 0; i < COUNT; ++i) {
        ints64[idx(i)] += doubles[idx(i)] * 0.25;
        ints32[idx(i)] += doubles[idx(i)] * 0.25;
        ints64[idx(i)] += floats[idx(i)] * 0.25f;
        ints32[idx(i)] += floats[idx(i)] * 0.25f;

        results[idx(i)] =
            ((doubles[idx(i)] * ints32[idx(i)]) + (floats[idx(i)] * ints32[idx(i)])) -
            ((doubles[idx(i)] * ints64[idx(i)]) + (floats[idx(i)] * ints64[idx(i)]));
    }

    for(size_t i = 0; i < COUNT; ++i) {
        results[idx(i)] =
            ((doubles[idx(i)] * ints32[idx(i)]) + (floats[idx(i)] * ints32[idx(i)])) -
            ((doubles[idx(i)] * ints64[idx(i)]) + (floats[idx(i)] * ints64[idx(i)]));

        doubles[idx(i)] = results[idx(i)];
        floats[idx(i)] = results[idx(i)];

        ints64[idx(i)] = doubles[idx(i)] * 0.25;
        ints32[idx(i)] = doubles[idx(i)] * 0.25;
        ints64[idx(i)] = floats[idx(i)] * 0.25f;
        ints32[idx(i)] = floats[idx(i)] * 0.25f;
    }

    res = 0;
    for(size_t i = 0; i < results.size(); ++i) {
        res += results[i];
    }
}

int main(int argc, char ** argv) {

    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::time_point<Clock> TimePoint;

    if (argc != 0) {
        startValue = argc;
        useDataCount = DATA_COUNT;
    }

    const int TEST_COUNTS = 100;
    uint32_t sum = 0;
    uint32_t min = 999999999;
    uint32_t max = 0;

    for (int i = 0; i < TEST_COUNTS; ++i) {

        TimePoint start = Clock::now();

        test();

        TimePoint end = Clock::now();

        uint32_t duration =
            std::chrono::duration_cast<std::chrono::milliseconds> (
                end - start).count();

        printf("step %d time: %dms res:%llu\n", i, duration, res);

        if(min > duration) min = duration;
        if(max < duration) max = duration;

        sum += duration;
    }

    printf("min: %dms; average: %dms; max: %dms\n", min, sum / TEST_COUNTS, max);

    return 0;
}