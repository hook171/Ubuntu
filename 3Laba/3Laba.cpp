#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <atomic>
#include <sstream>
#include <iomanip>
#include <fstream>

// Определение ОС
#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
    #include <windows.h>
#else
    #define OS_LINUX
    #include <unistd.h>       // Для fork, getpid
    #include <sys/wait.h>     // Для waitpid
#endif

#include "my_shmem.hpp" 

using namespace std;
using namespace cplib;

// Структура для хранения данных в разделяемой памяти
struct SharedData {
    int counter;  // Счетчик
};

// Глобальные переменные
SharedMem<SharedData>* sharedMem = nullptr;
atomic<bool> running(true);  // Флаг для управления потоками

// Функция для получения текущего времени в формате строки
string getCurrentTime() {
    auto now = chrono::system_clock::now();
    auto now_time_t = chrono::system_clock::to_time_t(now);
    auto now_ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;

    tm local_tm;
#if defined(OS_WINDOWS)
    localtime_s(&local_tm, &now_time_t);  // Используем localtime_s для Windows
#else
    localtime_r(&now_time_t, &local_tm);  // Используем localtime_r для Linux
#endif

    stringstream ss;
    ss << put_time(&local_tm, "%Y-%m-%d %H:%M:%S") << "." << setw(3) << setfill('0') << now_ms.count();
    return ss.str();
}

// Функция для записи в лог-файл
void logToFile(const string& message) {
    ofstream logfile("/home/hook/Documents/GitHub/Ubuntu/3Laba/Logs/logs.txt", ios::app);  // Путь к лог-файлу
    if (logfile.is_open()) {
        logfile << message << endl;
        logfile.close();
    } else {
        cerr << "Ошибка открытия лог-файла!" << endl;
    }
}

// Поток для увеличения счетчика каждые 300 мс
void incrementCounter() {
    while (running) {
        this_thread::sleep_for(chrono::milliseconds(300));
        sharedMem->Lock();
        sharedMem->Data()->counter++;
        sharedMem->Unlock();
    }
}

// Поток для записи в лог каждую секунду
void logCounter() {
    while (running) {
        this_thread::sleep_for(chrono::seconds(1));
        sharedMem->Lock();
        int counter = sharedMem->Data()->counter;
        string logMessage = getCurrentTime() + " [PID: " + to_string(
#if defined(OS_WINDOWS)
            GetCurrentProcessId()
#else
            getpid()
#endif
        ) + "] [MAIN] Текущее значение счетчика: " + to_string(counter);
        logToFile(logMessage);
        sharedMem->Unlock();
    }
}

// Функция для выполнения задачи копии 1
void copy1Task() {
    sharedMem->Lock();
    int oldValue = sharedMem->Data()->counter;
    sharedMem->Data()->counter += 10;
    int newValue = sharedMem->Data()->counter;
    string logMessage = getCurrentTime() + " [PID: " + to_string(
#if defined(OS_WINDOWS)
        GetCurrentProcessId()
#else
        getpid()
#endif
    ) + "] [COPY 1] Увеличиваю счетчик на 10. Старое значение: " + to_string(oldValue) + ", новое значение: " + to_string(newValue);
    logToFile(logMessage);
    sharedMem->Unlock();
}

// Функция для выполнения задачи копии 2
void copy2Task() {
    sharedMem->Lock();
    int oldValue = sharedMem->Data()->counter;
    sharedMem->Data()->counter *= 2;
    int newValue = sharedMem->Data()->counter;
    string logMessage = getCurrentTime() + " [PID: " + to_string(
#if defined(OS_WINDOWS)
        GetCurrentProcessId()
#else
        getpid()
#endif
    ) + "] [COPY 2] Умножаю счетчик на 2. Старое значение: " + to_string(oldValue) + ", новое значение: " + to_string(newValue);
    logToFile(logMessage);
    sharedMem->Unlock();

    this_thread::sleep_for(chrono::seconds(2));

    sharedMem->Lock();
    oldValue = sharedMem->Data()->counter;
    sharedMem->Data()->counter /= 2;
    newValue = sharedMem->Data()->counter;
    logMessage = getCurrentTime() + " [PID: " + to_string(
#if defined(OS_WINDOWS)
        GetCurrentProcessId()
#else
        getpid()
#endif
    ) + "] [COPY 2] Делю счетчик на 2. Старое значение: " + to_string(oldValue) + ", новое значение: " + to_string(newValue);
    logToFile(logMessage);
    sharedMem->Unlock();
}

