#include "obj_parser.hpp"

std::vector<std::unique_ptr<Triangle>> fan_triangulation(std::vector<Point>& face_verts) {
    std::vector<std::unique_ptr<Triangle>> triangles;
    for (int i = 2; i < face_verts.size() - 1; i++) {
        triangles.push_back(std::make_unique<Triangle>(face_verts[1], face_verts[i], face_verts[i + 1]));
    }
    return triangles;
}

int get_face_vertex_index(std::string face_index_group) {
    std::vector<std::string> index_group;
    boost::split(index_group, face_index_group, boost::is_any_of("/"), boost::token_compress_off);
    return std::stoi(index_group[0]);
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
            continue;
        }

        if (type == "vt") {
            continue;
        }

        if (type == "f") {
            if (data.size() > 4) { // Polygon
                std::vector<Point> face_verts;
                // TODO: need garbage value to do 1-indexing (smarter: have index casting when creating faces)
                face_verts.push_back(Point(INFINITY, INFINITY, INFINITY));
                for (int i = 1; i < data.size(); i++) {

                    face_verts.push_back(parser.vertices[get_face_vertex_index(data[i])]);
                }
                auto triangles = fan_triangulation(face_verts);
                for (auto& triangle : triangles) {
                    curr_mesh->add_child(std::move(triangle));
                }
                continue;
            }

            auto p1 = parser.vertices[get_face_vertex_index(data[1])];
            auto p2 = parser.vertices[get_face_vertex_index(data[2])];
            auto p3 = parser.vertices[get_face_vertex_index(data[3])];
            curr_mesh->add_child(std::make_unique<Triangle>(p1, p2, p3));
            continue;
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