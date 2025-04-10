#pragma once
#include <raylib.h>
#include <math.h>

namespace ProblemSpace
{
    Vector3 Origin = {0.f, 0.f, 0.f};

    float Output(Vector2 input)
    {
        float a = pow(input.x, 2) + pow(input.y, 2);
        // return -1 * exp(-1 * pow((pow(input.x, 2) + pow(input.y, 2)), 2));
        return sinf(a);
    }

};