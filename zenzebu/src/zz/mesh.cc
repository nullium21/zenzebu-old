#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include "mesh.h"
#include <unordered_map>

using namespace zz;
using namespace zz::component;

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

    invalidate();
}

float *mesh::vert_buffer() {
    float *buf = new float[verts.size() * 3];
    for (size_t i = 0; i < verts.size(); i++) {
        buf[i*3+0] = verts[i].x;
        buf[i*3+1] = verts[i].y;
        buf[i*3+2] = verts[i].z;
    }
    return buf;
}

size_t *mesh::idx_buffer() {
    return indices.data();
}

float *mesh::vert_buffer_all() {
    float *buf = new float[indices.size() * 3];
    for (size_t i = 0; i < indices.size(); i++) {
        glm::vec3 vert = verts[indices[i]];
        buf[i*3+0] = vert.x;
        buf[i*3+1] = vert.y;
        buf[i*3+2] = vert.z;
    }
    return buf;
}

void mesh::invalidate() {
    vao = vbo = -1;
}

bool mesh::should_create_buffers_on_init() {
    switch (type) {
        case mesh::draw_static:
        case mesh::draw_dynamic:
            return true;

        default: return false;
    }
}
