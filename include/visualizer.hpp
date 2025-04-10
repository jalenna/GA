#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "./problem.hpp"

#define BACKGROUNDCOLOR Color{18, 18, 18, 255}
#define LINECOLOR Color{40, 40, 40, 255}

class Visualizer
{
private:
    float nCols;
    float totalArea;

    Camera3D camera = {
        {8.f, 4.f, 8.f},    // Position
        {0, 0, 0},          // Target
        {0, 1.f, 0},        // Up
        75.f,               // Fov-y
        CAMERA_PERSPECTIVE, // Perspective
    };

    std::vector<Vector3> values;
    std::vector<unsigned char> colorMap;
    float cubeSize = 0.f;

    // Drawing
private:
    void DrawInitialGrid()
    {
        for (int i = 0; i < nCols; i++)
        {
            for (int j = 0; j < nCols; j++)
            {
                const Vector3 &value = values.at((i * nCols) + j);
                DrawCubeWiresV(Vector3{value.x, 0.f, value.z}, Vector3{cubeSize, 0.f, cubeSize}, Color{40, 40, 40, 255});
            }
        }
    }

    void DrawGridCenterPoints()
    {
        for (int i = 0; i < nCols; i++)
        {
            for (int j = 0; j < nCols; j++)
            {
                const Vector3 &value = values.at((i * nCols) + j);
                DrawSphere(Vector3{value.x, 0.f, value.z}, 0.01f, Color{40, 40, 40, 255});
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
                DrawSphere(values.at(idx), 0.01f, Color{colorMap.at(idx), 40, 40, 255});
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

        float maxHeight = 0.f;

        for (int i = 0; i < nCols; i++)
        {
            for (int j = 0; j < nCols; j++)
            {
                float x = startX + i * cubeSize + cubeSize / 2.f;
                float z = startZ + j * cubeSize + cubeSize / 2.f;
                float y = ProblemSpace::Output(Vector2{x, z});

                values.at((i * nCols) + j) = Vector3{x, y, z};
                float currentHeight = fabsf(y);
                if (currentHeight > maxHeight)
                    maxHeight = currentHeight;
            }
        }

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

    // Main
public:
    Visualizer(int nCols = 10, float totalArea = 1.f, int width = 1000, int height = 500)
    {
        this->nCols = nCols;
        this->totalArea = totalArea * 2;
        InitWindow(width, height, "GA Demo");
        SetTargetFPS(60);
        GenerateGrid();
        SetFullScreen();
    }

    void Visualize(int inputc, Vector2 *input)
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        BeginDrawing();

        ClearBackground(BACKGROUNDCOLOR);

        BeginMode3D(camera);
        // Render problem space
        // DrawCustomGrid(nCols, 1.f, LINECOLOR); // Debug
        // Draw3DAxis(); // Debug

        // TODO
        DrawInitialGrid();      // Stagger animation then,
        DrawGridCenterPoints(); // Stagger animation then,
        DrawGridHeight();       // Stagger animation

        // Render predictions

        EndMode3D();

        EndDrawing();
    }

    ~Visualizer()
    {
    }
};
