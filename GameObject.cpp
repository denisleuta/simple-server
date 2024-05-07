#include "GameObject.h"
GameObject::GameObject(int startX, int startY) : x(startX), y(startY) {}

int GameObject::GetX() const {
    return x;
}

int GameObject::GetY() const {
    return y;
}