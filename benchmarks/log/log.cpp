#include <cstdio>
#include <vector>
#include <chrono>

#include <cmath>
#include <iomanip>
#include <vector>
using namespace std;

const size_t COUNT = 1000000;

double test(double p, double x)
{
    double count = 1;     //Keeps track of the count in the loop
    double totalValue = 0; //The summation of each polynomial evaluated
   bool reciprocal = false;   //Flag to use if greater than 2
    double precision = p;     //The the highest order of polynomial to use.
    //double x;             //Value to evaluate to ln for.

   if (x > 2.0)   //Use the rule -ln(1/x) == ln(x) to keep accuracy               
   {
      x = 1 / x;        //Change to using 1/x rather than x
      reciprocal = true; //Flag as true (sign change is later)
   }
   
   while (count < precision)
   {
      totalValue += pow(-1, count + 1) * (pow((x-1),count)/count);
      count++;
   } 
   
   if (reciprocal == true)
      totalValue *= -1;   //If reciprocal was used multiply by -1 to change sign

   return totalValue;
}

int main(int argc, char ** argv) {

    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::time_point<Clock> TimePoint;

    printf("argc %d\n", argc);

    const int TEST_COUNTS = 100;
    uint32_t sum = 0;
    uint32_t min = 999999999;
    uint32_t max = 0;

    double res = (argc + argc) * COUNT;

    for (int i = 0; i < TEST_COUNTS; ++i) {

        TimePoint start = Clock::now();

        res = test(argc * COUNT, res * COUNT);

        TimePoint end = Clock::now();

        uint32_t duration =
            std::chrono::duration_cast<std::chrono::milliseconds> (
                end - start).count();

        printf("step %d time: %dms result:%f\n", i, duration, res);

        if(min > duration) min = duration;
        if(max < duration) max = duration;

        sum += duration;
    }

    printf("min: %dms; average: %dms; max: %dms\n", min, sum / TEST_COUNTS, max);

    return 0;
}