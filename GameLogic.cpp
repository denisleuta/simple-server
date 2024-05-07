#include "GameLogic.h"
void GameLogic::Update(Snake& snake, Fruit& fruit, int& score, int width, int height, char dir, bool& gameOver) {
    int newX = snake.GetX();
    int newY = snake.GetY();
    int prevX = newX;
    int prevY = newY;

    switch (dir) {
    case 'a':
        newX--;
        break;
    case 'd':
        newX++;
        break;
    case 'w':
        newY--;
        break;
    case 's':
        newY++;
        break;
    }

    if (newX > width || newX < 0 || newY > height || newY < 0)
        gameOver = true;

    if (snake.IsColliding(newX, newY))
        gameOver = true;

    if (newX == fruit.GetX() && newY == fruit.GetY()) {
        score += 10;
        fruit.Respawn(width, height);
        snake.Move(newX, newY);
    }
    else {
        snake.Move(newX, newY);
    }
}