// Поток для запуска копий каждые 3 секунды
void spawnCopies() {
    while (running) {
        this_thread::sleep_for(chrono::seconds(3));

#if defined(OS_WINDOWS)
        // Запуск копии 1 (Windows)
        STARTUPINFO si1 = { sizeof(si1) };
        PROCESS_INFORMATION pi1;
        if (CreateProcess(NULL, "3Laba.exe Copy1", NULL, NULL, FALSE, 0, NULL, NULL, &si1, &pi1)) {
            string logMessage = getCurrentTime() + " [PID: " + to_string(GetCurrentProcessId()) + "] [MAIN] Запущена COPY 1.";
            logToFile(logMessage);
            WaitForSingleObject(pi1.hProcess, INFINITE);
            CloseHandle(pi1.hProcess);
            CloseHandle(pi1.hThread);
        }

        // Запуск копии 2 (Windows)
        STARTUPINFO si2 = { sizeof(si2) };
        PROCESS_INFORMATION pi2;
        if (CreateProcess(NULL, "3Laba.exe Copy2", NULL, NULL, FALSE, 0, NULL, NULL, &si2, &pi2)) {
            string logMessage = getCurrentTime() + " [PID: " + to_string(GetCurrentProcessId()) + "] [MAIN] Запущена COPY 2.";
            logToFile(logMessage);
            WaitForSingleObject(pi2.hProcess, INFINITE);
            CloseHandle(pi2.hProcess);
            CloseHandle(pi2.hThread);
        }
#else
        // Запуск копии 1 (Linux)
        pid_t pid1 = fork();
        if (pid1 == 0) {  // Дочерний процесс
            copy1Task();
            exit(0);  // Завершаем дочерний процесс
        } else if (pid1 > 0) {  // Родительский процесс
            string logMessage = getCurrentTime() + " [PID: " + to_string(getpid()) + "] [MAIN] Запущена COPY 1.";
            logToFile(logMessage);
            waitpid(pid1, nullptr, 0);  // Ожидаем завершения дочернего процесса
        }

        // Запуск копии 2 (Linux)
        pid_t pid2 = fork();
        if (pid2 == 0) {  // Дочерний процесс
            copy2Task();
            exit(0);  // Завершаем дочерний процесс
        } else if (pid2 > 0) {  // Родительский процесс
            string logMessage = getCurrentTime() + " [PID: " + to_string(getpid()) + "] [MAIN] Запущена COPY 2.";
            logToFile(logMessage);
            waitpid(pid2, nullptr, 0);  // Ожидаем завершения дочернего процесса
        }
#endif
    }
}

// Основная функция
int main(int argc, char* argv[]) {
    // Инициализация разделяемой памяти
    sharedMem = new SharedMem<SharedData>("shared_counter");

    if (!sharedMem->IsValid()) {
        cerr << "Ошибка инициализации разделяемой памяти!" << endl;
        return 1;
    }

    // Если программа запущена как копия
    if (argc > 1) {
        string mode(argv[1]);
        if (mode == "Copy1") {
            copy1Task();
            return 0;
        } else if (mode == "Copy2") {
            copy2Task();
            return 0;
        }
    }

    // Запись в лог при старте
    string logMessage = getCurrentTime() + " [PID: " + to_string(
#if defined(OS_WINDOWS)
        GetCurrentProcessId()
#else
        getpid()
#endif
    ) + "] [MAIN] Программа запущена.";
    logToFile(logMessage);

    // Запуск потоков
    thread incrementThread(incrementCounter);
    thread logThread(logCounter);
    thread spawnThread(spawnCopies);

    // Интерфейс командной строки
    while (running) {
        cout << "Введите новое значение счетчика (или 'q' для выхода): ";
        string input;
        cin >> input;

        if (input == "q") {
            running = false;
            break;
        }

        try {
            int newValue = stoi(input);
            sharedMem->Lock();
            int oldValue = sharedMem->Data()->counter;
            sharedMem->Data()->counter = newValue;
            string logMessage = getCurrentTime() + " [PID: " + to_string(
#if defined(OS_WINDOWS)
                GetCurrentProcessId()
#else
                getpid()
#endif
            ) + "] [MAIN] Пользователь изменил счетчик. Старое значение: " + to_string(oldValue) + ", новое значение: " + to_string(newValue);
            logToFile(logMessage);
            sharedMem->Unlock();
        } catch (invalid_argument&) {
            cerr << "Некорректный ввод!" << endl;
        }
    }

    // Ожидание завершения потоков
    incrementThread.join();
    logThread.join();
    spawnThread.join();

    // Запись в лог при завершении
    logMessage = getCurrentTime() + " [PID: " + to_string(
#if defined(OS_WINDOWS)
        GetCurrentProcessId()
#else
        getpid()
#endif
    ) + "] [MAIN] Программа завершена.";
    logToFile(logMessage);

    // Освобождение ресурсов
    delete sharedMem;
    return 0;
}