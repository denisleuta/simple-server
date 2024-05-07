#include "SnakeGames.h"

SnakeGame::SnakeGame(int w, int h) : width(w), height(h), score(0) {
    gameOver = false;
    dir = STOP;
    snake = Snake(w / 2 - 1, h / 2 - 1);
    fruit = Fruit(w, h);
}
int SnakeGame::GetScore() const {
    return score;
}

void SnakeGame::Setup() {
    gameOver = false;
    dir = STOP;
    snake = Snake(width / 2 - 1, height / 2 - 1);
    fruit.Respawn(width, height);
    score = 0;
    obstacles.clear();
    GenerateObstacles();
}

void SnakeGame::GenerateObstacles() {
    for (int i = 0; i < numObstacles; i++) {
        int obstacleX = rand() % (width - 2) + 1;
        int obstacleY = rand() % (height - 2) + 1;
        obstacles.push_back(Obstacle(obstacleX, obstacleY));
    }
}

void SnakeGame::RunGameLoop() {
    GameRenderer renderer;
    InputHandler inputHandler;
    GameLogic logic;

    while (!IsGameOver()) {
        renderer.Draw(snake, fruit, obstacles, width, height, score);

        char input = inputHandler.GetInput();
        logic.Update(snake, fruit, score, width, height, input, gameOver);

        for (const Obstacle& obstacle : obstacles) {
            if (snake.GetX() == obstacle.GetX() && snake.GetY() == obstacle.GetY()) {
                gameOver = true;
                break;
            }
        }

        //Sleep(10);
    }
}
bool SnakeGame::IsGameOver() const {
    return gameOver;
}

