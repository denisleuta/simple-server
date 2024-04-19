#include <iostream>
#include <boost/asio.hpp>
#include <sqlite3.h>
#include <vector>

using namespace boost::asio;
using ip::tcp;

// Функция для выполнения запросов SELECT к базе данных SQLite
std::vector<std::string> execute_sql_select(const std::string& sql) {
    std::vector<std::string> results;

    sqlite3* db;
    char* zErrMsg = nullptr;
    int rc;

    // Открыть соединение с базой данных SQLite
    rc = sqlite3_open("ZmeykaResults.db;", &db);

    if (rc) {
        std::cerr << "Ошибка при открытии базы данных: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return results; // Возвращаем пустой вектор в случае ошибки
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
int main() {
    setlocale(LC_ALL, "RU");
    try {
        // Создаем io_context для управления вводом/выводом
        io_context io_context;

        // Создаем TCP acceptor для прослушивания новых соединений
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 9999));

        std::cout << "Сервер запущен. Ожидание подключений..." << std::endl;

        while (true) {
            // Создаем сокет для нового соединения
            tcp::socket socket(io_context);

            // Ожидаем новое подключение
            acceptor.accept(socket);

            std::cout << "Принято новое соединение" << std::endl;

            // Читаем данные из сокета
            std::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(buffer(buf), error);

            if (error == error::eof) {
                std::cout << "Соединение закрыто клиентом" << std::endl;
            }
            else if (error) {
                throw boost::system::system_error(error); // Бросаем исключение в случае ошибки
            }
            else {
                std::string request(buf.data(), len);
                std::cout << "Принято от клиента: " << request << std::endl;

                // Выполняем запрос SELECT к базе данных
                std::string sql = "SELECT * FROM results";
                std::vector<std::string> query_results = execute_sql_select(sql);

                std::string sql1 = "SELECT name FROM results";
                std::vector<std::string> query_results1 = execute_sql_select(sql1);

                // Отправляем результаты клиенту
                std::string response;
                for (const auto& result : query_results) {
                    response += result + "\n";
                }

                // Отправляем результаты клиенту
                std::string response1;
                for (const auto& result1 : query_results1) {
                    response += result1 + "\n";
                }

                write(socket, buffer(response));
                std::cout << "Результаты отправлены клиенту" << std::endl;

                write(socket, buffer(response1));
                std::cout << "Результаты2 отправлены клиенту" << std::endl;
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
