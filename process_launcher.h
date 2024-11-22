#ifndef PROCESS_LAUNCHER_H
#define PROCESS_LAUNCHER_H

#include <string>

namespace ProcessLauncher {
    int launchProcess(const std::string &command); // Запускает процесс, ждёт завершения и возвращает код завершения
}

#endif // PROCESS_LAUNCHER_H
