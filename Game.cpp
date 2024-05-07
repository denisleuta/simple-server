#include "Game.h"

using namespace std;

bool game_running = true; // Глобальная переменная для управления циклом игры


void Game::start_game() {
    setlocale(LC_ALL, "RU");
    Timer gameTimer;
    double gameTime = 0.0;
    HighScoreManager highScoreManager;

    while (game_running) { // Проверяем, запущена ли игра
        SnakeGame game(20, 20);
        game.Setup();
        game.RunGameLoop();

        gameTime = gameTimer.GetElapsedTime();
        cout << "Время вашей игровой сессии: " << gameTime << " секунд" << endl;

        int score = game.GetScore();

        cout << "Введите ваше имя: ";
        string playerName;
        cin >> playerName;

        highScoreManager.SaveHighScore(playerName, score);
        highScoreManager.SaveHighScore(playerName, score);

        // Добавляем запись результата в базу данных
        std::string sql = "INSERT INTO results (name, result) VALUES ('" + playerName + "', " + std::to_string(score) + ")";
        execute_sql_update(sql);
        std::cout << "Результат игры успешно записан в базу данных." << std::endl;

        cout << "Лучшие счета:\n";
        vector<pair<string, int>> highScores = highScoreManager.GetHighScores();
        for (const auto& entry : highScores) {
            cout << entry.first << ": " << entry.second << endl;
        }

        // Здесь можно добавить проверку для остановки игры по желанию игрока
        cout << "Хотите продолжить игру? (yes/no): ";
        string continue_game;
        cin >> continue_game;
        if (continue_game == "no") {
            stop_game(); // Вызываем функцию stop_game()
        }

        this_thread::sleep_for(chrono::seconds(3));
    }
}

// Определение функции stop_game()
void Game::stop_game() {
    game_running = false; // Устанавливаем флаг остановки игры
}