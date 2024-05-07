#pragma once
class GameObject {
public:
    GameObject(int startX, int startY);
    int GetX() const;
    int GetY() const;

protected:
    int x, y;
};