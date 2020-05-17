#include "CTime.h"

void CTime::reset()
{
    savedPoint = std::chrono::high_resolution_clock::now();
}

float CTime::timeElapsed()
{
    return  std::chrono::duration_cast<std::chrono::duration<float>>(now() - savedPoint).count();
    //return std::chrono::duration_cast<std::chrono::microseconds> (std::chrono::high_resolution_clock::now() - savedPoint).count();
}

std::chrono::high_resolution_clock::time_point CTime::now()
{
    return std::chrono::high_resolution_clock::now();
}
