#include "design.h"

#include <raylib.h>
#include <iostream>
#include <cmath>

void DrawThickArrow(Vector2 start, Vector2 end, float thickness, float headSize, Color color)
{
    Vector2 direction = {end.x - start.x, end.y - start.y};
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    Vector2 normalized = {direction.x * 1.0f / length, direction.y * 1.0f / length};

    Vector2 perp = {-normalized.y, normalized.x};

    Vector2 bodyStartLeft = {start.x + perp.x * thickness / 2, start.y + perp.y * thickness / 2};
    Vector2 bodyStartRight = {start.x - perp.x * thickness / 2, start.y - perp.y * thickness / 2};
    Vector2 bodyEndLeft = {end.x + perp.x * thickness / 2, end.y + perp.y * thickness / 2};
    Vector2 bodyEndRight = {end.x + perp.x * thickness / 2, end.y + perp.y * thickness / 2};

    DrawTriangle(bodyStartLeft, bodyStartRight, bodyEndRight, color);
    DrawTriangle(bodyStartLeft, bodyEndRight, bodyEndLeft, color);

    Vector2 arrowHeadLeft = {end.x + perp.x * headSize / 2, end.y + perp.y * headSize / 2};
    Vector2 arrowHeadRight = {end.x - perp.x * headSize / 2, end.y - perp.y * headSize / 2};
    Vector2 arrowTip = {end.x + normalized.x * headSize, end.y + normalized.y * headSize};

    DrawTriangle(arrowTip, arrowHeadLeft, arrowHeadRight, color);
}

const int BlockSize = 200;

int DrawArrow(const Vector2 start, const Vector2 end)
{
    float thickness = 15.0f;
    float headSize = 30.0f;
    Color color = DARKBLUE;
    Vector2 direction = {end.x - start.x, end.y - start.y};
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);

    if (length == 0)
        return -1;
    Vector2 normalized = {direction.x / length, direction.y / length};
    Vector2 perp = {-normalized.y, normalized.x};

    Vector2 bodyStartLeft = {start.x + perp.x * (thickness / 2), start.y + perp.y * (thickness / 2)};
    Vector2 bodyStartRight = {start.x - perp.x * (thickness / 2), start.y - perp.y * (thickness / 2)};
    Vector2 bodyEndLeft = {end.x - normalized.x * headSize + perp.x * (thickness / 2), end.y - normalized.y * headSize + perp.y * (thickness / 2)};
    Vector2 bodyEndRight = {end.x - normalized.x * headSize - perp.x * (thickness / 2), end.y - normalized.y * headSize - perp.y * (thickness / 2)};

    Vector2 arrowTip = {end.x, end.y};
    Vector2 arrowHeadLeft = {end.x - normalized.x * headSize + perp.x * (thickness / 2), end.y - normalized.y * headSize + perp.y * (thickness / 2) + 20};
    Vector2 arrowHeadRight = {(end.x - normalized.x * headSize - perp.x * (thickness / 2)), end.y - normalized.y * headSize - perp.y * (thickness / 2) - 20};


    DrawTriangle(bodyStartRight, bodyStartLeft, bodyEndRight, color);
    DrawTriangle(bodyStartLeft, bodyEndLeft, bodyEndRight, color);

    DrawTriangle(arrowTip, arrowHeadRight, arrowHeadLeft, color);
}

int DrawNode(const float x, const float y)
{
    const float width_rect = 140;
    const float height_rect = 140;
    float start_point_x = width_rect / 2.0f + x - 7.0f;
    float start_point_y = height_rect / 2.0f + y + 23.0f;

    Vector2 startPoint = {start_point_x - 35.0f, start_point_y - 35.0f};
    Vector2 endPoint1 = {start_point_x, start_point_y};
    Vector2 StartPoint2 = {start_point_x - 4.0f, start_point_y + 4.0f};
    Vector2 endPoint2 = {start_point_x + 60.0f, start_point_y - 60.0f};
    DrawRectangle(x, y, width_rect, height_rect, LIGHTGRAY);

    DrawLineEx(startPoint, endPoint1, 10.0f, GREEN); 
    DrawLineEx(StartPoint2, endPoint2, 10.0f, GREEN);
    return 1;
}

int DrawNodeLoad(const float x, const float y, float &rotationAngle, float &child_coord_x, float &child_coord_y)
{
    const int numPoints = 12;
    child_coord_x = x;
    child_coord_y = y;

    const float circleRadius = 20.0f;
    const float width_rect = 140;
    const float height_rect = 140;
    DrawRectangle(x, y, width_rect, height_rect, LIGHTGRAY);
    rotationAngle += 2.0f; 
    if (rotationAngle >= 360.0f)
        rotationAngle -= 360.0f;
    Vector2 circleCenter = {width_rect / 2.0f + x, height_rect / 2.0f + y};

    DrawText("Loading Animation Example", 10, 10, 20, DARKGRAY);

    for (int i = 0; i < numPoints; i++)
    {
        float angle = (2 * PI / numPoints) * i + rotationAngle * (PI / 180.0f);
        float x = circleCenter.x + std::cos(angle) * circleRadius;
        float y = circleCenter.y + std::sin(angle) * circleRadius;

        float alpha = (1.0f - (float)i / numPoints) * 0.75f + 0.25f;

        DrawCircle((int)x, (int)y, 3.0f, Color{0, 122, 255, (unsigned char)(alpha * 255)});
    }
}

int DrawTree(arranged_block_iterable_proxy current_block, int y, int x, float &rotationAngle, float &child_coord_x, float &child_coord_y, bool is_root)
{
    int num_of_children = current_block.size();
    bool is_print_arrow = true;
    if (is_root)
    {
        is_root = false;
        is_print_arrow = false;
    }
    if (num_of_children == 0)
    {
        Vector2 end = {static_cast<float>(x), static_cast<float>(y) + 70.0f};
        Vector2 start = {static_cast<float>(x) - 60.0f, static_cast<float>(y) + 70.0f};
        DrawArrow(start, end);
        DrawNodeLoad(static_cast<float>(x), static_cast<float>(y), rotationAngle, child_coord_x, child_coord_y);
        return BlockSize;
    }
    if (num_of_children == 0)
    {
        printf("ERROR in DrawTree\n");
        return -1;
    }
    int height = 0;
    for (auto it = current_block.begin(); it < current_block.end(); ++it)
    {
        height += DrawTree(*it, height + y, x + BlockSize, rotationAngle, child_coord_x, child_coord_y, is_root);

        DrawNode(x, y);
        if (is_print_arrow)
        {
            Vector2 end = {static_cast<float>(x), static_cast<float>(y) + 70.0f};
            Vector2 start = {static_cast<float>(x) - 60.0f, static_cast<float>(y) + 70.0f};
            DrawArrow(start, end);
        }
    }
    return height;
}

