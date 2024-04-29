#include <windows.h>
#include <WinUser.h>
#include <iostream>
#pragma warning(disable:4996)
void InitConsole()
{
    if (!AttachConsole(ATTACH_PARENT_PROCESS))
    {
        AllocConsole();
    }
    freopen("CONOUT$", "w", stdout);
}