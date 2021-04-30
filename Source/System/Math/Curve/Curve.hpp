#pragma once
#include <vector>

#include "../Algebra/Vector3.hpp"

namespace Engine5
{
 

    class Curve
    {
    public:
        Curve();
        ~Curve();

    public:
        std::vector<Vector3> points;
    };
}
