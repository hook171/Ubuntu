#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>  // Для rand() и srand()
#include <ctime>    // Для time()
#include "my_serial.hpp"

float simulateTemperature() {
    return 15.0 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (30.0 - 15.0)));
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [port]" << std::endl;
        return -1;
    }

    srand(time(0));  // Инициализация генератора случайных чисел

    // Открываем COM-порт
    cplib::SerialPort serialPort(argv[1], cplib::SerialPort::BAUDRATE_115200);
    if (!serialPort.IsOpen()) {
        std::cerr << "Failed to open port " << argv[1] << std::endl;
        return -1;
    }

    std::cout << "Port " << argv[1] << " opened successfully." << std::endl;

    while (true) {
        // Генерация случайной температуры
        float temp = simulateTemperature();
        std::string tempStr = std::to_string(temp) + "\n";  // Преобразуем в строку и добавляем символ новой строки

        // Отправляем данные через порт
        if (serialPort.Write(tempStr) == cplib::SerialPort::RE_OK) {
            std::cout << "Sent: " << tempStr;
        } else {
            std::cerr << "Failed to write to port" << std::endl;
        }

        // Задержка между отправкой данных
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}