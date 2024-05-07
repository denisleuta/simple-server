#pragma once
#include "Snake.h"
#include "Fruit.h"
#include "Obstacle.h"
#include <iostream>
#include <vector>

class GameRenderer {
public:
    void Draw(const Snake& snake, const Fruit& fruit, const std::vector<Obstacle>& obstacles, int width, int height, int score) const;
};