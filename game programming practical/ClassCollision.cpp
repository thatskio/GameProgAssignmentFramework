#include "ClassCollision.h"

bool ClassCollision::CheckRectCollision(
    float box1X, float box1Y, float box1Width, float box1Height,
    float box2X, float box2Y, float box2Width, float box2Height) 
{
    if (box1X + box1Width < box2X ||
        box1X > box2X + box2Width ||
        box1Y + box1Height < box2Y ||
        box1Y > box2Y + box2Height) {
        return false; 
    }
    return true; 
}

bool ClassCollision::CheckScaledRectCollision(
    float box1X, float box1Y, float box1Width, float box1Height, float box1Padding,
    float box2X, float box2Y, float box2Width, float box2Height, float box2Padding)
{
    return CheckRectCollision(
        box1X + box1Padding, box1Y + box1Padding, box1Width - (box1Padding * 2), box1Height - (box1Padding * 2),
        box2X + box2Padding, box2Y + box2Padding, box2Width - (box2Padding * 2), box2Height - (box2Padding * 2)
    );
}

bool ClassCollision::CheckRadianCollision(float point1X, float point1Y, float point2X, float point2Y, float minimumCollisionDistance) {
    float deltaX = point2X - point1X;
    float deltaY = point2Y - point1Y;
    float squaredDistance = (deltaX * deltaX) + (deltaY * deltaY);
    return squaredDistance < (minimumCollisionDistance * minimumCollisionDistance);
}