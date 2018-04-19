
#include <cstdio>
#include <limits>
#include <cmath>
#include <vector>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

const int TEST_COUNTS = 100;
const size_t COUNT = 10000000;

std::vector<glm::ivec3> pointsInt(COUNT);
std::vector<glm::ivec3> pointsInt2(COUNT);
std::vector<glm::ivec3> pointsInt3(COUNT);

void test(int step)
{
    const float stepS = step / float(TEST_COUNTS);
    const float width = 800.f;
    const float height = 600.f;
    const float aspect = width / height;
    const glm::mat4 proj = glm::perspective(60.f, aspect, 10.f, 10000.f);
    const glm::mat4 view = glm::lookAt(glm::vec3(100.f + stepS, 20.f, 100.f + stepS), glm::vec3(), glm::vec3(0.f, 0.f, 1.f));
    const glm::vec4 viewport = glm::vec4(0.f, 0.f, width, height);

    for (size_t i = 0; i < COUNT; ++i)
    {
        const float s = i / float(COUNT);
        const glm::vec3 point = glm::vec3(100.f * s, 200.f * s, 50.f * s);
        const glm::vec3 projPoint = glm::project(point, view, proj, viewport);

        if (projPoint.x > 350.f && projPoint.y < 330.f)
        {
#ifdef SAFECAST
            {
                glm::ivec3& v1 = pointsInt[i];
                try_int_cast(projPoint.x, v1.x);
                v1.y = projPoint.y;
                v1.z = projPoint.z;
            }
#else
            pointsInt[i] = glm::ivec3(int(projPoint.x), int(projPoint.y), int(projPoint.z));
#endif
            pointsInt2[i] = glm::ivec3(int(projPoint.x * 0.5f), int(projPoint.y * 0.5f), int(projPoint.z * 0.5f));
            pointsInt3[i] = glm::ivec3(int(projPoint.x * 0.25f), int(projPoint.y * 0.25f), int(projPoint.z * 0.25f));
        }
    }
}

int main(int argc, char ** argv) {
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::time_point<Clock> TimePoint;

    uint32_t sum = 0;
    uint32_t min = 999999999;
    uint32_t max = 0;

#ifdef SAFECAST
    printf("safe cast...\n");
#else
    printf("unsafe cast...\n");
#endif

    for (int i = 0; i < TEST_COUNTS; ++i) {

        TimePoint start = Clock::now();

        test(i);

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


