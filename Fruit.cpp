#include "Fruit.h"
#include <algorithm>

Fruit::Fruit() : GameObject(0, 0) {}

Fruit::Fruit(int maxX, int maxY) : GameObject(0, 0) {
    Respawn(maxX, maxY);
}

void Fruit::Respawn(int maxX, int maxY) {
    x = rand() % maxX;
    y = rand() % maxY;
}