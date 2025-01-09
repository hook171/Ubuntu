#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <vector>
#include <thread>
#include <sstream>
#include <iomanip>
#include <cstdlib>

class TemperatureLogger {
private:
    std::vector<float> hourData;    
    std::vector<float> dayData;     
    const int hourLimit = 60;       
    const int dayLimit = 24;

    // записываем данные в файл тут
    void logData(const std::string &fileName, const std::string &data) {
        std::ofstream outFile(fileName, std::ios::app);
        if (outFile.is_open()) {
            outFile << data << std::endl;
            outFile.close();
            std::cout << "Data written to: " << fileName << std::endl;
        } else {
            std::cerr << "Ошибка при открытии файла: " << fileName << std::endl;
        }
    }

public:
    void addTemperature(float temp) {
    
        hourData.push_back(temp);

        if (hourData.size() > hourLimit) {
            hourData.erase(hourData.begin());
        }

        dayData.push_back(temp);

        if (dayData.size() > dayLimit) {
            dayData.erase(dayData.begin());
        }

        logData("C:/Users/ARTEM/Documents/GitHub/Ubuntu/4Laba/measurements.log", std::to_string(temp));
    }

    // средняя температура
    float calculateAverage(const std::vector<float> &data) {
        float sum = 0;
        for (float temp : data) {
            sum += temp;
        }
        return data.empty() ? 0 : sum / data.size();
    }

    // средняя температура за прошлый час
    void logHourlyAverage() {
        float avgTemp = calculateAverage(hourData);
        std::time_t now = std::time(0);
        struct tm t;
        localtime_s(&t, &now);
        std::ostringstream oss;
        oss << std::put_time(&t, "%Y-%m-%d %H:%M:%S") << " Hourly Avg Temp: " << avgTemp;
        logData("C:/Users/ARTEM/Documents/GitHub/Ubuntu/4Laba/hourly_average.log", oss.str());
    }

    // средняя температура за день
    void logDailyAverage() {
        float avgTemp = calculateAverage(dayData);
        std::time_t now = std::time(0);
        struct tm t;
        localtime_s(&t, &now);
        std::ostringstream oss;
        oss << std::put_time(&t, "%Y-%m-%d %H:%M:%S") << " Daily Avg Temp: " << avgTemp;
        logData("C:/Users/ARTEM/Documents/GitHub/Ubuntu/4Laba/daily_average.log", oss.str());
    }
};

// симуляция температуры
float simulateTemperature() {
    return 15.0 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (30.0 - 15.0)));
}

int main() {
    srand(time(0)); 

    TemperatureLogger logger;
    
    for (int i = 0; i < 1000; ++i) {

        float temp = simulateTemperature();
        
        logger.addTemperature(temp);

        // Логируем среднее за каждый час
        if (i % 60 == 0) {  // Каждые 60 измерений (каждый час)
            logger.logHourlyAverage();
        }

        // Логируем среднее за каждый день
        if (i % 1440 == 0) {  // Каждые 1440 измерений (каждый день)
            logger.logDailyAverage();
        }

        // Пауза в 1 минуту между измерениями
        std::this_thread::sleep_for(std::chrono::minutes(1));
    }

    return 0;
}
