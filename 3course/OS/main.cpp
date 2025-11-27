/*
Communication: Named Pipes

Processing approach: Multiplexing

Operations: Concatenation
            If possible, the sum of numbers
*/



#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>

#define PIPE_NAME_FN1 "\\\\.\\pipe\\fn1_pipe"
#define PIPE_NAME_FN2 "\\\\.\\pipe\\fn2_pipe"
#define BUFFER_SIZE 1024

std::atomic<bool> shouldTerminate(false);

//function for stopping after button click
void keyboardHandler() {
    while (!shouldTerminate) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            shouldTerminate = true;
            std::cout << "\n[Stopping calculations...]\n";
            break;
        }
        Sleep(50);
    }
}

bool isNumber(const std::string& s) {
    try {
        size_t idx;
        std::stod(s, &idx);
        return idx == s.size();
    }
    catch (...) {
        return false;
    }
}

//function for creating the process
bool CreateChildProcess(const char* processName) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char cmdLine[256];
    strcpy_s(cmdLine, processName);

    if (!CreateProcessA(
        NULL,
        cmdLine,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        std::cerr << "Process creation error " << processName << ": " << GetLastError() << std::endl;
        return false;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return true;
}

int main() {
    SetConsoleOutputCP(65001);

    std::cout << "Enter ESC to stop the calculations.\n\n";

    while (true) {
        std::cout << "Enter argument x (or 'q' to quit): ";
        std::string x;
        std::getline(std::cin, x);

        if (x == "q" || x == "Q") break;

        if (x.empty()) continue;

        shouldTerminate = false;

        //creating named pipes
        HANDLE hPipeFn1 = CreateNamedPipeA(
            PIPE_NAME_FN1,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            1,
            BUFFER_SIZE,
            BUFFER_SIZE,
            0,
            NULL
        );

        HANDLE hPipeFn2 = CreateNamedPipeA(
            PIPE_NAME_FN2,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            1,
            BUFFER_SIZE,
            BUFFER_SIZE,
            0,
            NULL
        );

        if (hPipeFn1 == INVALID_HANDLE_VALUE || hPipeFn2 == INVALID_HANDLE_VALUE) {
            std::cerr << "Error creating pipes: " << GetLastError() << std::endl;
            continue;
        }


        /*
        //starting processes fn1 and fn2
        if (!CreateChildProcess("fn1.exe")) {
            CloseHandle(hPipeFn1);
            CloseHandle(hPipeFn2);
            continue;
        }

        if (!CreateChildProcess("fn2.exe")) {
            CloseHandle(hPipeFn1);
            CloseHandle(hPipeFn2);
            continue;
        }
        */

        //starting processes fn1_1 and fn2_1
        if (!CreateChildProcess("fn1_1.exe")) {
            CloseHandle(hPipeFn1);
            CloseHandle(hPipeFn2);
            continue;
        }

        if (!CreateChildProcess("fn2_1.exe")) {
            CloseHandle(hPipeFn1);
            CloseHandle(hPipeFn2);
            continue;
        }

        Sleep(100);

        std::cout << "Waiting for processes to connect...\n";

        bool connected1 = ConnectNamedPipe(hPipeFn1, NULL) ? true : (GetLastError() == ERROR_PIPE_CONNECTED);
        bool connected2 = ConnectNamedPipe(hPipeFn2, NULL) ? true : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (!connected1 || !connected2) {
            std::cerr << "Error connecting to pipes\n";
            CloseHandle(hPipeFn1);
            CloseHandle(hPipeFn2);
            continue;
        }

        std::cout << "Processes are connected. Starting calculations...\n";

        //sending x to both processes
        DWORD bytesWritten;
        if (!WriteFile(hPipeFn1, x.c_str(), static_cast<DWORD>(x.length()), &bytesWritten, NULL)) {
            std::cerr << "Error writing to pipe fn1: " << GetLastError() << std::endl;
            CloseHandle(hPipeFn1);
            CloseHandle(hPipeFn2);
            continue;
        }
        FlushFileBuffers(hPipeFn1);

        if (!WriteFile(hPipeFn2, x.c_str(), static_cast<DWORD>(x.length()), &bytesWritten, NULL)) {
            std::cerr << "Error writing to pipe fn2: " << GetLastError() << std::endl;
            CloseHandle(hPipeFn1);
            CloseHandle(hPipeFn2);
            continue;
        }
        FlushFileBuffers(hPipeFn2);

        std::cout << "X sent successfully to both processes. Waiting for results...\n";

        std::thread keyboardThread(keyboardHandler);


        //multiplexing: reading results from both channels
        std::string result1, result2;
        bool completed1 = false, completed2 = false;
        bool success = true;
        std::string errorReason;

        char buffer1[BUFFER_SIZE];
        char buffer2[BUFFER_SIZE];

        while ((!completed1 || !completed2) && !shouldTerminate) {
            //termination check
            if (shouldTerminate) {
                errorReason = "Calculation terminated by user (ESC pressed)";
                success = false;
                break;
            }

            //checking and reading from pipe fn1
            if (!completed1) {
                DWORD bytesAvailable;
                if (PeekNamedPipe(hPipeFn1, NULL, 0, NULL, &bytesAvailable, NULL)) {
                    if (bytesAvailable > 0) {
                        DWORD bytesRead;
                        if (ReadFile(hPipeFn1, buffer1, BUFFER_SIZE - 1, &bytesRead, NULL)) {
                            buffer1[bytesRead] = '\0';
                            result1 = std::string(buffer1);
                            completed1 = true;
                            std::cout << "[fn1] Result obtained: " << result1 << std::endl;
                        }
                    }
                }
                else {
                    DWORD error = GetLastError();
                    if (error == ERROR_BROKEN_PIPE || error == ERROR_PIPE_NOT_CONNECTED) {
                        errorReason = "The pipe fn1 is broken.";
                        success = false;
                        break;
                    }
                }
            }

            //checking and reading from pipe fn2
            if (!completed2) {
                DWORD bytesAvailable;
                if (PeekNamedPipe(hPipeFn2, NULL, 0, NULL, &bytesAvailable, NULL)) {
                    if (bytesAvailable > 0) {
                        DWORD bytesRead;
                        if (ReadFile(hPipeFn2, buffer2, BUFFER_SIZE - 1, &bytesRead, NULL)) {
                            buffer2[bytesRead] = '\0';
                            result2 = std::string(buffer2);
                            completed2 = true;
                            std::cout << "[fn2] Result obtained: " << result2 << std::endl;
                        }
                    }
                }
                else {
                    DWORD error = GetLastError();
                    if (error == ERROR_BROKEN_PIPE || error == ERROR_PIPE_NOT_CONNECTED) {
                        errorReason = "The pipe fn2 is broken.";
                        success = false;
                        break;
                    }
                }
            }

            if (!completed1 || !completed2) {
                Sleep(10);
            }
            else break;
        }

        shouldTerminate = true;

        Sleep(10);

        if (keyboardThread.joinable()) keyboardThread.detach();

        //result output
        std::cout << "\n=== Result ===\n";
        if (success && completed1 && completed2) {

            // ERROR detection
            if (result1.find("ERROR") != std::string::npos ||
                result2.find("ERROR") != std::string::npos)
                std::cout << "Status: Fail. Child process error.\n";
            else {
                //concatenation
                std::string finalResult = result1 + result2;
                std::cout << "Concatenation result: " << finalResult << std::endl;

                if (isNumber(result1) && isNumber(result2)) {
                    //if two results are numbers - also calculate their sum
                    double v1 = std::stod(result1);
                    double v2 = std::stod(result2);

                    double finalResult2 = v1 + v2;
                    std::cout << "Sum result: " << finalResult2 << std::endl;
                }

                std::cout << "Status: Successful.\n";
            }
        }
        else {
            std::cout << "Result: Undefined\n";
            if (!errorReason.empty())
                std::cout << "Reason: " << errorReason << std::endl;
            else
                std::cout << "Reason: Calculation not completed\n";
        }
        std::cout << "==================\n\n";

        //closing pipes
        CloseHandle(hPipeFn1);
        CloseHandle(hPipeFn2);

        Sleep(500);
    }

    return 0;
}