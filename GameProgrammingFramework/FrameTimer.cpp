#include "FrameTimer.h"

void FrameTimer::Init(int fps)
{
	QueryPerformanceFrequency(&timerFreq);
	QueryPerformanceCounter(&timeNow);
	QueryPerformanceCounter(&timePrevious);

	//FPS to run (called upon initialization)
	requestedfFPS = fps;
	//The number interval in the given timer, per frame at the requested rate.
	intervalPerFrame = ((float)timerFreq.QuadPart / requestedfFPS);
}

int FrameTimer::FramesToUpdate()
{
	int framesToUpdate = 0;
	QueryPerformanceCounter(&timeNow);

	//Getting delta time between time passed for frames
	intervalsSinceLastUpdate = (float)(timeNow.QuadPart - timePrevious.QuadPart);
	framesToUpdate = (int)(intervalsSinceLastUpdate / intervalPerFrame);

	//If we are not updating any frame, keep the old previous timer count
	if (framesToUpdate != 0) {
		timePrevious.QuadPart += (LONGLONG)(framesToUpdate * intervalPerFrame);
	}
	return framesToUpdate;
}
