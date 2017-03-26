#include <sstream>
#include "detection.h"


Detection::Detection(std::string name, float top, float left,
              float bottom, float right, float probability)
{
    this->name = name;
    this->top = top;
    this->left = left;
    this->bottom = bottom;
    this->right = right;
    this->probability = probability;

}

Detection::Detection(std::string line)
{
    std::stringstream stream(line);

    stream >> name;
    stream >> top;
    stream >> left;
    stream >> bottom;
    stream >> right;
    stream >> probability;
}

json11::Json Detection::to_json() const
{
    return json11::Json::object {
        { "name", name },
        { "top", top },
        { "left", left },
        { "bottom", bottom },
        { "right", right },
        { "probability", probability }
    };
}