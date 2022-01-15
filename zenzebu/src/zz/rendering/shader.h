#pragma once

#include <string>
#include <istream>

namespace zz::render {
    class shader {
    public:
        shader(std::string &vert, std::string &frag);
        shader(std::istream &vert, std::istream &frag);

        bool is_initialized();

        ~shader();

        void use();

        void uniform(std::string name, float x);
        void uniform(std::string name, int x);
        void uniform(std::string name, uint x);

        void uniform(std::string name, float x, float y);
        void uniform(std::string name, int x, int y);
        void uniform(std::string name, uint x, uint y);

        void uniform(std::string name, float x, float y, float z);
        void uniform(std::string name, int x, int y, int z);
        void uniform(std::string name, uint x, uint y, uint z);

        void uniform(std::string name, float x, float y, float z, float w);
        void uniform(std::string name, int x, int y, int z, int w);
        void uniform(std::string name, uint x, uint y, uint z, uint w);

        int get_id();
    private:
        int id = -1;
    };
}