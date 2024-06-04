#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <algorithm>
#include "Fruit.h"
#include "Snake.h"
#include "GameLogic.h"
#include "GameRenderer.h"
#include "HighScoreManager.h"
#include "InputHandler.h"
#include "Obstacle.h"
#include "SnakeGames.h"
#include "Timer.h"
#include <boost/asio.hpp>

void send_results_to_server(const std::string& playerName, int score);

class Game {
private:
    static bool game_running;
    static int score;
    
public:
    static void start_game();
    static void stop_game();
    static int getScore();
    static std::string playerName;
};
