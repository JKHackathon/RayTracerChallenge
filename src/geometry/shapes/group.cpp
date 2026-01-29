#include "group.hpp"
#include "shapes.hpp"

BoundingBox Group::bounds_of() const {
    if (bb_is_valid) { // cache group bb
        return bb;
    }

    bb = BoundingBox();

    for (auto& shape : shapes) {
        BoundingBox child_box = shape.get()->parent_space_bounds_of();
        bb.add_BB(child_box);
    }
    bb_is_valid = true;
    return bb;
}

Vector Group::local_normal_at(const Point local_p, Intersection i) const {
    throw std::runtime_error("Group local_normal_at shouold not be used!\n");
}

IntersectionRecord Group::local_intersect(const Ray local_r) const {
    IntersectionRecord xs;
    if (!bounds_of().intersects(local_r)) {
        return IntersectionRecord();
    }

    for (const auto& shape : shapes) {
        xs.append_record(shape.get()->intersect(local_r));
    }
    std::sort(xs.intersections.begin(), xs.intersections.end(), [](Intersection a, Intersection b) {
        return a.t < b.t;
        });
    return xs;
}

std::pair<std::vector<std::unique_ptr<Shape>>, std::vector<std::unique_ptr<Shape>>> Group::partition_children() {
    auto [left_bb, right_bb] = bounds_of().split_bounds();
    std::vector<std::unique_ptr<Shape>> left_group;
    std::vector<std::unique_ptr<Shape>> right_group;

    for (auto& shape : shapes) {
        auto child_bb = shape.get()->bounds_of().transform(shape.get()->transform);
        if (left_bb.contains_bb(child_bb)) {
            left_group.push_back(std::move(shape));
        }
        else if (right_bb.contains_bb(child_bb)) {
            right_group.push_back(std::move(shape));
        }
    }
    // Cleanup original group
    std::erase_if(shapes, [](auto const& shape) { return !shape;});
    return { std::move(left_group), std::move(right_group) };
}

// TODO: all the creating and moving is going to be so slow...
void Group::divide(int min_children) {
    if (min_children <= shapes.size()) {
        auto [left, right] = partition_children();
        if (!left.empty()) {
            make_subgroup(left);
        }
        if (!right.empty()) {
            make_subgroup(right);
        }
    }

    for (auto& shape : shapes) {
        shape.get()->divide(min_children);
    }
}