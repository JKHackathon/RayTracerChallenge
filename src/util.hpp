#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>

const float EPSILON = 0.00001;
 
bool float_equal(double a, double b) {
    return std::fabs(a - b) < EPSILON;
}

#endif