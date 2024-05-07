#include "HighScoreManager.h"

void HighScoreManager::SaveHighScore(const std::string& playerName, int score) {
    highScores.push_back(std::make_pair(playerName, score));

    if (highScores.size() > 5) {
        auto minScore = highScores.begin();
        for (auto it = highScores.begin(); it != highScores.end(); ++it) {
            if (it->second < minScore->second) {
                minScore = it;
            }
        }
        highScores.erase(minScore);
    }

    SortAndSaveScores();
}

std::vector<std::pair<std::string, int>> HighScoreManager::GetHighScores() const {
    return highScores;
}

void HighScoreManager::SortAndSaveScores() {
    std::sort(highScores.begin(), highScores.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second > b.second;
        });

    std::ofstream file("highscores.txt");
    if (file.is_open()) {
        for (const auto& entry : highScores) {
            file << entry.first << " " << entry.second << std::endl;
        }
        file.close();
    }
}