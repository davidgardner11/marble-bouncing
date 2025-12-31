#pragma once

#include <SDL2/SDL.h>
#include <cstdint>

class Time {
public:
    Time();

    void tick();  // Call once per frame

    float getDeltaTime() const { return deltaTime; }
    float getFPS() const { return fps; }
    uint64_t getFrameCount() const { return frameCount; }
    float getElapsedTime() const { return elapsedTime; }  // Total elapsed time in seconds

private:
    uint64_t lastTime;
    uint64_t currentTime;
    uint64_t startTime;
    float deltaTime;
    float fps;
    uint64_t frameCount;
    float elapsedTime;

    // FPS smoothing
    static constexpr int FPS_SAMPLE_COUNT = 60;
    float fpsSamples[FPS_SAMPLE_COUNT];
    int fpsSampleIndex;

    void updateFPS();
};
