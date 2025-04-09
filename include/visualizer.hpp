#pragma once
#include <raylib.h>
#include <vector>
#include "./problem.hpp"

#define BACKGROUNDCOLOR (Color){18, 18, 18, 255}
#define LINECOLOR (Color){40, 40, 40, 255}

#define ROWS 20
#define COLS 20
#define CUBE_EXTENT .5f

class Visualizer
{
private:
    Camera3D camera = {
        {5.f, 3.f, 3.f},    // Position
        {0, 0, 0},          // Target
        {0, 1.f, 0},        // Up
        75.f,               // Fov-y
        CAMERA_PERSPECTIVE, // Perspective
    };

    std::vector<Vector3> values;
    float currentMax = 0.f;

private:
    void DrawPointsOnGrid()
    {
        const float safeMax = (currentMax == 0) ? 1.0f : currentMax;
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                const int idx = (i * COLS) + j;
                const Vector3 &value = values[idx];
                const unsigned char c = 255 * fabsf(value.y) / safeMax;
                if (fabsf(value.y) > 0.01)
                {
                    DrawSphere((Vector3){value.x, value.y, value.z}, .05f, (Color){c, 40, 40, 255});
                }
            }
        }
    }

    void DrawCustomGrid(int slices, float spacing, Color color)
    {
        int halfSlices = slices / 2;

        for (int i = -halfSlices; i <= halfSlices; i++)
        {
            // Vertical lines (Z axis)
            DrawLine3D((Vector3){(float)i * spacing, 0.0f, (float)-halfSlices * spacing},
                       (Vector3){(float)i * spacing, 0.0f, (float)halfSlices * spacing},
                       color);

            // Horizontal lines (X axis)
            DrawLine3D((Vector3){(float)-halfSlices * spacing, 0.0f, (float)i * spacing},
                       (Vector3){(float)halfSlices * spacing, 0.0f, (float)i * spacing},
                       color);
        }
    }

public:
    Visualizer(int width, int height)
    {
        InitWindow(width, height, "GA Demo");
        SetTargetFPS(60);
        values.resize(ROWS * COLS);
        // printf("size: %f\n", CUBE_EXTENT);

        for (int i = ROWS / -2, xIDX = 0; xIDX < ROWS; xIDX++, i++)
        {
            float x = i * (CUBE_EXTENT / 2);
            for (int j = COLS / -2, yIDX = 0; yIDX < COLS; yIDX++, j++)
            {
                float z = j * (CUBE_EXTENT / 2);
                float v = ProblemSpace::Output((Vector2){x, z});
                if (abs(v) > currentMax)
                {
                    currentMax = fabsf(v);
                }
                // printf("idx: x %f y %f v %f\n", x, z, abs(v));
                values[(xIDX * COLS) + yIDX] = (Vector3){x, v, z};
            }
        }
        const int display = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
        ToggleFullscreen();
    }

    void Visualize(int inputc, Vector2 *input)
    {
        // for (int i = 0; i < inputc; i++)
        // {
        //     float output = ProblemSpace::Output(input[i]);
        // }
        UpdateCamera(&camera, CAMERA_ORBITAL);

        BeginDrawing();

        ClearBackground(BACKGROUNDCOLOR);

        BeginMode3D(camera);
        // Render problem space
        // DrawCustomGrid(ROWS, CUBE_EXTENT, LINECOLOR);
        DrawPointsOnGrid();

        // Render predictions

        EndMode3D();

        EndDrawing();
    }

    ~Visualizer() {}
};
