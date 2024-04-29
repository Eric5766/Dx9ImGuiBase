#include <Windows.h>
#include <WinUser.h>
#include <iostream>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include <Hook.h>
#include <MinHookHooks.h>
#include <Console.h>
#include <MemoryHelpers.h>
#include <BotMenus.h>
#include <map>;

using namespace std;

bool ShowMenu = false;
bool InitImGui = false;

void InputHandler() 
{
    for (int i = 0; i < 5; i++) 
    for (int i = 0; i < 5; i++) 
    {
        ImGui::GetIO().MouseDown[i] = false;
    }

    int Button = -1;
    if (GetAsyncKeyState(VK_LBUTTON)) 
    {
        Button = 0;
    }

    if (Button != -1) 
    {
        ImGui::GetIO().MouseDown[Button] = true;
    }
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ShowMenu)
    {
        ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
        return true;
    }
    return CallWindowProc(Process.TargetWndProc, hWnd, msg, wParam, lParam);
}

HRESULT APIENTRY hkEndScene(IDirect3DDevice9* pDevice) 
{
    if (pDevice == NULL)
        return oEndScene(pDevice);

    if (!InitImGui) 
    {     
        IMGUI_CHECKVERSION();

        D3DDEVICE_CREATION_PARAMETERS pParameters;
        pDevice->GetCreationParameters(&pParameters);
        Process.TargetHwnd = pParameters.hFocusWindow;

        if (Process.TargetHwnd != NULL) 
        {
            Process.TargetWndProc = (WNDPROC)SetWindowLongPtr(Process.TargetHwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
            ImGui_ImplWin32_Init(Process.TargetHwnd);
            ImGui_ImplDX9_Init(pDevice);
            ImGui::GetIO().ImeWindowHandle = Process.TargetHwnd;
            InitImGui = true;
        }   
    }
    
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		ShowMenu = !ShowMenu;
        /*BlockMouseToGame = !BlockMouseToGame;*/
	}
    ImGui::GetIO().MouseDrawCursor = false;



	if (ShowMenu == true)
	{
		InputHandler();
		BotMenu();
	}

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	return oEndScene(pDevice);
}

