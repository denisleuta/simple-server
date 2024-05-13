#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

int main() {
    setlocale(LC_ALL, "RU");
    try {
        io_context io_context;
        tcp::socket socket(io_context);

        // Подключение к серверу
        socket.connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 9999));

        //Для пользователей других пк
        //socket.connect(tcp::endpoint(ip::address::from_string("172.23.100.47"), 9999)); Нужно ввести ip адрес устройства с которого запустили сервер

        std::cout << "Подключение к серверу успешно" << std::endl;

        while (true) {
            std::string request;
            std::cout << "Введите запрос, например: START_GAME, SELECT_RESULTS, OFF_SERVER, CLEAR_RESULTS: ";
            std::getline(std::cin, request);

            // Отправляем запрос на сервер
            write(socket, buffer(request));

            // Получаем и выводим ответ от сервера
            std::vector<char> response_data(1024); // Выделить достаточно большой буфер для ответа
            size_t response_length = socket.read_some(buffer(response_data)); // Читать данные до тех пор, пока они есть

            // Преобразовать полученные данные в строку и вывести на экран
            std::string response(response_data.begin(), response_data.begin() + response_length);
            std::cout << "Ответ от сервера:\n" << response << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}

