#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>

const float EPSILON = 0.0001;

inline bool float_equal(float a, float b) { return std::fabs(a - b) < EPSILON; }

#endif