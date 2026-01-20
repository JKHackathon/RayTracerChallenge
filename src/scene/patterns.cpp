#include "patterns.hpp"

#include "../geometry/shapes/shapes.hpp"

Color Pattern::pattern_at_shape(const Shape* object, Point p_world) const {
    Point p_obj = object->world_to_object(p_world);
    Point p_pattern = transform.inverse() * p_obj;
    return pattern_at(p_pattern);
}