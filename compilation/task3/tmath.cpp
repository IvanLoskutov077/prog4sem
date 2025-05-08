#include "tmath.hpp"

template<typename T>
T sum(T x, T y)
{
    return x + y;
}

template<typename T>
T mult(T x, T y)
{
    return x * y;
}

template int sum<int>(int, int);
template unsigned int sum<unsigned int>(unsigned int, unsigned int);
template float sum<float>(float, float);
template double sum<double>(double, double);

template int mult<int>(int, int);
template unsigned int mult<unsigned int>(unsigned int, unsigned int);
template float mult<float>(float, float);
template double mult<double>(double, double);