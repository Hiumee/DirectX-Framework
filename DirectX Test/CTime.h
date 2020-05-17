#pragma once

#include <chrono>

class CTime
{
// TODO
private:
    std::chrono::high_resolution_clock::time_point savedPoint;
public:
    void reset();
    float timeElapsed();
    std::chrono::high_resolution_clock::time_point now();
};