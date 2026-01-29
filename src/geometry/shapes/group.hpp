#pragma once

#include "shapes.hpp"
#include <unordered_set>

struct ShapePtrHash {
    using is_transparent = void;

    size_t operator()(const std::unique_ptr<Shape>& p) const {
        return std::hash<const Shape*>{}(p.get());
    }

    size_t operator()(const Shape* p) const {
        return std::hash<const Shape*>{}(p);
    }
};

struct ShapePtrEqual {
    using is_transparent = void;

    bool operator()(const std::unique_ptr<Shape>& a,
        const std::unique_ptr<Shape>& b) const {
        return a.get() == b.get();
    }

    bool operator()(const std::unique_ptr<Shape>& a, const Shape* b) const {
        return a.get() == b;
    }
};

struct Group : public Shape {
    // TODO: should this be a normal ptr? Does the group own it or does the world?
    // std::unordered_set<std::unique_ptr<Shape>, ShapePtrHash, ShapePtrEqual> shapes;
    std::vector<std::unique_ptr<Shape>> shapes;

    void add_child(std::unique_ptr<Shape> shape) {
        shape.get()->parent = this;
        // shapes.emplace(std::move(shape));
        shapes.push_back(std::move(shape));
        invalidate_bb();
    }

    // Note: definitely overcomplicated, but still a fun experiment
    // Could also require at least one shape inputted
    template<typename... ShapePtrs>
    void add_children(ShapePtrs... shape) requires (std::derived_from< // child of base class Shape
        typename std::remove_cvref_t<ShapePtrs>::element_type, // element_type (ie unique_ptr, shared_ptr, etc)
        Shape
    > && ...) { // rvalue reference
        ((shape.get()->parent = this), ...);
        (shapes.push_back(std::move(shape)), ...);
        invalidate_bb();
    }

    BoundingBox bounds_of() const override;

    std::pair<std::vector<std::unique_ptr<Shape>>, std::vector<std::unique_ptr<Shape>>> partition_children();

    void make_subgroup(std::vector<std::unique_ptr<Shape>>& shapes) {
        auto sub_g_u = std::make_unique<Group>();
        Group* subgroup = sub_g_u.get();
        for (auto& shape : shapes) {
            subgroup->add_child(std::move(shape));
        }
        add_child(std::move(sub_g_u));
    }

    void divide(int min_children) override;

private:
    // Cached bounding box
    mutable BoundingBox bb;
    mutable bool bb_is_valid;

    Vector local_normal_at(const Point local_p, Intersection i) const override;
    IntersectionRecord local_intersect(const Ray local_r) const override;
    void invalidate_bb() const {
        bb_is_valid = false;
    }
};