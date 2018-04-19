
#include <cstdio>
#include <chrono>
#include <string>

bool testFile(int i)
{
    char fileIndex[2] = {};
    sprintf (fileIndex, "%d", i);

    FILE* f = fopen(fileIndex, "rb");
    if (!f)
    {
        printf("fopen error!\n");
        return false;
    }
    fseek(f, 0, SEEK_END);
    const long size = ftell(f);
    rewind(f);

    std::string buffer;
    buffer.resize(size);

    const size_t readed = fread(&buffer[0], size, 1, f);
    fclose(f);

    if (buffer.empty())
    {
        printf("buffer error!\n");
        return false;
    }
    return true;
}


const size_t COUNT = 50;
const int FILES_COUNT = 1000;

bool test() {
    for (size_t i = 0; i < COUNT; ++i)
    {
        for (int j = 0; j < FILES_COUNT; ++j)
        {
            if (!testFile(j))
                return false;
        }
    }
    return true;
}

int main(int argc, char ** argv) {
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::time_point<Clock> TimePoint;

    const int TEST_COUNTS = 100;
    uint32_t sum = 0;
    uint32_t min = 999999999;
    uint32_t max = 0;

    for (int i = 0; i < TEST_COUNTS; ++i) {

        TimePoint start = Clock::now();

        if (!test())
            break;

        TimePoint end = Clock::now();

        uint32_t duration =
            std::chrono::duration_cast<std::chrono::milliseconds> (
                end - start).count();

        printf("step %d time: %dms\n", i, duration);

        sum += duration;

        if(min > duration) min = duration;
        if(max < duration) max = duration;
    }

    printf("min: %dms; average: %dms; max: %dms\n", min, sum / TEST_COUNTS, max);

    return 0;
}
