#pragma once

#include "../entities/Ball.h"
#include <vector>
#include <unordered_map>

class SpatialGrid {
public:
    SpatialGrid(float cellSize, float worldWidth, float worldHeight);

    // Clear and rebuild grid
    void clear();
    void insertBall(size_t ballIndex, const Vector2D& position);

    // Get potential collision pairs
    void getPotentialCollisions(
        const std::vector<Ball>& balls,
        std::vector<std::pair<size_t, size_t>>& outPairs
    );

private:
    float cellSize;
    int gridWidth, gridHeight;

    // Grid cells store ball indices
    std::vector<std::vector<size_t>> cells;

    int getCellX(float x) const;
    int getCellY(float y) const;
    int getCellIndex(int cx, int cy) const;
};
