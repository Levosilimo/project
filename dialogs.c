#include <windows.h>
LRESULT CALLBACK SettingsProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        CreateWindow(TEXT("button"), TEXT("test"),
            WS_VISIBLE | WS_CHILD,
            020, 530, 70, 15,
            hWnd, (HMENU)1, NULL, NULL);

        CreateWindow(TEXT("button"), TEXT("ok"),
            WS_VISIBLE | WS_CHILD,
            720, 530, 50, 15,
            hWnd, (HMENU)2, NULL, NULL);
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case 1:
            MessageBox(hWnd,L"test",L"message",NULL);
            break;
        case 2:
            MessageBox(hWnd, L"ok", L"message", NULL);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        DestroyWindow(hWnd);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}