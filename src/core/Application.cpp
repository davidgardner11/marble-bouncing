#include "Application.h"
#include "Config.h"
#include "../math/MathUtils.h"
#include <iostream>

Application::Application()
    : renderer(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, Config::WINDOW_TITLE)
    , bouncinessSlider(Config::SLIDER_X, Config::SLIDER_Y, Config::SLIDER_WIDTH, Config::SLIDER_HEIGHT, 0.8f, 1.2f, Config::RESTITUTION)
    , ballSizeSlider(Config::SIZE_SLIDER_X, Config::SIZE_SLIDER_Y, Config::SIZE_SLIDER_WIDTH, Config::SIZE_SLIDER_HEIGHT, 5.0f, 25.0f, Config::BALL_RADIUS)
    , resetButton(Config::RESET_BUTTON_X, Config::RESET_BUTTON_Y, Config::RESET_BUTTON_WIDTH, Config::RESET_BUTTON_HEIGHT, "Reset")
    , restitution(Config::RESTITUTION)
    , ballRadius(Config::BALL_RADIUS)
    , running(false)
    , accumulator(0.0f)
{
    // Set up reset button callback
    resetButton.setOnClick([this]() {
        resetSimulation();
    });
}

Application::~Application() {
    cleanup();
}

bool Application::initialize() {
    // Initialize renderer
    if (!renderer.initialize()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return false;
    }

    // Initialize circle renderer
    circleRenderer.initialize(renderer.getSDLRenderer());

    // Initialize text renderer
    if (!textRenderer.initialize()) {
        std::cerr << "Failed to initialize text renderer" << std::endl;
        return false;
    }

    // Initialize game state
    gameState.initialize();

    running = true;
    return true;
}

void Application::run() {
    while (running) {
        time.tick();
        float frameTime = time.getDeltaTime();

        // Prevent spiral of death
        if (frameTime > 0.25f) {
            frameTime = 0.25f;
        }

        accumulator += frameTime;

        // Handle events
        handleEvents();

        // Fixed timestep updates
        int steps = 0;
        while (accumulator >= Config::FIXED_TIMESTEP && steps < Config::MAX_PHYSICS_STEPS) {
            update(Config::FIXED_TIMESTEP);
            accumulator -= Config::FIXED_TIMESTEP;
            steps++;
        }

        // Render
        render();
    }
}

void Application::cleanup() {
    circleRenderer.cleanup();
    textRenderer.cleanup();
    renderer.cleanup();
}

void Application::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            bouncinessSlider.handleMouseDown(event.button.x, event.button.y);
            ballSizeSlider.handleMouseDown(event.button.x, event.button.y);
            resetButton.handleMouseDown(event.button.x, event.button.y);
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            bouncinessSlider.handleMouseUp();
            ballSizeSlider.handleMouseUp();
            resetButton.handleMouseUp(event.button.x, event.button.y);
        } else if (event.type == SDL_MOUSEMOTION) {
            bouncinessSlider.handleMouseMove(event.motion.x, event.motion.y);
            ballSizeSlider.handleMouseMove(event.motion.x, event.motion.y);
            resetButton.handleMouseMove(event.motion.x, event.motion.y);
            // Update restitution from slider
            restitution = bouncinessSlider.getValue();
            // Update ball radius from slider
            ballRadius = ballSizeSlider.getValue();
        }
    }
}

void Application::update(float deltaTime) {
    // Update ball radius in ball manager
    gameState.getBallManager().setBallRadius(ballRadius);

    gameState.update(deltaTime, restitution);
}

void Application::render() {
    // Clear screen
    renderer.clear(Config::BACKGROUND_COLOR);

    // Render game objects
    renderContainer();
    renderBalls();
    renderUI();

    // Present
    renderer.endFrame();
}

void Application::renderContainer() {
    const Container& container = gameState.getContainer();

    Vector2D center = container.getCenter();
    float radius = container.getRadius();
    float gapStart = container.getGapStartAngle();
    float gapEnd = container.getGapEndAngle();

    // Draw the container as an arc (excluding the gap)
    // We need to draw from gapEnd to gapStart (the complement of the gap)
    float arcStart = gapEnd;
    float arcEnd = gapStart + MathUtils::TWO_PI;

    circleRenderer.drawArc(
        renderer.getSDLRenderer(),
        center,
        radius,
        arcStart,
        arcEnd,
        Config::CONTAINER_COLOR,
        3  // thickness
    );
}

void Application::renderBalls() {
    const std::vector<Ball>& balls = gameState.getBallManager().getBalls();

    for (const Ball& ball : balls) {
        circleRenderer.drawFilledCircleFast(
            renderer.getSDLRenderer(),
            ball.position,
            ball.radius,
            ball.color
        );
    }
}

void Application::renderUI() {
    // Render reset button
    resetButton.render(renderer.getSDLRenderer());
    textRenderer.renderText(
        renderer.getSDLRenderer(),
        "Reset",
        Config::RESET_BUTTON_X + 25,
        Config::RESET_BUTTON_Y + 7,
        Config::TEXT_COLOR
    );

    // Render FPS (cached)
    textRenderer.renderFPSCached(
        renderer.getSDLRenderer(),
        time.getFPS(),
        Config::FPS_DISPLAY_X,
        Config::FPS_DISPLAY_Y
    );

    // Render ball count (cached)
    textRenderer.renderBallCountCached(
        renderer.getSDLRenderer(),
        gameState.getBallCount(),
        Config::BALL_COUNT_X,
        Config::BALL_COUNT_Y
    );

    // Render timer (cached)
    textRenderer.renderTimerCached(
        renderer.getSDLRenderer(),
        time.getElapsedTime(),
        Config::TIMER_DISPLAY_X,
        Config::TIMER_DISPLAY_Y
    );

    // Render bounciness slider
    bouncinessSlider.render(renderer.getSDLRenderer(), "Bounciness");

    // Render slider label and value
    char bouncinessLabel[64];
    snprintf(bouncinessLabel, sizeof(bouncinessLabel), "Bounciness: %.0f%%", restitution * 100.0f);
    textRenderer.renderText(
        renderer.getSDLRenderer(),
        bouncinessLabel,
        Config::SLIDER_X,
        Config::SLIDER_Y - 25,
        Config::TEXT_COLOR
    );

    // Render ball size slider
    ballSizeSlider.render(renderer.getSDLRenderer(), "Ball Size");

    // Render ball size label and value
    char sizeLabel[64];
    snprintf(sizeLabel, sizeof(sizeLabel), "Ball Size: %.0fpx", ballRadius * 2.0f);
    textRenderer.renderText(
        renderer.getSDLRenderer(),
        sizeLabel,
        Config::SIZE_SLIDER_X,
        Config::SIZE_SLIDER_Y - 25,
        Config::TEXT_COLOR
    );
}

void Application::resetSimulation() {
    // Clear all balls and reset to initial state
    gameState.getBallManager().getBalls().clear();
    gameState.initialize();

    // Reset timer
    time = Time();
}
