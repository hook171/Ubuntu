#include "server.h"
#include <httplib.h>
#include "sqlite3.h"
#include <iostream>
#include <sstream>
#include <iomanip>

extern sqlite3* db; // Используем внешнюю переменную db

// Функция для получения текущей температуры
std::string get_current_temperature() {
    std::stringstream ss;
    const char* sql = "SELECT value, timestamp FROM temperature ORDER BY timestamp DESC LIMIT 1;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            double value = sqlite3_column_double(stmt, 0);
            const char* timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            ss << "{\"temperature\": " << value << ", \"timestamp\": \"" << timestamp << "\"}";
        } else {
            ss << "{\"error\": \"No data available\"}";
        }
        sqlite3_finalize(stmt);
    } else {
        ss << "{\"error\": \"Database query failed\"}";
    }
    return ss.str();
}

// Функция для получения статистики за период
std::string get_statistics(const std::string& start, const std::string& end) {
    std::stringstream ss;
    const char* sql = "SELECT MIN(value), MAX(value), AVG(value) FROM temperature WHERE timestamp BETWEEN ? AND ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, start.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, end.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            double min = sqlite3_column_double(stmt, 0);
            double max = sqlite3_column_double(stmt, 1);
            double avg = sqlite3_column_double(stmt, 2);
            ss << "{\"min\": " << min << ", \"max\": " << max << ", \"avg\": " << avg << "}";
        } else {
            ss << "{\"error\": \"No data available for the specified period\"}";
        }
        sqlite3_finalize(stmt);
    } else {
        ss << "{\"error\": \"Database query failed\"}";
    }
    return ss.str();
}

void runHttpServer() {
    std::cout << "HTTP server is starting..." << std::endl;  // Лог
    httplib::Server svr;

    // Настройка эндпоинтов
    svr.set_mount_point("/", "./web-service");
    svr.Get("/current", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(get_current_temperature(), "application/json");
    });
    svr.Get("/stats", [](const httplib::Request& req, httplib::Response& res) {
        std::string start = req.get_param_value("start");
        std::string end = req.get_param_value("end");
        if (start.empty() || end.empty()) {
            res.set_content("{\"error\": \"Missing start or end parameters\"}", "application/json");
        } else {
            res.set_content(get_statistics(start, end), "application/json");
        }
    });

    std::cout << "Starting HTTP server on port 8080..." << std::endl;
    if (!svr.listen("localhost", 8080)) {
        std::cerr << "Failed to start HTTP server on port 8080!" << std::endl;
    }

    svr.listen("localhost", 8080);
}