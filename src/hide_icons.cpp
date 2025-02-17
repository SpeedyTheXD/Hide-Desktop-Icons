#define UNICODE
#include <windows.h>
#include <shellapi.h>
#include <fstream>
#include <string>
#include <unordered_map>

#define ID_TRAY_EXIT 1001
#define WM_TRAYICON (WM_USER + 1)
NOTIFYICONDATA nid;

using namespace std;
HHOOK hKeyboardHook;
#define IDI_APP_ICON 101
#define IDI_TRAY_ICON 102

int read_file(const string &filename)
{
    ifstream in_file(filename);
    int key_code;

    if (in_file >> key_code)
    {
        in_file.close();
        return key_code;
    }

    in_file.close();
    return -1;
}

void AddTrayIcon(HWND hWnd, HINSTANCE hInstance)
{
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;

    HICON hTrayIcon = (HICON)LoadImage(hInstance, L"resources/tray_icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
    nid.hIcon = hTrayIcon;

    wcscpy_s(nid.szTip, L"Hide Desktop Icons");

    Shell_NotifyIcon(NIM_DELETE, &nid);
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void RemoveTrayIcon()
{
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

void ShowTrayMenu(HWND hWnd)
{
    POINT pt;
    GetCursorPos(&pt);

    HMENU hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT, L"Exit");

    SetForegroundWindow(hWnd);
    TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_RIGHTALIGN, pt.x, pt.y, 0, hWnd, NULL);
    DestroyMenu(hMenu);
}

// Windows Procedure for tray icon interaction
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_TRAYICON)
    {
        if (lParam == WM_RBUTTONUP) // Right-click menu
        {
            ShowTrayMenu(hWnd);
        }
    }
    else if (uMsg == WM_COMMAND)
    {
        if (LOWORD(wParam) == ID_TRAY_EXIT) // User clicks "Exit"
        {
            RemoveTrayIcon();
            PostQuitMessage(0);
        }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND GetDesktopIconsHwnd()
{
    HWND hProgman = FindWindowW(L"Progman", NULL);
    HWND hShellView = NULL;
    HWND hWorkerW = NULL;

    // Find SHELLDLL_DefView inside Progman
    hShellView = FindWindowExW(hProgman, NULL, L"SHELLDLL_DefView", NULL);

    // If not found, search in WorkerW windows
    if (!hShellView)
    {
        hWorkerW = FindWindowW(L"WorkerW", NULL);
        while (hWorkerW)
        {
            hShellView = FindWindowExW(hWorkerW, NULL, L"SHELLDLL_DefView", NULL);
            if (hShellView)
                break;
            hWorkerW = FindWindowExW(NULL, hWorkerW, L"WorkerW", NULL);
        }
    }

    // Now find the SysListView32 window, which contains the icons
    return FindWindowExW(hShellView, NULL, L"SysListView32", NULL);
}

bool IsUserOnDesktop()
{
    HWND hForeground = GetForegroundWindow();
    HWND hDesktopIcons = GetDesktopIconsHwnd();

    return (hForeground == hDesktopIcons);
}

// HWND GetDesktopIconsHwnd()
// {
//     HWND hProgman = FindWindowW(L"Progman", NULL);
//     HWND hShellView = FindWindowExW(hProgman, NULL, L"SHELLDLL_DefView", NULL);
//     HWND hIcons = FindWindowExW(hShellView, NULL, L"SysListView32", NULL);
//     return hIcons;
// }

void ToggleDesktopIcons()
{
    static bool isHidden = false;
    HWND hDesktopIcons = GetDesktopIconsHwnd();
    if (hDesktopIcons)
    {
        if (isHidden)
            ShowWindow(hDesktopIcons, SW_SHOW);
        else
            ShowWindow(hDesktopIcons, SW_HIDE);

        isHidden = !isHidden;
        RedrawWindow(hDesktopIcons, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN)
    {
        KBDLLHOOKSTRUCT *pKey = (KBDLLHOOKSTRUCT *)lParam;
        int key_code = read_file("key.txt");

        if (pKey->vkCode == key_code)
        {
            ToggleDesktopIcons();
            return 1;
        }
    }

    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

void SetHook()
{
    hKeyboardHook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
}

void UnHook()
{
    UnhookWindowsHookEx(hKeyboardHook);
}

void tray_icon(HINSTANCE hInstance)
{
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"TrayIconWindow";
    RegisterClass(&wc);

    HWND hWnd = CreateWindow(L"TrayIconWindow", L"Tray Icon Window", 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
    AddTrayIcon(hWnd, hInstance);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // App icon
    HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));

    // Preventing multiple exes file to be run at the same time
    HANDLE hMutex = CreateMutex(NULL, TRUE, L"Global\\MyUniqueMutexName");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(hMutex);
        return 0;
    }

    // Create a tray icon
    tray_icon(hInstance);

    SetHook();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnHook();
    CloseHandle(hMutex);
    return 0;
}
