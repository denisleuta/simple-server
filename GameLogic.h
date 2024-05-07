#pragma once
#include "Fruit.h"
#include "Snake.h"
class GameLogic {
public:
    void Update(Snake& snake, Fruit& fruit, int& score, int width, int height, char dir, bool& gameOver);
};