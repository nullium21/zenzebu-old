#pragma once

using std::string;
using std::shared_ptr;
using std::unordered_map;
using std::vector;

namespace zz {
    struct layer_stack;

    struct layer {
        virtual string name();

        virtual void on_add(layer_stack &);
        virtual void on_remove(layer_stack &);
        virtual void on_update();
    };

    struct layer_stack {
        ~layer_stack();

        uint push(layer);

        layer *pop(uint);
        layer *pop(string);

        bool has(string);
    private:
        vector<layer *> layers{};
        uint next_id = 0;
        unordered_map<uint, uint> id_to_index{};
    };
}
