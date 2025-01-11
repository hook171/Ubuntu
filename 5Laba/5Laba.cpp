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

    // Эндпоинт для получения статистики
    svr.Get("/temperature/data", [&](const httplib::Request& req, httplib::Response& res) {
        std::ostringstream htmlContent;
        htmlContent << "<html><head><meta charset=\"UTF-8\"><title>Температура</title></head><body>";
        htmlContent << "<h1>Данные о температуре</h1>";

        // Форма выбора периода
        htmlContent << "<form action=\"/temperature/data\" method=\"get\">"
                    << "<label for=\"period\">Выберите период:</label>"
                    << "<select name=\"period\" id=\"period\">"
                    << "<option value=\"hour\">Последний час</option>"
                    << "<option value=\"day\">Последний день</option>"
                    << "</select>"
                    << "<input type=\"submit\" value=\"Показать статистику\">"
                    << "</form>";

        // Получаем параметр period из запроса
        std::string period = req.get_param_value("period");

        // Текущая температура
        const char* currentTempQuery = "SELECT temperature FROM temperature ORDER BY id DESC LIMIT 1;";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, currentTempQuery, -1, &stmt, nullptr) != SQLITE_OK) {
            res.set_content("<html><head><meta charset=\"UTF-8\"></head><body><h1>Ошибка подготовки запроса</h1></body></html>", "text/html; charset=UTF-8");
            return;
        }

        htmlContent << "<h2>Текущая температура</h2>";
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            float temp = sqlite3_column_double(stmt, 0);
            htmlContent << "<p>" << temp << "°C</p>";
        } else {
            htmlContent << "<p>Нет данных</p>";
        }

        sqlite3_finalize(stmt);

        // Средняя температура
        const char* avgTempQuery;
        if (period == "hour") {
            avgTempQuery = "SELECT AVG(temperature) FROM temperature WHERE timestamp >= datetime('now', '-1 hour');";
        } else if (period == "day") {
            avgTempQuery = "SELECT AVG(temperature) FROM temperature WHERE timestamp >= datetime('now', '-1 day');";
        } else {
            avgTempQuery = "SELECT AVG(temperature) FROM temperature;";
        }

        if (sqlite3_prepare_v2(db, avgTempQuery, -1, &stmt, nullptr) != SQLITE_OK) {
            res.set_content("<html><head><meta charset=\"UTF-8\"></head><body><h1>Ошибка подготовки запроса</h1></body></html>", "text/html; charset=UTF-8");
            return;
        }

        htmlContent << "<h2>Средняя температура</h2>";
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            float avgTemp = sqlite3_column_double(stmt, 0);
            htmlContent << "<p>" << avgTemp << "°C</p>";
        } else {
            htmlContent << "<p>Нет данных</p>";
        }

        sqlite3_finalize(stmt);

        // Таблица с данными о температуре
        htmlContent << "<h2>История температур</h2>";
        const char* tableQuery;
        if (period == "hour") {
            tableQuery = "SELECT timestamp, temperature FROM temperature WHERE timestamp >= datetime('now', '-1 hour') ORDER BY id DESC LIMIT 100;";
        } else if (period == "day") {
            tableQuery = "SELECT timestamp, temperature FROM temperature WHERE timestamp >= datetime('now', '-1 day') ORDER BY id DESC LIMIT 100;";
        } else {
            tableQuery = "SELECT timestamp, temperature FROM temperature ORDER BY id DESC LIMIT 10;";
        }

        if (sqlite3_prepare_v2(db, tableQuery, -1, &stmt, nullptr) != SQLITE_OK) {
            res.set_content("<html><head><meta charset=\"UTF-8\"></head><body><h1>Ошибка подготовки запроса</h1></body></html>", "text/html; charset=UTF-8");
            return;
        }

        htmlContent << "<table border=\"1\"><tr><th>Время</th><th>Температура</th></tr>";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            float temperature = sqlite3_column_double(stmt, 1);
            htmlContent << "<tr><td>" << timestamp << "</td><td>" << temperature << "°C</td></tr>";
        }
        htmlContent << "</table>";

        sqlite3_finalize(stmt);

        htmlContent << "</body></html>";
        res.set_content(htmlContent.str(), "text/html; charset=UTF-8");

    });

    svr.listen("localhost", 8080);
}

// Функция для симуляции температуры
float simulateTemperature() {
    return 15.0 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (30.0 - 15.0)));
}

#include <cstdlib> // Для std::system

int main() {
    srand(time(0));

    // Путь к базе данных
    const char* dbPath = "C:/Users/ARTEM/Documents/GitHub/Ubuntu/5Laba/temperature.db";

    // Открываем базу данных
    sqlite3* db;
    if (sqlite3_open(dbPath, &db) != SQLITE_OK) {
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
            std::this_thread::sleep_for(std::chrono::minutes(1)); // в таблице данные с ежесекундным обновлением, для теста
        }
    });

    // Запускаем сервер в отдельном потоке
    std::thread serverThread([&]() {
        runServer(db);
    });

    // Открываем вкладку в браузере
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Даем время серверу запуститься
    std::system("start http://localhost:8080/temperature/data");

    // Дожидаемся завершения потоков
    dataThread.join();
    serverThread.join();

    // Закрываем базу данных
    sqlite3_close(db);

    return 0;
}