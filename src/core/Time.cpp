#include "Time.h"
#include <algorithm>

Time::Time()
    : lastTime(SDL_GetPerformanceCounter())
    , currentTime(lastTime)
    , startTime(lastTime)
    , deltaTime(0.0f)
    , fps(0.0f)
    , frameCount(0)
    , elapsedTime(0.0f)
    , fpsSampleIndex(0)
{
    // Initialize FPS samples
    for (int i = 0; i < FPS_SAMPLE_COUNT; ++i) {
        fpsSamples[i] = 0.0f;
    }
}

void Time::tick() {
    lastTime = currentTime;
    currentTime = SDL_GetPerformanceCounter();

    uint64_t frequency = SDL_GetPerformanceFrequency();
    deltaTime = static_cast<float>(currentTime - lastTime) / static_cast<float>(frequency);

    // Clamp delta time to prevent huge jumps
    deltaTime = std::min(deltaTime, 0.25f);

    // Update elapsed time
    elapsedTime = static_cast<float>(currentTime - startTime) / static_cast<float>(frequency);

    frameCount++;
    updateFPS();
}

void Time::updateFPS() {
    if (deltaTime > 0.0f) {
        float currentFPS = 1.0f / deltaTime;

        // Store in circular buffer
        fpsSamples[fpsSampleIndex] = currentFPS;
        fpsSampleIndex = (fpsSampleIndex + 1) % FPS_SAMPLE_COUNT;

        // Calculate average FPS
        float sum = 0.0f;
        for (int i = 0; i < FPS_SAMPLE_COUNT; ++i) {
            sum += fpsSamples[i];
        }
        fps = sum / static_cast<float>(FPS_SAMPLE_COUNT);
    }
}
