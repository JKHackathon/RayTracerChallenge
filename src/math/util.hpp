#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>

const double EPSILON = 0.0001;
const double MIN_DOUBLE = std::numeric_limits<double>::lowest();
const double MAX_DOUBLE = std::numeric_limits<double>::max();

inline bool double_equal(double a, double b) { return std::fabs(a - b) < EPSILON; }

#endif