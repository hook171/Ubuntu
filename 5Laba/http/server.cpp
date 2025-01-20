#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <fstream>
#include <sstream>
#include "../db/db_handler.h"  // Подключаем заголовочный файл для работы с базой данных

#ifdef _WIN32
    // Windows
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #define close_socket closesocket
    #define read_socket(sock, buf, len, flags) recv(sock, buf, len, flags)
    #define write_socket(sock, buf, len, flags) send(sock, buf, len, flags)
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #define close_socket close
    #define read_socket(sock, buf, len, flags) read(sock, buf, len)
    #define write_socket(sock, buf, len, flags) write(sock, buf, len)
#endif

#ifdef _WIN32
    #define DB_PATH "C:/Users/hook/Documents/GitHub/Ubuntu/5Laba/web-service"
#else
    #define DB_PATH "/home/hook/Documents/GitHub/Ubuntu/5Laba/web-service"
#endif
const int PORT = 8080;  // Порт, на котором будет работать сервер
const int BUFFER_SIZE = 4096;  // Размер буфера для чтения данных

// Функция для чтения файла
std::string read_file(const std::string& path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file) {
        return "";
    }
    std::ostringstream contents;
    contents << file.rdbuf();
    file.close();
    return contents.str();
}

// Функция для получения MIME-типа файла
std::string get_mime_type(const std::string& path) {
    if (path.find(".html") != std::string::npos) {
        return "text/html";
    } else if (path.find(".css") != std::string::npos) {
        return "text/css";
    } else if (path.find(".js") != std::string::npos) {
        return "application/javascript";
    } else {
        return "text/plain";
    }
}

// Функция для получения текущей температуры из базы данных
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

std::string get_statistics(const std::string& start, const std::string& end) {
    std::stringstream ss;

    if (!db) {
        ss << "{\"error\": \"Database is not initialized\"}";
        return ss.str();
    }

    // Заменяем 'T' на пробел
    std::string start_fixed = start;
    std::string end_fixed = end;
    size_t t_pos = start_fixed.find('T');
    if (t_pos != std::string::npos) {
        start_fixed.replace(t_pos, 1, " ");
    }
    t_pos = end_fixed.find('T');
    if (t_pos != std::string::npos) {
        end_fixed.replace(t_pos, 1, " ");
    }

    const char* sql = "SELECT MIN(value), MAX(value), AVG(value) FROM temperature WHERE timestamp BETWEEN ? AND ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, start_fixed.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, end_fixed.c_str(), -1, SQLITE_STATIC);

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
        ss << "{\"error\": \"Database query failed: " << sqlite3_errmsg(db) << "\"}";
    }
    return ss.str();
}

void handle_request(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_read = read_socket(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read < 0) {
        std::cerr << "Failed to read from socket" << std::endl;
        close_socket(client_socket);
        return;
    }

    buffer[bytes_read] = '\0';
    std::string request(buffer);

    // Парсим HTTP-запрос
    std::istringstream iss(request);
    std::string method, path, protocol;
    iss >> method >> path >> protocol;

    // Логируем путь запроса
    std::cout << "Requested path: " << path << std::endl;

    std::string response;

    if (path == "/current") {
        // Возвращаем текущую температуру
        response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: application/json\r\n";
        response += "\r\n";
        response += get_current_temperature();
    } else if (path.find("/stats") == 0) {
        // Парсим параметры запроса
        size_t start_pos = path.find("start=");
        size_t end_pos = path.find("end=");
        if (start_pos != std::string::npos && end_pos != std::string::npos) {
            std::string start = path.substr(start_pos + 6, 19); // 19 символов для формата "YYYY-MM-DDTHH:MM"
            std::string end = path.substr(end_pos + 4, 19);
            
            // Возвращаем статистику
            response = "HTTP/1.1 200 OK\r\n";
            response += "Content-Type: application/json\r\n";
            response += "\r\n";
            response += get_statistics(start, end);
        } else {
            // Если параметры не указаны, возвращаем ошибку
            response = "HTTP/1.1 400 Bad Request\r\n";
            response += "Content-Type: application/json\r\n";
            response += "\r\n";
            response += "{\"error\": \"Missing start or end parameters\"}";
        }
    } else {
        // Обрабатываем статические файлы
        std::string full_path = DB_PATH + path;

        // Если путь пустой, возвращаем index.html
        if (path == "/") {
            full_path = std::string(DB_PATH) + "/index.html";
        }

        // Читаем файл
        std::string file_content = read_file(full_path);

        if (!file_content.empty()) {
            response = "HTTP/1.1 200 OK\r\n";
            response += "Content-Type: " + get_mime_type(full_path) + "\r\n";
            response += "Content-Length: " + std::to_string(file_content.size()) + "\r\n";
            response += "\r\n";
            response += file_content;
        } else {
            // Возвращаем 404, если файл не найден
            response = "HTTP/1.1 404 Not Found\r\n";
            response += "Content-Type: text/plain\r\n";
            response += "Content-Length: 13\r\n";
            response += "\r\n";
            response += "404 Not Found";
        }
    }

    // Отправляем ответ клиенту
    write_socket(client_socket, response.c_str(), static_cast<int>(response.size()), 0);
    close_socket(client_socket);
}

// Функция для запуска HTTP-сервера
void runSocketServer() {
#ifdef _WIN32
    // Инициализация Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return;
    }
#endif

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Создаем сокет
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        return;
    }

    // Настраиваем адрес сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязываем сокет к адресу
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        close_socket(server_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return;
    }

    // Прослушиваем входящие соединения
    if (listen(server_socket, 10) < 0) {
        std::cerr << "Failed to listen on socket" << std::endl;
        close_socket(server_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return;
    }

    std::cout << "HTTP server is running on port " << PORT << std::endl;

    while (true) {
        // Принимаем входящее соединение
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }

        // Обрабатываем запрос
        handle_request(client_socket);
    }

    close_socket(server_socket);

#ifdef _WIN32
    WSACleanup();
#endif
}