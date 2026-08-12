#pragma once
#include <Windows.h>

class FrameTimer {
public:
    void Init(int fps);
    int FramesToUpdate();

private:
    LARGE_INTEGER timerFreq;
    LARGE_INTEGER timeNow;
    LARGE_INTEGER timePrevious;
    int requestedfFPS;
    float intervalPerFrame;
    float intervalsSinceLastUpdate;
};