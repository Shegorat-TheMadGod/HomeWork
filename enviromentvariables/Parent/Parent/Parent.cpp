#include <Windows.h>
#include <iostream>
#include <string>

int main()
{
    std::string envVariable;
    std::cout << "Enter enviromet variable (e.g. MY_VARIABLE=1): ";
    std::getline(std::cin, envVariable);
    _putenv(envVariable.c_str());
    envVariable = " A " + envVariable;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    LPCSTR path = "C:/Users/Vulturem/source/repos/Child/Debug/child.exe";
    if (!CreateProcessA(path, const_cast<char*>(envVariable.c_str()), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
    {
        std::cout << "Child process not created" << std::endl;
    }
    std::cout << "exit_parent" << std::endl;
    return 0;
}