#pragma once

#include "../math/color.hpp"
#include "../math/transformations.hpp"
#include "../math/tuples.hpp"

struct Shape;

struct Pattern {
   public:
    Transform transform;
    Color pattern_at_shape(const Shape* object, Point p_world) const;

   private:
    virtual Color pattern_at(Point p_pattern) const = 0;
};

struct TestPattern : public Pattern {
    Color pattern_at(Point p_pattern) const override {
        return Color(p_pattern.x, p_pattern.y, p_pattern.z);
    }
};

struct StripePattern : public Pattern {
    Color a;
    Color b;

    StripePattern(Color a, Color b) : a(a), b(b) {}

    // TODO: make this private and fix tests
    Color stripe_at(Point p) const {
        return int(std::floor(p.x)) % 2 == 0 ? a : b;
    }

   private:
    Color pattern_at(Point p_pattern) const override {
        return stripe_at(p_pattern);
    }
};

struct GradientPattern : public Pattern {
    Color a;
    Color b;

    GradientPattern(Color a, Color b) : a(a), b(b) {}

   private:
    Color pattern_at(Point p_pattern) const override {
        return a + (b - a) * (p_pattern.x - floorf(p_pattern.x));
    }
};

struct RingPattern : public Pattern {
    Color a;
    Color b;

    RingPattern(Color a, Color b) : a(a), b(b) {}

   private:
    Color pattern_at(Point p_pattern) const override {
        return int(floor(sqrt(pow(p_pattern.x, 2) + pow(p_pattern.z, 2)))) %
                           2 ==
                       0
                   ? a
                   : b;
    }
};

struct CheckerPattern : public Pattern {
    Color a;
    Color b;

    CheckerPattern(Color a, Color b) : a(a), b(b) {}

   private:
    Color pattern_at(Point p_pattern) const override {
        return int(floor(p_pattern.x) + floor(p_pattern.y) + floor(p_pattern.z)) % 2 == 0
                   ? a
                   : b;
    }
};