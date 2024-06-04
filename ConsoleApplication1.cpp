#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include "Game.h"

using namespace boost::asio;
using ip::tcp;

// Функция для отправки результатов игры на сервер
void send_results_to_server(const std::string& playerName, int score) {
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket(io_context);

    socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 9999));

    std::string message = "ADD_RESULT " + playerName + " " + std::to_string(score);
    boost::asio::write(socket, boost::asio::buffer(message));

    socket.close();
}

int main() {
    setlocale(LC_ALL, "RU");
    try {

        io_context io_context;
        tcp::socket socket(io_context);

        socket.connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 9999));

        std::cout << "Привет друг!" << std::endl;
        std::cout << "Представляем тебе наш сервер с игрой - Змеечка." << std::endl;

        std::cout << "Инструкция:" << std::endl;
        std::cout << "1. START_GAME - начать играть." << std::endl;
        std::cout << "2. SELECT_RESULTS - запросить результаты предыдущих игроков." << std::endl;
        std::cout << "3. CLEAR_RESULTS - удалить результаты предыдущих игроков." << std::endl;
        std::cout << "4. OFF_SERVER - выключить сервер." << std::endl;

        while (true) {
            std::string request;
            std::cout << "Введите запрос: ";
            std::getline(std::cin, request);

            write(socket, buffer(request));

            std::vector<char> response_data(1024);
            size_t response_length = socket.read_some(buffer(response_data)); 

            std::string response(response_data.begin(), response_data.begin() + response_length);
            std::cout << "Ответ от сервера:\n" << response << std::endl;

            if (request == "START_GAME") {
                Game::start_game();
            }

            // Очищаем буфер ввода
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    catch (std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
