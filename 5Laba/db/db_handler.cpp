#include "sqlite3.h" // Теперь этот заголовочный файл будет найден через CMake
#include <iostream>
#include <string>

#ifdef _WIN32
    #define LOG_FILE_PATH "C:/Users/ARTEM/Documents/GitHub/Ubuntu/5Laba/db/database.db"
#else
    #define LOG_FILE_PATH "/home/hook/Documents/GitHub/Ubuntu/5Laba/db/database.db"
#endif

sqlite3* db;

void init_db() {
    int rc = sqlite3_open(LOG_FILE_PATH, &db);
    if (rc) {
        std::cerr << "Ошибка открытия базы данных: " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    } else {
        std::cout << "Database opened successfully." << std::endl;  // Лог
    }

    const char* sql = "CREATE TABLE IF NOT EXISTS temperature (id INTEGER PRIMARY KEY, value REAL, timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";
    rc = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка создания таблицы: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Table created or already exists." << std::endl;  // Лог
    }
}

void write_temperature(float temp) {
    std::string sql = "INSERT INTO temperature (value) VALUES (" + std::to_string(temp) + ");";
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка записи данных: " << sqlite3_errmsg(db) << std::endl;
    }
}

void close_db() {
    sqlite3_close(db);
}