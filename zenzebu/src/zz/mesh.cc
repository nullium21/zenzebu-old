#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include "mesh.h"
#include <unordered_map>

using namespace zz;
using namespace zz::component;

size_t mesh_buffer::num_faces() {
    return num_tris() / 3;
}

size_t mesh_buffer::num_tris() {
    return idxs.size() / 3;
}

float *mesh_buffer::buffer() {
    float *buf = new float[idxs.size()];

    for (size_t j = 0; j < idxs.size(); ++j) {
        buf[j] = verts[idxs[j]];
    }

    return buf;
}

glm::vec3 mesh_buffer::operator[](size_t i) {
    auto idxs_iter = idxs.begin() + (i * 3);

    return {
        verts[idxs_iter[0]],
        verts[idxs_iter[1]],
        verts[idxs_iter[2]]
    };
}

mesh::mesh(std::vector<glm::vec3> verts) {
    std::unordered_map<glm::vec3, size_t> idxs = {};
    for (int i = 0; i < verts.size(); ++i) {
        auto iter = idxs.find(verts[i]);
        if (iter != idxs.end())
            indices.push_back((*iter).second);
        else {
            verts.push_back(verts[i]);
            indices.push_back(verts.size()-1);
        }
    }
}

mesh::mesh(mesh_buffer buf) {
    indices = buf.idxs;
    for (int i = 0; i < buf.num_tris(); i++) {
        verts.push_back(buf[i]);
    }
}

mesh_buffer mesh::buffer() {
    std::vector<float> floats {};

    for (glm::vec3 vert : verts) {
        floats.push_back(vert.x);
        floats.push_back(vert.y);
        floats.push_back(vert.z);
    }

    return {
        floats, indices
    };
}
