#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <vector>
#include <thread>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cctype>  // для std::isdigit
#include "my_serial.hpp"

#ifdef _WIN32
    #define LOG_FILE_PATH "C:/Users/ARTEM/Documents/GitHub/Ubuntu/4Laba/Tests/measurements.log"
#else
    #define LOG_FILE_PATH "/home/hook/Documents/GitHub/Ubuntu/4Laba/Tests/measurements.log"
#endif

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

class TemperatureLogger {
private:
    std::vector<float> measurements;  // Хранит последние 24 * 60 измерений
    std::vector<float> hourlyAverages;  // Хранит последние 24 * 30 средних за час
    std::vector<float> dailyAverages;  // Хранит последние 365 средних за день

    const int measurementsLimit = 24 * 60;  // 1440 измерений (24 часа * 60 минут)
    const int hourlyAveragesLimit = 24 * 30;  // 720 средних за час (30 дней * 24 часа)
    const int dailyAveragesLimit = 365;  // 365 средних за день (1 год)

    // Логирование данных в файл
    void logData(const std::string &fileName, const std::string &data) {
        std::ofstream outFile(fileName, std::ios::app);
        if (outFile.is_open()) {
            outFile << data << std::endl;
            outFile.close();
            std::cout << "Data written to: " << fileName << std::endl;
        } else {
            std::cerr << "Error opening file: " << fileName << std::endl;
        }
    }

public:
    // Добавление температуры в лог
    void addTemperature(float temp) {
        measurements.push_back(temp);
        if (measurements.size() > measurementsLimit) {
            measurements.erase(measurements.begin());
        }

        // Логируем текущее измерение
        logData(LOG_FILE_PATH, std::to_string(temp));
    }

    // Вычисление среднего значения
    float calculateAverage(const std::vector<float> &data) {
        float sum = 0;
        for (float temp : data) {
            sum += temp;
        }
        return data.empty() ? 0 : sum / data.size();
    }

    // Логирование среднего за час
    void logHourlyAverage() {
        float avgTemp = calculateAverage(measurements);  // Среднее за последний час
        hourlyAverages.push_back(avgTemp);
        if (hourlyAverages.size() > hourlyAveragesLimit) {
            hourlyAverages.erase(hourlyAverages.begin());
        }

        std::time_t now = std::time(0);
        struct tm t;
#ifdef _WIN32
        localtime_s(&t, &now);
#else
        localtime_r(&now, &t);
#endif
        std::ostringstream oss;
        oss << std::put_time(&t, "%Y-%m-%d %H:%M:%S") << " Hourly Avg Temp: " << avgTemp;
        logData(LOG_FILE_PATH, oss.str());
    }

    // Логирование среднего за день
    void logDailyAverage() {
        float avgTemp = calculateAverage(hourlyAverages);  // Среднее за последний день
        dailyAverages.push_back(avgTemp);
        if (dailyAverages.size() > dailyAveragesLimit) {
            dailyAverages.erase(dailyAverages.begin());
        }

        std::time_t now = std::time(0);
        struct tm t;
#ifdef _WIN32
        localtime_s(&t, &now);
#else
        localtime_r(&now, &t);
#endif
        std::ostringstream oss;
        oss << std::put_time(&t, "%Y-%m-%d %H:%M:%S") << " Daily Avg Temp: " << avgTemp;
        logData(LOG_FILE_PATH, oss.str());
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [port]" << std::endl;
        return -1;
    }

    TemperatureLogger logger;

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

                // Добавляем температуру в лог
                logger.addTemperature(temp);
                count++;

                // Логируем среднее за каждый час
                if (count % 60 == 0) {
                    logger.logHourlyAverage();
                }

                // Логируем среднее за каждый день
                if (count % 1440 == 0) {
                    logger.logDailyAverage();
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error converting string to float: " << cleanData << std::endl;
            }
        } else {
            std::cerr << "Read failed with error code: " << result << std::endl;
        }

        // Пауза между измерениями
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}