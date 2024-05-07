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
class Game {
public:
	static void start_game();
	static void stop_game();
 
};
void execute_sql_update(const std::string& sql);