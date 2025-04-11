#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>

#include <stdlib.h>
#include "./problem.hpp"

#define BACKGROUNDCOLOR Color{18, 18, 18, 255}
#define LINECOLOR Color{40, 40, 40, 255}

class Visualizer
{
private:
    int screenWidth;
    int screenHeight;
    float nCols;
    float totalArea;
    Font textFont;
    int currentFunction = 1;

    Camera3D camera = {
        {4.f, 3.f, 4.f},    // Position
        {0, 0, 0},          // Target
        {0, 1.f, 0},        // Up
        75.f,               // Fov-y
        CAMERA_PERSPECTIVE, // Perspective
    };

    std::vector<Vector3> values;
    std::vector<unsigned char> colorMap;
    float cubeSize = 0.f;
    float maxHeight = 0.f;

    float animationStartDelay = 5.f; // Seconds

    int animationStage = 0;
    float animationDuration = 3.f;        // Seconds
    float currentAnimationProgress = 0.f; // Seconds

    // Drawing
private:
    void DrawInitialGrid()
    {
        for (int i = 0; i < nCols; i++)
        {
            for (int j = 0; j < nCols; j++)
            {
                const Vector3 &value = values.at((i * nCols) + j);
                float y = 0.f;

                if (animationStage == 0)
                {
                    y = StaggeredHeight(i + j);
                }

                DrawCubeWiresV(Vector3{value.x, y, value.z}, Vector3{cubeSize, 0.f, cubeSize}, Fade(LINECOLOR, 1.f - fabsf(y) / 2.f));
            }
        }
    }

    void DrawGridCenterPoints()
    {
        for (int i = 0; i < nCols; i++)
        {
            for (int j = 0; j < nCols; j++)
            {
                float y = 0;

                if (animationStage == 1)
                {
                    y = StaggeredHeight(i + j, 2.f);
                }

                const Vector3 &value = values.at((i * nCols) + j);
                DrawSphere(Vector3{value.x, y, value.z}, 0.01f, Color{40, 40, 40, 255});
            }
        }
    }

    void DrawGridHeight()
    {
        for (int i = 0; i < nCols; i++)
        {
            for (int j = 0; j < nCols; j++)
            {
                const int idx = (i * nCols) + j;
                Vector3 current = values.at(idx);

                float y = current.y;

                if (animationStage == 2)
                {
                    y = StaggeredHeight(i + j, 0.f, current.y);
                }

                unsigned char currentR = static_cast<unsigned char>(static_cast<int>(255 * (fabsf(y) / maxHeight)));
                unsigned char currentA = 0;
                if (current.y)
                {
                    currentA = static_cast<unsigned char>(static_cast<int>(255 * (fabsf(y) / fabsf(current.y))));
                }

                DrawSphere(Vector3{current.x, y, current.z}, 0.1f, Color{currentR, 40, 40, currentA});
            }
        }
    }

    void
    DrawCustomGrid(int slices, float spacing, Color color)
    {
        int halfSlices = slices / 2;

        for (int i = -halfSlices; i <= halfSlices; i++)
        {
            // Vertical lines (Z axis)
            DrawLine3D(Vector3{(float)i * spacing, 0.0f, (float)-halfSlices * spacing},
                       Vector3{(float)i * spacing, 0.0f, (float)halfSlices * spacing},
                       color);

            // Horizontal lines (X axis)
            DrawLine3D(Vector3{(float)-halfSlices * spacing, 0.0f, (float)i * spacing},
                       Vector3{(float)halfSlices * spacing, 0.0f, (float)i * spacing},
                       color);
        }
    }

    void Draw3DAxis(float axisLength = 1.0f, float tickSpacing = 0.2f, float tickSize = 0.05f, float labelOffset = 0.1f)
    {
        // Axis lines
        DrawLine3D(Vector3{-axisLength, 0, 0}, Vector3{axisLength, 0, 0}, RED);   // X
        DrawLine3D(Vector3{0, -axisLength, 0}, Vector3{0, axisLength, 0}, GREEN); // Y
        DrawLine3D(Vector3{0, 0, -axisLength}, Vector3{0, 0, axisLength}, BLUE);  // Z

        for (float t = -axisLength; t <= axisLength; t += tickSpacing)
        {
            // Skip 0 to avoid clutter
            if (t == 0)
                continue;

            // X Axis
            DrawLine3D(Vector3{t, -tickSize, 0}, Vector3{t, tickSize, 0}, RED);
            // Y Axis
            DrawLine3D(Vector3{-tickSize, t, 0}, Vector3{tickSize, t, 0}, GREEN);
            // Z Axis
            DrawLine3D(Vector3{-tickSize, 0, t}, Vector3{tickSize, 0, t}, BLUE);
        }
    }

