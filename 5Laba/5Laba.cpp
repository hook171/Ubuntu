#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "sqlite3.h"
#include "httplib.h"

// Класс для логирования температуры
class TemperatureLogger {
private:
    sqlite3* db; // Указатель на базу данных

public:
    TemperatureLogger(sqlite3* database) : db(database) {}

    void addTemperature(float temp) {
        logDataToDatabase(temp); // Логируем в базу данных
    }

    void logDataToDatabase(float temperature) {
        const char* insertQuery = "INSERT INTO temperature (timestamp, temperature) VALUES (datetime('now'), ?);";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, insertQuery, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        sqlite3_bind_double(stmt, 1, temperature);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Ошибка выполнения запроса: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
    }
};

// Функция инициализации базы данных
void initializeDatabase(sqlite3* db) {
    const char* createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS temperature (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT NOT NULL,
            temperature REAL NOT NULL
        );
    )";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, createTableQuery, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Ошибка создания таблицы: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Функция для запуска HTTP-сервера
void runServer(sqlite3* db) {
    httplib::Server svr;

    // Эндпоинт для получения текущей температуры
    svr.Get("/temperature/current", [&](const httplib::Request&, httplib::Response& res) {
        const char* query = "SELECT temperature FROM temperature ORDER BY id DESC LIMIT 1;";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
            res.set_content("<html><head><meta charset=\"UTF-8\"></head><body><h1>Ошибка подготовки запроса</h1></body></html>", "text/html; charset=UTF-8");
            return;
        }

        std::ostringstream htmlContent;
        htmlContent << "<html><head><meta charset=\"UTF-8\"><title>Текущая температура</title></head><body>";
        htmlContent << "<h1>Текущая температура</h1>";

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            float temp = sqlite3_column_double(stmt, 0);
            htmlContent << "<p>Текущая температура: " << temp << "°C</p>";
        } else {
            htmlContent << "<p>Нет данных</p>";
        }

        htmlContent << "</body></html>";
        res.set_content(htmlContent.str(), "text/html; charset=UTF-8");

        sqlite3_finalize(stmt);
    });

    // Эндпоинт для получения статистики (например, средней температуры)
    svr.Get("/temperature/average", [&](const httplib::Request&, httplib::Response& res) {
        const char* query = "SELECT AVG(temperature) FROM temperature;";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
            res.set_content("<html><head><meta charset=\"UTF-8\"></head><body><h1>Ошибка подготовки запроса</h1></body></html>", "text/html; charset=UTF-8");
            return;
        }

        std::ostringstream htmlContent;
        htmlContent << "<html><head><meta charset=\"UTF-8\"><title>Средняя температура</title></head><body>";
        htmlContent << "<h1>Средняя температура</h1>";

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            float avgTemp = sqlite3_column_double(stmt, 0);
            htmlContent << "<p>Средняя температура: " << avgTemp << "°C</p>";
        } else {
            htmlContent << "<p>Нет данных</p>";
        }

        htmlContent << "</body></html>";
        res.set_content(htmlContent.str(), "text/html; charset=UTF-8");

        sqlite3_finalize(stmt);
    });

    svr.listen("localhost", 8080);
}

// Функция для симуляции температуры
float simulateTemperature() {
    return 15.0 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (30.0 - 15.0)));
}

int main() {
    srand(time(0));

    // Открываем базу данных
    sqlite3* db;
    if (sqlite3_open("temperature.db", &db) != SQLITE_OK) {
        std::cerr << "Ошибка открытия базы данных: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // Инициализируем базу данных
    initializeDatabase(db);

    // Создаём логгер
    TemperatureLogger logger(db);

    // Поток для сбора данных
    std::thread dataThread([&]() {
        for (int i = 0; i < 1000; ++i) {
            float temp = simulateTemperature();
            logger.addTemperature(temp);

            // Пауза в 1 минуту между измерениями
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    // Запускаем сервер
    runServer(db);

    // Дожидаемся завершения потока
    dataThread.join();

    // Закрываем базу данных
    sqlite3_close(db);

    return 0;
}
