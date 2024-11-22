#include "process_launcher.h"
#include <iostream>

int main() {
    std::string command;

#ifdef _WIN32
    command = "notepad.exe"; // Для Windows
#else
    command = "ls"; // Для Linux
#endif

    int exitCode = ProcessLauncher::launchProcess(command);
    std::cout << "Process exited with code: " << exitCode << std::endl;

    return 0;
}
