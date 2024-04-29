#include <mutex>
#include <windows.h>

HMODULE g_dinput = 0;
std::mutex g_load_mutex{};

void FailedToLoadOriginalDinput8()
{
    MessageBoxA(0, "Unable to load the original dinput8.dll.", "dinput8LoadFail", 0);
    ExitProcess(0);
}

bool load_dinput8() 
{
    //std::scoped_lock _{ g_load_mutex };

    if (g_dinput) 
    {
        return true;
    }

    wchar_t buffer[MAX_PATH]{ 0 };
    if (GetSystemDirectoryW(buffer, MAX_PATH) != 0) 
    {
        // Load the original dinput8.dll
        if ((g_dinput = LoadLibraryW((std::wstring{ buffer } + L"\\dinput8.dll").c_str())) == NULL) 
        {
            FailedToLoadOriginalDinput8();
            return false;
        }
        return true;
    }
    FailedToLoadOriginalDinput8();
    return false;
}
extern "C" 
{
    // DirectInput8Create wrapper for dinput8.dll
    __declspec(dllexport) HRESULT WINAPI direct_input8_create(HINSTANCE hinst, DWORD dw_version, const IID& riidltf, LPVOID* ppv_out, LPUNKNOWN punk_outer) 
    {
        // This needs to be done because when we include dinput.h in DInputHook,
        // It is a redefinition, so we assign an export by not using the original name
#pragma comment(linker, "/EXPORT:DirectInput8Create=_direct_input8_create@20")
        load_dinput8();
        return ((decltype(direct_input8_create)*)GetProcAddress(g_dinput, "DirectInput8Create"))(hinst, dw_version, riidltf, ppv_out, punk_outer);
    }
}