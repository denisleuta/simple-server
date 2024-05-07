#pragma once
#include "GameObject.h"
class Snake : public GameObject {
public:
    Snake();
    Snake(int startX, int startY);
    void Move(int newX, int newY);
    bool IsColliding(int newX, int newY) const;


private:

    int tailX[100];
    int tailY[100];
    int nTail;
};