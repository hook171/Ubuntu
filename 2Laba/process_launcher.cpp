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

    if (!CreateProcess(nullptr, const_cast<char *>(command.c_str()), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        std::cerr << "Failed to create process: " << GetLastError() << std::endl;
        return -1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return static_cast<int>(exitCode);

#else
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork process" << std::endl;
        return -1;
    }

    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", command.c_str(), nullptr);
        _exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return -1;
        }
    }
#endif
}
}
