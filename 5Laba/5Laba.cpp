#include <winsock2.h> // Подключаем winsock2.h до windows.h
#include <windows.h>
#include <sqlite3.h>
#include <httplib.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include "my_serial.hpp" // Оставляем, если это заголовочный файл
#include "db/db_handler.h" // Подключаем заголовочный файл для работы с базой данных
#include "http/server.h"   // Подключаем заголовочный файл для HTTP-сервера

// Функция для очистки строки от нечитаемых символов
std::string cleanString(const std::string& input) {
    std::string result;
    for (char ch : input) {
        if (std::isdigit(ch) || ch == '.' || ch == '-') {
            result += ch;
        }
    }
    return result;
}

// Функция для запуска HTTP-сервера в отдельном потоке
void startHttpServer() {
    std::cout << "Starting HTTP server..." << std::endl;
    runHttpServer(); // Функция из http/server.cpp
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [port]" << std::endl;
        return -1;
    }

    // Инициализация базы данных
    init_db();

    // Запуск HTTP-сервера в отдельном потоке
    std::thread httpThread(startHttpServer);
    httpThread.detach(); // Отделяем поток, чтобы он работал независимо

    // Открываем COM-порт
    cplib::SerialPort serialPort(argv[1], cplib::SerialPort::BAUDRATE_115200);
    if (!serialPort.IsOpen()) {
        std::cerr << "Failed to open port " << argv[1] << std::endl;
        return -1;
    }

    std::cout << "Port " << argv[1] << " opened successfully." << std::endl;

    // Устанавливаем таймаут для чтения
    serialPort.SetTimeout(1.0);  // Таймаут 1 секунда

    int count = 0;  // Счётчик полученных значений

    while (true) {
        char buffer[256];  // Буфер на 256 байт
        size_t read = 0;

        // Читаем данные из порта
        int result = serialPort.Read(buffer, sizeof(buffer) - 1, &read);
        if (result == cplib::SerialPort::RE_OK) {
            buffer[read] = '\0';  // Добавляем нулевой символ для корректного вывода строки
            std::string data(buffer);

            // Очищаем данные от нечитаемых символов
            std::string cleanData = cleanString(data);
            std::cout << "Received: " << cleanData << std::endl;

            try {
                // Преобразуем данные в число
                float temp = std::stof(cleanData);

                // Записываем температуру в базу данных
                write_temperature(temp);
                count++;

            } catch (const std::invalid_argument& e) {
                std::cerr << "Error converting string to float: " << cleanData << std::endl;
            }
        } else {
            std::cerr << "Read failed with error code: " << result << std::endl;
        }

        // Пауза между измерениями
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Закрытие базы данных (этот код никогда не выполнится из-за бесконечного цикла)
    close_db();

    return 0;
}