HRESULT APIENTRY hkPresent(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) 
{
    if (pDevice == NULL)
        return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT APIENTRY hkDrawIndexedPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) 
{
    if (pDevice == NULL)
        return oDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

    return oDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT APIENTRY hkDrawPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) 
{
    if (pDevice == NULL)
        return oDrawPrimitive(pDevice, PrimitiveType, StartVertex, PrimitiveCount);

	return oDrawPrimitive(pDevice, PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT APIENTRY hkSetTexture(IDirect3DDevice9* pDevice, DWORD Stage, IDirect3DBaseTexture9* pTexture) 
{
    if (pDevice == NULL)
        return oSetTexture(pDevice, Stage, pTexture);

	return oSetTexture(pDevice, Stage, pTexture);
}

HRESULT APIENTRY hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) 
{
    if (pDevice == NULL)
        return oReset(pDevice, pPresentationParameters);

    D3DDEVICE_CREATION_PARAMETERS pParameters{ 0 };
    ImGui_ImplDX9_InvalidateDeviceObjects();
    auto ResetReturn = oReset(pDevice, pPresentationParameters);
    ImGui_ImplDX9_CreateDeviceObjects();
    return ResetReturn;
}

HRESULT APIENTRY hkSetStreamSource(IDirect3DDevice9* pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) 
{
    if (pDevice == NULL)
        return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);

	return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT APIENTRY hkSetVertexDeclaration(IDirect3DDevice9* pDevice, IDirect3DVertexDeclaration9* pDecl) 
{
    if (pDevice == NULL)
        return oSetVertexDeclaration(pDevice, pDecl);

	return oSetVertexDeclaration(pDevice, pDecl);
}

HRESULT APIENTRY hkSetVertexShaderConstantF(IDirect3DDevice9* pDevice, UINT StartRegister, const float* pConstantData, UINT Vector4fCount) 
{
    if (pDevice == NULL)
        return oSetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);

	return oSetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkSetVertexShader(IDirect3DDevice9* pDevice, IDirect3DVertexShader9* pShader) 
{
    if (pDevice == NULL)
        return oSetVertexShader(pDevice, pShader);

	return oSetVertexShader(pDevice, pShader);
}

HRESULT APIENTRY hkSetPixelShader(IDirect3DDevice9* pDevice, IDirect3DPixelShader9* pShader) 
{
    if (pDevice == NULL)
        return oSetPixelShader(pDevice, pShader);

	return oSetPixelShader(pDevice, pShader);
}

LRESULT CALLBACK LLMOUSEHOOK(int nCode, WPARAM wParam, LPARAM lParam)
{
        MSLLHOOKSTRUCT* MouseInfo = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
        switch (wParam)
        {
        case WM_MOUSEWHEEL:
            ImGui::GetIO().MouseWheel += (float)GET_WHEEL_DELTA_WPARAM(MouseInfo->mouseData) / (float)WHEEL_DELTA *0.25f;
        }
        return CallNextHookEx(NULL, nCode, wParam, lParam);
}

DWORD WINAPI InitHook(LPVOID lpParameter) 
{
    HWND WindowHwnd = CreateWindow(L"BUTTON", L"DX", WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, Process.hModule, NULL);
    if (WindowHwnd == NULL) 
    {
        return FALSE;
    }

    IDirect3D9* IDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
    if (IDirect3D9 == NULL) 
    {
        DestroyWindow(WindowHwnd);
        return FALSE;
    }

    D3DPRESENT_PARAMETERS pParameters;
    ZeroMemory(&pParameters, sizeof(pParameters));
    pParameters.Windowed = TRUE;
    pParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pParameters.hDeviceWindow = WindowHwnd;
    pParameters.BackBufferFormat = D3DFMT_UNKNOWN;

    IDirect3DDevice9* pDevice;
    if (IDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, WindowHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pParameters, &pDevice) != D3D_OK) 
    {
        IDirect3D9->Release();
        DestroyWindow(WindowHwnd);
        return FALSE;
    }

    if (pDevice == NULL) 
    {
        IDirect3D9->Release();
        DestroyWindow(WindowHwnd);
        return FALSE;
    }

#if defined _M_X64
    DWORD64* DVTable = (DWORD64*)pDevice;
    DVTable = (DWORD64*)DVTable[0];
#elif defined _M_IX86
    DWORD* DVTable = (DWORD*)pDevice;
    DVTable = (DWORD*)DVTable[0];
#endif

    oEndScene = (EndScene)DVTable[42];
    oPresent = (Present)DVTable[17];
    oDrawIndexedPrimitive = (DrawIndexedPrimitive)DVTable[82];
    oDrawPrimitive = (DrawPrimitive)DVTable[81];
    oSetTexture = (SetTexture)DVTable[65];
    oReset = (Reset)DVTable[16];
    oSetStreamSource = (SetStreamSource)DVTable[100];
    oSetVertexDeclaration = (SetVertexDeclaration)DVTable[87];
    oSetVertexShaderConstantF = (SetVertexShaderConstantF)DVTable[94];
    oSetVertexShader = (SetVertexShader)DVTable[92];
    oSetPixelShader = (SetPixelShader)DVTable[107];

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(LPVOID&)oEndScene, (PBYTE)hkEndScene);
    DetourAttach(&(LPVOID&)oPresent, (PBYTE)hkPresent);
    DetourAttach(&(LPVOID&)oDrawIndexedPrimitive, (PBYTE)hkDrawIndexedPrimitive);
    DetourAttach(&(LPVOID&)oDrawPrimitive, (PBYTE)hkDrawPrimitive);
    DetourAttach(&(LPVOID&)oSetTexture, (PBYTE)hkSetTexture);
    DetourAttach(&(LPVOID&)oReset, (PBYTE)hkReset);
    DetourAttach(&(LPVOID&)oSetStreamSource, (PBYTE)hkSetStreamSource);
    DetourAttach(&(LPVOID&)oSetVertexDeclaration, (PBYTE)hkSetVertexDeclaration);
    DetourAttach(&(LPVOID&)oSetVertexShaderConstantF, (PBYTE)hkSetVertexShaderConstantF);
    DetourAttach(&(LPVOID&)oSetVertexShader, (PBYTE)hkSetVertexShader);
    DetourAttach(&(LPVOID&)oSetPixelShader, (PBYTE)hkSetPixelShader);
    DetourTransactionCommit();
    
    pDevice->Release();
    IDirect3D9->Release();
    DestroyWindow(WindowHwnd);
    return TRUE;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) 
{
    switch (fdwReason) 
    {
    case DLL_PROCESS_ATTACH:
        Process.hModule = hModule; 
        InitConsole();
        InitMinHook();
        DisableThreadLibraryCalls(hModule);
        ImGui::CreateContext();
        CreateThread(0, 0, InitHook, 0, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        FreeLibraryAndExitThread(hModule, 0);
        break;
    }
    return TRUE;
}