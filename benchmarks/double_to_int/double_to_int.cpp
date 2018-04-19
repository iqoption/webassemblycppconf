
#include <cstdio>
#include <limits>
#include <cmath>
#include <vector>
#include <chrono>

template<typename IntType, typename FloatType>
bool cast_src_check_bounds(FloatType src, std::false_type)
{
    return !(src < FloatType(0) || src >= FloatType(std::numeric_limits<IntType>::max()));
}

template<typename IntType, typename FloatType>
bool cast_src_check_bounds(FloatType src, std::true_type)
{
    return !(src <= FloatType(std::numeric_limits<IntType>::min()) || src >= FloatType(std::numeric_limits<IntType>::max()));
}

template<typename IntType, typename FloatType>
__attribute__ ((optnone))
bool try_int_cast(FloatType src, IntType& result, IntType defaultValue = 0)
{
    static_assert(std::is_integral<IntType>::value, "IntType must be integral");
    static_assert(std::is_floating_point<FloatType>::value, "FloatType must be floating point");

    if (!std::isfinite(src))
    {
        result = defaultValue;
        return false;
    }

    if (cast_src_check_bounds<IntType, FloatType>(src,
            typename std::conditional<std::is_signed<IntType>::value, std::true_type, std::false_type>::type()))
    {
        result = IntType(src);
        return true;
    }

    result = defaultValue;
    return false;
};

const size_t COUNT = 20000000;

std::vector<double> doubles(COUNT);
std::vector<int> ints32(COUNT);

void test() {
    double d = 0.0;

    for(size_t i = 0; i < COUNT; ++i) {
        doubles[i] = d += 0.25;
    }

    for(size_t i = 0; i < COUNT; ++i) {
#ifdef SAFECAST
        try_int_cast(doubles[i], ints32[i]);
#else
        ints32[i] = doubles[i];
#endif
    }
}

int main(int argc, char ** argv) {
#ifdef SAFECAST
    printf("safe cast...\n");
#else
    printf("unsafe cast...\n");
#endif

    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::time_point<Clock> TimePoint;

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

        printf("step %d time: %dms\n", i, duration);

        if(min > duration) min = duration;
        if(max < duration) max = duration;

        sum += duration;
    }

    printf("min: %dms; average: %dms; max: %dms\n", min, sum / TEST_COUNTS, max);

    return 0;
}
