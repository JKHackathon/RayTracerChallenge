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
    }

private:
    Vector local_normal_at(const Point local_p) const override;
    IntersectionRecord local_intersect(const Ray local_r) const override;
};