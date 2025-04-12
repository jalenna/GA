#pragma once
#include <raylib.h>
#include <math.h>

namespace ProblemSpace
{
    typedef float (*Output)(Vector2 in);

    Vector3 Origin = {0.f, 0.f, 0.f};

    float ThreeDSin(Vector2 input)
    {
        float a = pow(input.x, 2) + pow(input.y, 2);
        return sinf(a);
    }

    float ThreeDExp(Vector2 input)
    {
        float a = pow(input.x, 2) + pow(input.y, 2);
        return -1 * exp(-1 * pow(a, 2));
    }

    float CosSin(Vector2 input)
    {
        return (sin(4 * input.x) - cos(5 * input.y)) / 5;
    }

    float HighCos(Vector2 input)
    {
        return cos(pow(input.x, 2) + pow(input.y, 2) - 0.5) - 0.5;
    }

    std::vector<Output> outputs = {
        ThreeDSin,
        ThreeDExp,
        CosSin,
        HighCos,
    };
};