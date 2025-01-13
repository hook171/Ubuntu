#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "my_serial.hpp"

// Функция для генерации случайной температуры
float simulateTemperature() {
    return 15.0 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (30.0 - 15.0)));
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [port]" << std::endl;
        return -1;
    }

    srand(time(0));

    // Открываем последовательный порт
    cplib::SerialPort serialPort(argv[1], cplib::SerialPort::BAUDRATE_115200);
    if (!serialPort.IsOpen()) {
        std::cerr << "Failed to open port " << argv[1] << std::endl;
        return -1;
    }

    // Основной цикл симуляции
    while (true) {
        // Генерируем температуру
        float temp = simulateTemperature();
        std::string tempStr = std::to_string(temp) + "\n";

        // Отправляем температуру в порт
        serialPort.Write(tempStr);

        // Пауза между измерениями
        std::this_thread::sleep_for(std::chrono::minutes(1)); // Тестировал на секундах
    }

    return 0;
}