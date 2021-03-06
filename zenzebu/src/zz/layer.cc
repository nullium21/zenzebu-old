#ifndef ZZ_USEPCH
#include "zz/log.h"
#endif

#include "zz/ecs.h"
#include "zz/layer.h"

using namespace zz;
using std::make_shared;

string layer::name() {
    return "unknown layer";
}

void layer::on_add(layer_stack &stack) {
    ZZ_CORE_INFO("added layer {0} to stack", name());
}

void layer::on_remove(layer_stack &stack) {
    ZZ_CORE_INFO("removed layer {0} from stack", name());
}

void layer::on_update(layer_stack &stack) {

}

layer_stack::~layer_stack() {
    for (auto ptr : layers)
        delete ptr;
}

uint layer_stack::push(layer *l) {
    uint idx = layers.size();
    layers.push_back(l);

    uint id = next_id++;

    id_to_index[id] = idx;

    l->on_add(*this);

    return id;
}

layer *layer_stack::pop(uint id) {
    if (id_to_index.find(id) == id_to_index.end())
        return nullptr;

    uint idx = id_to_index[id];
    layer *ret = layers[idx];

    layers.erase(layers.begin() + idx); // remove layer by index
    id_to_index.erase(id);

    ret->on_remove(*this);

    return ret;
}

layer *layer_stack::pop(string lname) {
    for (auto it = layers.begin(); it != layers.end(); it++) {
        layer *ptr = *it;

        if (ptr->name() == lname) {
            it = layers.erase(it);
            ptr->on_remove(*this);
            return ptr;
        }
    }
    return nullptr;
}

bool layer_stack::has(string lname) {
    for (auto it = layers.begin(); it != layers.end(); it++) {
        layer *ptr = *it;

        if (ptr->name() == lname)
            return true;
    }
    return false;
}

void layer_stack::update() {
    for (layer *l : layers)
        l->on_update(*this);
}

void layering::update_all() {
    auto view = ecs::entt()->view<layer_stack>();
    view.each([](auto &stack) {
        stack.update();
    });
}
