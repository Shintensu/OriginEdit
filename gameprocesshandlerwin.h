#ifndef GAMEPROCESSHANDLERWIN_H
#define GAMEPROCESSHANDLERWIN_H

#include <QVector>
#include <Windows.h>

#ifdef Q_OS_WINDOWS

struct Vec3
{
    float x, y, z;
};

class GameProcessHandler
{
public:
    GameProcessHandler();
    ~GameProcessHandler();
    DWORD GetProcessID(const wchar_t* processName);
    uintptr_t GetModuleBaseAddress(DWORD processID, const wchar_t* moduleName);
    uintptr_t GetDynamicMemoryAddress(uintptr_t modulePtr, uintptr_t baseAddressOffset, QVector<unsigned int> offsets);
    template<typename T>
    T GetTypeValue(uintptr_t baseAddressOffset, QVector<unsigned int> offsets, uintptr_t additionalOffset);
    template<typename T>
    void SetTypeValue(uintptr_t baseAddressOffset, QVector<unsigned int> offsets, uintptr_t additionalOffset, T input);
    void FlipBit(uintptr_t baseAddressOffset, QVector<unsigned int> offsets, uintptr_t additionalOffset, int bitIndex);

public:
    DWORD m_processID;
    HANDLE m_processHandle;

    uintptr_t m_addressOf_main;
    uintptr_t m_addressOf_flower_kernel;
};
#endif

#endif // GAMEPROCESSHANDLERWIN_H
