#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <sqlite3.h>
#include <string>

using namespace boost::asio;
using ip::tcp;

bool stop_game_flag = false;
bool start_game_flag = false;

std::mutex start_game_mutex;
std::mutex stop_game_mutex;

// SQL-запрос SELECT
std::vector<std::string> execute_sql_select(const std::string& sql) {
    std::vector<std::string> results;

    sqlite3* db;
    char* zErrMsg = nullptr;
    int rc;

    // Открыть соединение с базой данных SQLite
    rc = sqlite3_open("database.bd", &db);

    if (rc) {
        std::cerr << "Ошибка при открытии базы данных: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return results;
    }
    else {
        std::cout << "База данных открыта успешно." << std::endl;
    }

    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка при подготовке SQL запроса: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return results;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::string row_data;
        int columns = sqlite3_column_count(stmt);

        for (int i = 0; i < columns; ++i) {
            const char* column_text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row_data += column_text ? column_text : "NULL";
            if (i < columns - 1)
                row_data += ", ";
        }

        results.push_back(row_data);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return results;
}

// Функция для выполнения SQL-запроса DELETE
void execute_sql_update(const std::string& sql) {
    sqlite3* db;
    char* zErrMsg = nullptr;
    int rc;

    rc = sqlite3_open("database.bd", &db);

    if (rc) {
        std::cerr << "Ошибка при открытии базы данных: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка при выполнении SQL запроса: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(db);
}

// Функция для обработки подключения клиента к серверу
void handle_connection(tcp::socket socket) {
    try {
        while (true) {
            std::array<char, 128> buf;
            boost::system::error_code error;
            size_t len = socket.read_some(buffer(buf), error);

            if (error == error::eof) {
                std::cout << "Соединение закрыто клиентом" << std::endl;
                break;
            }
            else if (error) {
                throw boost::system::system_error(error);
            }
            else {
                std::string request(buf.data(), len);
                std::cout << "Принято от клиента: " << request << std::endl;

                 if (request == "START_GAME") {
                    std::cout << "Sending command to start the game to the client...\n";
                    std::string response = "START_GAME\n";
                    boost::asio::write(socket, boost::asio::buffer(response));
                 }
                else if (request == "OFF_SERVER") {
                    std::cout << "Завершение работы сервера..." << std::endl;
                    std::string response = "Сервер завершает работу\n";
                    write(socket, buffer(response));
                    socket.close();
                    return;
                }
                else if (request == "SELECT_RESULTS") {
                    std::string sql = "SELECT * FROM results;";
                    std::vector<std::string> query_results = execute_sql_select(sql);

                    std::string response;
                    for (const auto& result : query_results) {
                        response += result + "\n";
                    }

                    write(socket, buffer(response));
                    std::cout << "Результаты отправлены клиенту" << std::endl;
                }
                else if (request.find("ADD_RESULT") == 0) {
                     size_t pos = request.find(" ");
                     if (pos != std::string::npos) {
                         std::istringstream iss(request.substr(pos + 1));
                         std::string playerName;
                         int score;
                         if (iss >> playerName >> score) {
                             std::string sql = "INSERT INTO results (name, result) VALUES ('" + playerName + "', " + std::to_string(score) + ")";
                             execute_sql_update(sql);
                             std::string response = "Результат успешно добавлен в базу данных\n";
                             write(socket, buffer(response));
                         }
                         else {
                             std::string response = "Ошибка при чтении данных\n";
                             write(socket, buffer(response));
                         }
                     }
                     else {
                         std::string response = "Неверный формат запроса\n";
                         write(socket, buffer(response));
                     }
                 }
                else if (request == "CLEAR_RESULTS") {
                    std::cout << "Очистка базы данных от результатов..." << std::endl;

                    std::string sql = "DELETE FROM results";
                    execute_sql_update(sql);

                    std::cout << "База данных успешно очищена" << std::endl;

                    std::string response = "База данных успешно очищена\n";
                    write(socket, buffer(response));
                }
                else {
                    std::cout << "Введен несуществующий запрос " << std::endl;
                    std::string response = "Введен несуществующий запрос\n";
                    write(socket, buffer(response));
                }
            }
        }
    }
    catch (std::exception& e) {
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    try {
        boost::asio::io_context io_context;

        // Создание акцептора для принятия входящих соединений
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 9999));
        acceptor.set_option(tcp::acceptor::reuse_address(true));

        std::cout << "Сервер запущен. Ожидание подключений..." << std::endl;

        // Принятие и обработка входящих соединений
        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::cout << "Принято новое соединение" << std::endl;

            // Запуск обработчика для каждого соединения в отдельном потоке
            std::thread(handle_connection, std::move(socket)).detach();
        }
    }
    catch (std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
