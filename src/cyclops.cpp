#include "../include/crow_all.h"

#include <sstream>

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
        .name("hello")
    ([]{
        return "Hello World!";
    });

    app.port(1234)
        .multithreaded()
        .run();
}
