#pragma once

#include "../entities/Container.h"
#include "../physics/PhysicsEngine.h"
#include "BallManager.h"

class GameState {
public:
    GameState();

    void initialize();
    void update(float deltaTime);

    // Access game objects
    BallManager& getBallManager() { return ballManager; }
    Container& getContainer() { return container; }
    PhysicsEngine& getPhysics() { return physics; }

    const BallManager& getBallManager() const { return ballManager; }
    const Container& getContainer() const { return container; }

    // Stats
    size_t getBallCount() const;

private:
    BallManager ballManager;
    Container container;
    PhysicsEngine physics;
};
