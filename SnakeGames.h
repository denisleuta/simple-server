#pragma once
#include "Snake.h"
#include "Fruit.h"
#include "Obstacle.h"
#include <vector>
#include "GameRenderer.h"
#include "GameLogic.h"
#include "HighScoreManager.h"
#include "InputHandler.h"
#include <fstream>

class SnakeGame {
public:
    SnakeGame(int w, int h);
    int GetScore() const;
    void Setup();
    void RunGameLoop();
    void Obstacles();
    void GenerateObstacles();
private:
    bool IsGameOver() const;
    bool gameOver;
    const int width;
    const int height;
    int score;
    Snake snake;
    Fruit fruit;
    std::vector<Obstacle> obstacles;
    const int numObstacles = 15;
    enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
    eDirection dir;
};