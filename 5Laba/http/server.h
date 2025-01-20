#ifndef SERVER_H
#define SERVER_H

#include <string>

// Объявление функции для запуска HTTP-сервера
void runSocketServer();

// Объявление функции для получения текущей температуры
std::string get_current_temperature();

// Объявление функции для получения статистики за период
std::string get_statistics(const std::string& start, const std::string& end);

#endif // SERVER_H