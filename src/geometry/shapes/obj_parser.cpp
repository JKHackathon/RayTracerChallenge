#include "obj_parser.hpp"

void fan_triangulation(std::vector<Point>& face_verts, std::vector<std::unique_ptr<Triangle>>& triangles) {
    // std::vector<std::unique_ptr<Triangle>> triangles;
    for (int i = 2; i < face_verts.size() - 1; i++) {
        triangles.push_back(std::make_unique<Triangle>(face_verts[1], face_verts[i], face_verts[i + 1]));
    }
}

void smooth_fan_triangulation(std::vector<Point>& face_verts, std::vector<Vector>& face_norms, std::vector<std::unique_ptr<Triangle>>& triangles) {
    for (int i = 2; i < face_verts.size() - 1; i++) {
        triangles.push_back(std::make_unique<SmoothTriangle>(face_verts[1], face_verts[i], face_verts[i + 1], face_norms[1], face_norms[i], face_norms[i + 1]));
    }
}

std::tuple<int, std::optional<int>, std::optional<int>> get_face_vertex_index(std::string face_index_group) {
    std::vector<std::string> index_group;
    boost::split(index_group, face_index_group, boost::is_any_of("/"), boost::token_compress_off);
    int v = std::stoi(index_group[0]);
    if (index_group.size() == 1) {
        return { v, std::nullopt, std::nullopt };
    }

    std::optional<int> vt;
    std::optional<int> vn;
    if (index_group[1] != "")
        vt = std::stoi(index_group[1]);
    if (index_group[2] != "")
        vn = std::stoi(index_group[2]);
    return { v, vt, vn };
}

ObjParser ObjParser::parse_obj_file(const char* filename) {
    ObjParser parser;
    Group* curr_mesh = parser.get_default_group();
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + std::string(filename) + "\n");
    }

    std::string line;
    while (getline(file, line)) {
        boost::trim(line); // TODO: boost functions are not the most efficient!!!
        std::vector<std::string> data;
        boost::split(data, line, boost::is_any_of(" "), boost::token_compress_on);

        std::string type = data[0];
        if (type == "g") {
            auto new_group_u = std::make_unique<Group>();
            curr_mesh = new_group_u.get();
            parser.groups.emplace(data[1], std::move(new_group_u));
            continue;
        }
        if (type == "v") {
            parser.vertices.push_back(Point(std::stod(data[1]),
                std::stod(data[2]),
                std::stod(data[3])));
            continue;
        }

        if (type == "vn") {
            parser.normals.push_back(Vector(std::stod(data[1]),
                std::stod(data[2]),
                std::stod(data[3])));
            continue;
        }

        if (type == "vt") {
            continue;
        }

        if (type == "f") {
            if (data.size() > 4) { // Polygon
                std::vector<Point> face_verts;
                std::vector<Vector> face_norms;
                // TODO: need garbage value to do 1-indexing (smarter: have index casting when creating faces)
                face_verts.push_back(Point(INFINITY, INFINITY, INFINITY));
                face_norms.push_back(Vector(INFINITY, INFINITY, INFINITY));
                for (int i = 1; i < data.size(); i++) {
                    const auto [v, vt, vn] = get_face_vertex_index(data[i]);
                    face_verts.push_back(parser.vertices[v]);
                    if (vn.has_value()) {
                        face_norms.push_back(parser.normals[vn.value()]);
                    }
                }
                std::vector<std::unique_ptr<Triangle>> triangles;
                if (face_norms.size() <= 1) {
                    fan_triangulation(face_verts, triangles);
                }
                else {
                    assert(face_norms.size() == face_verts.size());
                    smooth_fan_triangulation(face_verts, face_norms, triangles);
                }

                for (auto& triangle : triangles) {
                    curr_mesh->add_child(std::move(triangle));
                }
                continue;
            }

            const auto [v1, vt1, vn1] = get_face_vertex_index(data[1]);
            const auto [v2, vt2, vn2] = get_face_vertex_index(data[2]);
            const auto [v3, vt3, vn3] = get_face_vertex_index(data[3]);
            auto p1 = parser.vertices[v1];
            auto p2 = parser.vertices[v2];
            auto p3 = parser.vertices[v3];
            if (!vn1.has_value()) {
                curr_mesh->add_child(std::make_unique<Triangle>(p1, p2, p3));
                continue;
            }
            auto n1 = parser.normals[vn1.value()];
            auto n2 = parser.normals[vn2.value()];
            auto n3 = parser.normals[vn3.value()];
            curr_mesh->add_child(std::make_unique<SmoothTriangle>(p1, p2, p3, n1, n2, n3));
        }
        parser.ignored_lines++;
    }
    return parser;
}

// TODO: this doesnt make it clear that `groups` is no longer valid
// TODO: Need to throw error if used after this called (and set flag)
// rvalue-qualification prevents accidental usage
std::unique_ptr<Group> ObjParser::obj_to_group()&& {
    auto parent_group_u = std::make_unique<Group>();
    Group* parent_group = parent_group_u.get();
    for (auto& [name, group] : groups) {
        parent_group->add_child(std::move(group));
    }
    return parent_group_u;
}