#include "cascade/core/time.hpp"
#include <iostream>
#include <iomanip>

using namespace cascade;

void testTime()
{
    std::cout << "=== Testing Time ===";

    Time varTime;
    varTime.start();

    std::cout << "Variable time step mode:\n";
    for (int i = 0; i < 5; ++i)
    {
        float dt = varTime.update();
        std::cout << "\tFrame " << varTime.getFrameCount()
                  << "\t: dt = " << std::fixed << std::setprecision(6) << dt << " s"
                  << "\t, elapsed = " << varTime.getElapsedTime() << " s";
    }
    std::cout << "\n";
}