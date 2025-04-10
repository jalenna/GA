#include <iostream>
#include "../include/visualizer.hpp"

int main(int argc, char **argv)
{
    int screenWidth = 1000;
    int screenHeight = 500;

    Visualizer visualizer = Visualizer(screenWidth, screenHeight);

    while (!WindowShouldClose())
    {
        visualizer.Visualize(0, {0});
    }
    CloseWindow();

    return 0;
}