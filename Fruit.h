#pragma once
#include "GameObject.h"
class Fruit : public GameObject {
public:
    Fruit();
    Fruit(int maxX, int maxY);
    void Respawn(int maxX, int maxY);
};