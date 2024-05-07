#include "GameRenderer.h"

using namespace std;

void GameRenderer::Draw(const Snake& snake, const Fruit& fruit, const std::vector<Obstacle>& obstacles, int width, int height, int score) const {
    
    std::cout << "\033[2J\033[H";
    for (int i = 0; i < width + 1; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0 || j == width - 1)
                cout << "#";
            if (i == snake.GetY() && j == snake.GetX())
                cout << "0";
            else if (i == fruit.GetY() && j == fruit.GetX())
                cout << "F";
            else {
                bool isObstacle = false;
                for (const Obstacle& obstacle : obstacles) {
                    if (obstacle.GetX() == j && obstacle.GetY() == i) {
                        cout << "@";
                        isObstacle = true;
                        break;
                    }
                }
                if (!isObstacle) {
                    if (snake.IsColliding(j, i))
                        cout << "o";
                    else
                        cout << " ";
                }
            }
        }
        cout << endl;
    }

    for (int i = 0; i < width + 1; i++)
        cout << "#";
    cout << endl;
    cout << "Score: " << score << endl;
}