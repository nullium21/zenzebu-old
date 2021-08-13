#include "zz/app.h"

using namespace zz::app;

int main(int argc, char const *argv[])
{
    auto app = create_app();

    app->run();

    delete app;
    
    return 0;
}