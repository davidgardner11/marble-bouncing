#pragma once

#include "../rendering/Renderer.h"
#include "../rendering/CircleRenderer.h"
#include "../rendering/TextRenderer.h"
#include "../game/GameState.h"
#include "Time.h"

class Application {
public:
    Application();
    ~Application();

    bool initialize();
    void run();
    void cleanup();

private:
    // Core systems
    Renderer renderer;
    GameState gameState;
    Time time;
    CircleRenderer circleRenderer;
    TextRenderer textRenderer;

    bool running;
    float accumulator;  // For fixed timestep

    // Game loop methods
    void handleEvents();
    void update(float deltaTime);
    void render();

    // Rendering helpers
    void renderContainer();
    void renderBalls();
    void renderUI();
};
