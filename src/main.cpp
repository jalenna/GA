#include <iostream>
#include "../include/visualizer.hpp"

int main(int argc, char **argv)
{
    int n_cols = 20;
    int totalArea = 3.f;
    int screenWidth = 1000;
    int screenHeight = 500;

    Visualizer visualizer = Visualizer(n_cols, totalArea, screenWidth, screenHeight);

    while (!WindowShouldClose())
    {
        visualizer.Visualize(0, {0});
    }

    return 0;
}