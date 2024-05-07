#include "Game.h"

using namespace std;

bool game_running = true; // ���������� ���������� ��� ���������� ������ ����


void Game::start_game() {
    setlocale(LC_ALL, "RU");
    Timer gameTimer;
    double gameTime = 0.0;
    HighScoreManager highScoreManager;

    while (game_running) { // ���������, �������� �� ����
        SnakeGame game(20, 20);
        game.Setup();
        game.RunGameLoop();

        gameTime = gameTimer.GetElapsedTime();
        cout << "����� ����� ������� ������: " << gameTime << " ������" << endl;

        int score = game.GetScore();

        cout << "������� ���� ���: ";
        string playerName;
        cin >> playerName;

        highScoreManager.SaveHighScore(playerName, score);
        highScoreManager.SaveHighScore(playerName, score);

        // ��������� ������ ���������� � ���� ������
        std::string sql = "INSERT INTO results (name, result) VALUES ('" + playerName + "', " + std::to_string(score) + ")";
        execute_sql_update(sql);
        std::cout << "��������� ���� ������� ������� � ���� ������." << std::endl;

        cout << "������ �����:\n";
        vector<pair<string, int>> highScores = highScoreManager.GetHighScores();
        for (const auto& entry : highScores) {
            cout << entry.first << ": " << entry.second << endl;
        }

        // ����� ����� �������� �������� ��� ��������� ���� �� ������� ������
        cout << "������ ���������� ����? (yes/no): ";
        string continue_game;
        cin >> continue_game;
        if (continue_game == "no") {
            stop_game(); // �������� ������� stop_game()
        }

        this_thread::sleep_for(chrono::seconds(3));
    }
}

// ����������� ������� stop_game()
void Game::stop_game() {
    game_running = false; // ������������� ���� ��������� ����
}