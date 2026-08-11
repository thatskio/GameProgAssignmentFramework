#pragma once

class ClassCollision {
public:
    // Bounding box collision
    static bool CheckRectCollision(
        float box1X, float box1Y, float box1Width, float box1Height,
        float box2X, float box2Y, float box2Width, float box2Height
    );

    // 80% collision box checking
    static bool CheckScaledRectCollision(
        float box1X, float box1Y, float box1Width, float box1Height, float box1Padding,
        float box2X, float box2Y, float box2Width, float box2Height, float box2Padding
    );

    // Faster collision method
    static bool CheckRadianCollision(
        float point1X, float point1Y,
        float point2X, float point2Y,
        float minimumCollisionDistance
    );
};