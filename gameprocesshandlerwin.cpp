
#include "gameprocesshandlerwin.h"

#ifdef Q_OS_WINDOWS

#include <iostream>

#include <TlHelp32.h>
//#include <processthreadsapi.h>

// Credit goes to Guided Hacking for the majority of the code in this class
// This class handles attaching to the game and reading/writing its memory.
// This is the Windows implementation.
GameProcessHandler::GameProcessHandler()
{
    m_processID = GetProcessID(L"okami.exe");
    m_addressOf_main = GetModuleBaseAddress(m_processID, L"main.dll");
    m_addressOf_flower_kernel = GetModuleBaseAddress(m_processID, L"flower_kernel.dll");

    m_processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, m_processID);
}

GameProcessHandler::~GameProcessHandler()
{
    CloseHandle(m_processHandle);
}

DWORD GameProcessHandler::GetProcessID(const wchar_t *processName)
{
    DWORD processID = 0;
    HANDLE handleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (handleSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(processEntry);

        if(Process32First(handleSnapshot, &processEntry))
        {
            do
            {
                if(!_wcsicmp(processEntry.szExeFile, processName))
                {
                    processID = processEntry.th32ProcessID;
                    break;
                }

            }while(Process32Next(handleSnapshot, &processEntry));
        }
    }
    CloseHandle(handleSnapshot);
    std::cout << processID << std::endl;
    return(processID);
}

uintptr_t GameProcessHandler::GetModuleBaseAddress(DWORD processID, const wchar_t *moduleName)
{
    uintptr_t moduleBaseAddress = 0;
    HANDLE handleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
    if (handleSnapshot != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 moduleEntry;
        moduleEntry.dwSize = sizeof(moduleEntry);
        if(Module32First(handleSnapshot, &moduleEntry))
        {
            do
            {
                if(!_wcsicmp(moduleEntry.szModule, moduleName))
                {
                    moduleBaseAddress = (uintptr_t) moduleEntry.modBaseAddr;
                }

            }while(Module32Next(handleSnapshot, &moduleEntry));
        }
    }
    CloseHandle(handleSnapshot);
    std::cout << moduleBaseAddress << std::endl;
    return(moduleBaseAddress);
}

uintptr_t GameProcessHandler::GetDynamicMemoryAddress(uintptr_t modulePtr, uintptr_t baseAddressOffset, QVector<unsigned int> offsets)
{
    uintptr_t address = modulePtr + baseAddressOffset;
    for(int i = 0 ; i < offsets.size() ; i++)
    {
        ReadProcessMemory(m_processHandle, (BYTE*)address, &address, sizeof(address), nullptr);
        address += offsets[i];
    }
    std::cout << address << std::endl;
    return address;
}

void GameProcessHandler::FlipBit(uintptr_t baseAddressOffset, QVector<unsigned int> offsets, uintptr_t additionalOffset, int bitIndex)
{
    char temp;
    uintptr_t address = GetDynamicMemoryAddress(m_addressOf_main, baseAddressOffset, offsets) + additionalOffset;
    ReadProcessMemory(m_processHandle, (BYTE*) address, &temp, sizeof(temp), nullptr);
    temp ^= (0b00000001 << bitIndex);
    WriteProcessMemory(m_processHandle, (BYTE*) address, &temp, sizeof(temp), nullptr);
}

template<typename T>
T GameProcessHandler::GetTypeValue(uintptr_t baseAddressOffset, QVector<unsigned int> offsets, uintptr_t additionalOffset)
{
    T output;
    uintptr_t address = GetDynamicMemoryAddress(m_addressOf_main, baseAddressOffset, offsets) + additionalOffset;
    ReadProcessMemory(m_processHandle, (BYTE*) address, &output, sizeof(output), nullptr);
    return output;
}

template Vec3 GameProcessHandler::GetTypeValue(uintptr_t baseAddressOffset, QVector<unsigned int> offsets, uintptr_t additionalOffset);
template qint8 GameProcessHandler::GetTypeValue(uintptr_t baseAddressOffset, QVector<unsigned int> offsets, uintptr_t additionalOffset);

template<typename T>
void GameProcessHandler::SetTypeValue(uintptr_t baseAddressOffset, QVector<unsigned int> offsets, uintptr_t additionalOffset, T input)
{
    uintptr_t address = GetDynamicMemoryAddress(m_addressOf_main, baseAddressOffset, offsets) + additionalOffset;
    WriteProcessMemory(m_processHandle, (BYTE*) address, &input, sizeof(input), nullptr);
}
template void GameProcessHandler::SetTypeValue(uintptr_t baseAddressOffset, QVector<unsigned int> offsets, uintptr_t additionalOffset, Vec3 input);
template void GameProcessHandler::SetTypeValue(uintptr_t baseAddressOffset, QVector<unsigned int> offsets, uintptr_t additionalOffset, float input);
template void GameProcessHandler::SetTypeValue(uintptr_t baseAddressOffset, QVector<unsigned int> offsets, uintptr_t additionalOffset, qint8 input);

#endif


