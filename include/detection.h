#include <string>
#include "json11.hpp"


class Detection
{
    public:
        std::string name;
        float top;
        float left;
        float bottom;
        float right;
        float probability;

    Detection() { }
    Detection(std::string name, float top, float left,
              float bottom, float right, float probability);

    Detection(std::string line);
    json11::Json::object to_json();
};