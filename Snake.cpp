#include "Snake.h"

Snake::Snake() : GameObject(0, 0), nTail(0) {}

Snake::Snake(int startX, int startY) : GameObject(startX, startY), nTail(0) {}

void Snake::Move(int newX, int newY) {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    x = newX;
    y = newY;
}

bool Snake::IsColliding(int newX, int newY) const {
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == newX && tailY[i] == newY)
            return true;
    }
    return false;
}