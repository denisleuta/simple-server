#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>
#include <memory>
#include <sqlite3.h>

using namespace boost::asio;
using ip::tcp;
using std::string;

// Функция для обработки входящего запроса клиента
void handle_client(std::shared_ptr<tcp::socket> socket, sqlite3* db) {
    try {
        boost::asio::streambuf buf;
        boost::system::error_code error;

        // Чтение данных от клиента в буфер
        size_t len = socket->read_some(boost::asio::buffer(buf), error);

        if (error == boost::asio::error::eof)
            return; // Соединение закрыто клиентом
        else if (error)
            throw boost::system::system_error(error);

        // Получение данных из буфера
        std::istream is(&buf);
        string message;
        is >> message;

        // Пример обработки запроса: выполнение SQL-запроса к базе данных SQLite
        const char* sql = message.c_str();
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

        if (rc != SQLITE_OK)
            throw std::runtime_error("Failed to prepare SQL statement");

        // Выполнение запроса
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // Обработка результата запроса
            // Здесь можно отправить данные обратно клиенту через сокет
        }

        sqlite3_finalize(stmt); // Освобождение ресурсов

        // Отправка ответа клиенту (в примере просто эхо)
        boost::asio::write(*socket, buffer("Received: " + message));
    }
    catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << std::endl;
    }
}

// Функция для прослушивания подключений и создания обработчиков клиентов
void start_server(boost::asio::io_service& io_service, short port, sqlite3* db) {
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

    while (true) {
        auto socket = std::make_shared<tcp::socket>(io_service);
        acceptor.accept(*socket);

        // Создание нового потока для обработки клиента
        std::thread(handle_client, socket, db).detach();
    }
}

int main() {
    try {
        boost::asio::io_service io_service;
        sqlite3* db;

        // Открытие/создание базы данных SQLite
        int rc = sqlite3_open("DB.db", &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return 1;
        }

        // Запуск сервера на порту 8080
        start_server(io_service, 8080, db);

        // Закрытие базы данных SQLite
        sqlite3_close(db);

    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
