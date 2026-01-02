#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>

const double EPSILON = 0.0001;

inline bool double_equal(double a, double b) { return std::fabs(a - b) < EPSILON; }

#endif