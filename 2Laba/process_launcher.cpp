#include "process_launcher.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace ProcessLauncher {

int launchProcess(const std::string &command) {
#ifdef _WIN32
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi = { 0 };

    // попытка запустить процесс
    if (!CreateProcess(nullptr, const_cast<char *>(command.c_str()), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        std::cerr << "Failed to create process: " << GetLastError() << std::endl;
        return -1;
    }

    // ожидание завершения процесса
    WaitForSingleObject(pi.hProcess, INFINITE);

    // получаем код завершения процесса
    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    // закрываем дескрипторы процесса и потока
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return static_cast<int>(exitCode);

#else
    // для линукса с помощью fork()
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork process" << std::endl;
        return -1;
    }

    if (pid == 0) {
        // в дочернем процессе выполняем через execl
        execl("/bin/sh", "sh", "-c", command.c_str(), nullptr);
        _exit(EXIT_FAILURE);
    } else {
        // в родительском процессе ждем завершения дочернего
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            // если все норм то возвращаем код завершения
            return WEXITSTATUS(status);
        } else {
            return -1;
        }
    }
#endif
}
}
