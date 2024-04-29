#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <dinput.h>
#include <Console.h>
#include "minhook/include/MinHook.h"

bool GetDinputDevice(void** pMouse, size_t size)
{
    if (!pMouse)
    {
        return false;
    }

    IDirectInput8* pDirectInput = NULL;

    // Create dummy device
    if (DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&pDirectInput, NULL) != DI_OK)
    {
        return false;
    }

    LPDIRECTINPUTDEVICE8  lpdiInput;
    
    // We're creating a sysMouse but it still seems to receive keyboard messages?
    if (pDirectInput->CreateDevice(GUID_SysMouse, &lpdiInput, NULL) != DI_OK)
    {
        pDirectInput->Release();
        return false;
    }

    lpdiInput->SetDataFormat(&c_dfDIKeyboard);
    lpdiInput->SetCooperativeLevel(GetActiveWindow(), DISCL_NONEXCLUSIVE);
    memcpy(pMouse, *reinterpret_cast<void***>(lpdiInput), size);
    lpdiInput->Release();
    pDirectInput->Release();
    return true;
}
typedef HRESULT(__stdcall* GetDeviceStateT)(IDirectInputDevice8* pThis, DWORD cbData, LPVOID lpvData);
GetDeviceStateT pGetDeviceState = nullptr; //pointer to original function

HRESULT CALLBACK hookGetDeviceState(IDirectInputDevice8* pThis, DWORD cbData, LPVOID lpvData)
{
    HRESULT result = pGetDeviceState(pThis, cbData, lpvData);
    if (result == DI_OK)
    {
        //if (BlockMouseToGame) // I know that in our case the struct is LPDIMOUSESTATE2x
        //{
        //    // Mouse Movement
        //   ((LPDIMOUSESTATE2)lpvData)->lX = 0;
        //   ((LPDIMOUSESTATE2)lpvData)->lY = 0;
        //   ((LPDIMOUSESTATE2)lpvData)->lZ = 0;
        //   
        //  ((LPDIMOUSESTATE2)lpvData)->rgbButtons[0] = 0; // Mouse LMB
        //  ((LPDIMOUSESTATE2)lpvData)->rgbButtons[1] = 0; // Mouse RMB
        //  ((LPDIMOUSESTATE2)lpvData)->rgbButtons[2] = 0; // Middle Mouse Button
        //}
    }
    return result;
}

void InitMinHook()
{
    MH_STATUS minhookstatus = MH_Initialize();
    if (minhookstatus == MH_OK)
    {
        printf("MinHook initialized!\n");
        static void* diMouse[32];
        if (GetDinputDevice(diMouse, sizeof(diMouse)))
        {
            MH_Initialize();
            MH_CreateHook(diMouse[9], hookGetDeviceState, reinterpret_cast<LPVOID*>(&pGetDeviceState));
            MH_EnableHook(diMouse[9]);
        }
       
    }
    if (minhookstatus != MH_OK)
    {
        MessageBoxA(0, "MinHook failed to Initialize", "MinHookFail", 0);
        ExitProcess(0);
    }
}




