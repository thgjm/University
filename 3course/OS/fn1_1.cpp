//fn1(x)=x^2

#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

#define PIPE_NAME "\\\\.\\pipe\\fn1_pipe"
#define BUFFER_SIZE 1024

std::string fn1(const std::string& x) {
    try {
        double num = std::stod(x);
        double result = num * num;
        std::ostringstream oss;
        oss << result;
        return oss.str();
    }
    catch (...) {
        return "ERROR: couldn't calculate";
    }
}

int main() {
    SetConsoleOutputCP(65001);

    //connecting to a named pipe
    HANDLE hPipe = INVALID_HANDLE_VALUE;

    //waiting for channel availability
    while (true) {
        hPipe = CreateFileA(
            PIPE_NAME,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hPipe != INVALID_HANDLE_VALUE) break;

        if (GetLastError() != ERROR_PIPE_BUSY) {
            std::cerr << "[fn1] Error connecting to the pipe: " << GetLastError() << std::endl;
            return 1;
        }

        if (!WaitNamedPipeA(PIPE_NAME, 5000)) {
            std::cerr << "[fn1] Pipe wait timeout\n";
            return 1;
        }
    }

    DWORD dwMode = PIPE_READMODE_MESSAGE;
    SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL);

    //reading x
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;

    if (!ReadFile(hPipe, buffer, BUFFER_SIZE - 1, &bytesRead, NULL)) {
        std::cerr << "[fn1] Error reading from the pipe: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return 1;
    }

    buffer[bytesRead] = '\0';
    std::string x(buffer);

    std::string result = fn1(x);

    //sending the result
    DWORD bytesWritten;
    if (!WriteFile(hPipe, result.c_str(), static_cast<DWORD>(result.length()), &bytesWritten, NULL)) {
        std::cerr << "[fn1] Error writing to the pipe: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return 1;
    }

    FlushFileBuffers(hPipe);

    CloseHandle(hPipe);
    return 0;
}