    // Utils
private:
    void GenerateGrid()
    {
        values.resize(nCols * nCols);
        colorMap.resize(nCols * nCols);
        cubeSize = totalArea / nCols;

        float startX = -totalArea / 2;
        float startZ = startX;

        // float maxHeight = 0.f;

        for (int i = 0; i < nCols; i++)
        {
            for (int j = 0; j < nCols; j++)
            {
                float x = startX + i * cubeSize + cubeSize / 2.f;
                float z = startZ + j * cubeSize + cubeSize / 2.f;
                float y = 0.f;
                if (currentFunction == 1)
                {
                    y = ProblemSpace::ThreeDSin(Vector2{x, z});
                }
                else if (currentFunction == 2)
                {
                    y = ProblemSpace::ThreeDExp(Vector2{x, z});
                }

                values.at((i * nCols) + j) = Vector3{x, y, z};
                float currentHeight = fabsf(y);
                if (currentHeight > maxHeight)
                    maxHeight = currentHeight;
            }
        }

        if (maxHeight == 0)
            maxHeight = 1.f;

        // TODO: In case we don't want the animation
        for (int i = 0; i < nCols * nCols; i++)
        {
            colorMap.at(i) = 255 * fabsf(values.at(i).y) / maxHeight;
        }
    }

    void SetFullScreen()
    {
        const int display = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
        ToggleFullscreen();
    }

    float StaggeredHeight(int pos, float startingHeight = -2.f, float endHeight = 0.f, float delay = 0.5f)
    {
        int maxPos = 2 * (nCols - 1);               // since you're using a square grid, max i + j
        float maxDelay = animationDuration * delay; // Let stagger consume 50% of total time (tweakable)
        float delayPerPos = maxDelay / maxPos;

        float tStart = pos * delayPerPos;
        float t = (currentAnimationProgress - tStart) / (animationDuration - maxDelay);

        t = Clamp(t, 0.f, 1.f);
        return Lerp(startingHeight, endHeight, t);
    }

    // TODO
    void PlayNextIfPossible()
    {
        currentAnimationProgress += GetFrameTime();
        if (currentAnimationProgress >= animationDuration)
        {
            currentAnimationProgress = 0.f;
            animationStage++;
        }
    }

    void RestartAll()
    {
        GenerateGrid();
        currentAnimationProgress = 0.f;
        animationStage = 2;
    }

    // Main
public:
    Visualizer(int nCols = 10, float totalArea = 1.f, int width = 1000, int height = 500)
    {
        this->nCols = nCols;
        this->totalArea = totalArea * 2;
        screenWidth = width;
        screenHeight = height;
        InitWindow(width, height, "GA Demo");
        SetTargetFPS(60);
        SetFullScreen();
        GenerateGrid();
        textFont = LoadFont("resources/fonts/Poppins-Regular.ttf");
    }

    void Visualize(int inputc, Vector2 *input)
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        BeginDrawing();

        ClearBackground(BACKGROUNDCOLOR);

        if (animationStartDelay > 0)
        {
            animationStartDelay -= GetFrameTime();
            DrawTextEx(textFont, TextFormat("Starting in %.2f", animationStartDelay), Vector2{10, 10}, 20, 1.f, WHITE);
        }
        else
        {
            DrawTextEx(textFont, "Press R to restart animations", Vector2{10, 10}, 20, 1.f, WHITE);
            DrawTextEx(textFont, "Press [1 - 2] for different functions", Vector2{10, 40}, 20, 1.f, WHITE);
            DrawTextEx(textFont, TextFormat("Current function: %d", currentFunction), Vector2{10, 70}, 20, 1.f, WHITE);

            BeginMode3D(camera);
            // Render problem space
            // DrawCustomGrid(nCols, 1.f, LINECOLOR); // Debug
            // Draw3DAxis(); // Debug

            if (IsKeyPressed(KEY_R))
            {
                animationStage = 0.f;
                currentAnimationProgress = 0.f;
            }
            if (IsKeyPressed(KEY_ONE))
            {
                if (currentFunction != 1)
                {
                    currentFunction = 1;
                    RestartAll();
                }
            }
            else if (IsKeyPressed(KEY_TWO))
            {
                if (currentFunction != 2)
                {
                    currentFunction = 2;
                    RestartAll();
                }
            }

            DrawInitialGrid();
            if (animationStage > 0)
            {
                DrawGridCenterPoints();
            }
            if (animationStage > 1)
            {
                DrawGridHeight();
            }
            PlayNextIfPossible();

            // Render predictions

            EndMode3D();
        }

        EndDrawing();
    }

    ~Visualizer()
    {
        CloseWindow();
    }
};
