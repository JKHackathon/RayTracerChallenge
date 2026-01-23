#pragma once
#include "all_shapes.hpp"

#include <fstream>
#include <boost/algorithm/string.hpp>
#include <unordered_map>

struct ObjParser {
public:
    int ignored_lines;
    std::vector<Point> vertices;
    std::vector<Vector> normals;

    ObjParser() : ignored_lines(0) {
        vertices.push_back(Point(INFINITY, INFINITY, INFINITY));
        normals.push_back(Vector(INFINITY, INFINITY, INFINITY));
        groups.emplace("DefaultGroup", std::make_unique<Group>());
    }

    static ObjParser parse_obj_file(const char* filename);

    Group* get_default_group() const {
        return groups.at("DefaultGroup").get();
    }

    Group* get_group(std::string groupName) {
        return groups.at(groupName).get();
    }

    std::unique_ptr<Group> obj_to_group()&&;
private:
    // TODO: parser shouldnt own the group
    // std::unique_ptr<Group> default_group;
    std::unordered_map<std::string, std::unique_ptr<Group>> groups;
};