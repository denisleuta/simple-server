#include "Game.h"

using namespace std;
bool Game::game_running = true;
int Game::score = 0;
std::string Game::playerName;

void Game::start_game() {
    setlocale(LC_ALL, "RU");
    Timer gameTimer;
    double gameTime = 0.0;
    HighScoreManager highScoreManager;

    while (game_running) { 
        SnakeGame game(20, 20);
        game.Setup();
        game.RunGameLoop();

        gameTime = gameTimer.GetElapsedTime();
        cout << "Время вашей игровой сессии: " << gameTime << " секунд" << endl;

        score = game.GetScore(); 

        cout << "Введите ваше имя: ";
        string playerName;
        cin >> playerName;

        highScoreManager.SaveHighScore(playerName, score);
        highScoreManager.SaveHighScore(playerName, score);

        cout << "Лучшие счета:\n";
        vector<pair<string, int>> highScores = highScoreManager.GetHighScores();
        for (const auto& entry : highScores) {
            cout << entry.first << ": " << entry.second << endl;
        }

        send_results_to_server(playerName, score);

        cout << "Хотите продолжить игру? (yes/no): ";
        string continue_game;
        cin >> continue_game;
        if (continue_game == "no") {
            stop_game(); 
            break; 
        }

        this_thread::sleep_for(chrono::seconds(5));
    }
}

void Game::stop_game() {
    game_running = false;
}

int Game::getScore() {
    return score;
}
