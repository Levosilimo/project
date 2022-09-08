#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "image.c"
#include "shapes.c"
#include "menu.c"

const char g_szClassName[] = "myWindowClass";

HMENU hMenu;
HBITMAP hBitmap;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CHOOSECOLOR cc;
    static COLORREF rgbCurrent = 0;  // current text color
    static HWND hwndPanel;
    static COLORREF acrCustClr[16];
    HDC hdc;
    PAINTSTRUCT ps;
    BITMAP bitmap;
    HDC hdcMem;
    HGDIOBJ oldBitmap;
	POINT ptClientUL; // client area upper left corner  
    POINT ptClientLR; // client area lower right corner  
	static HDC hdcCompat; // handle to DC for bitmap  
    static POINT pt;      // x- and y-coordinates of cursor  
    static POINT pt2;      // x- and y-coordinates of cursor 
    static RECT rcTarget; // rect to receive filled shape
    static RECT rcClient; // client area rectangle  
    static BOOL fSizeChordEllipse; // TRUE if ellipse is sized  
    static BOOL fSizeChord; // TRUE if ellipse is sized  
    static BOOL fDrawChordEllipse;   // TRUE if ellipse is drawn 
    static BOOL fDrawChord;   // TRUE if ellipse is drawn  
    static BOOL BrushTool = FALSE;   // TRUE if brush is activated
    static BOOL EraserTool = FALSE;   // TRUE if brush is activated
    static BOOL fSizeEllipse; // TRUE if ellipse is sized  
    static BOOL fDrawEllipse;   // TRUE if ellipse is drawn  
    static int nEllipseWidth;   // width for round corners  
    static int nEllipseHeight;  // height for round corners  
    static BOOL fDraw;   // TRUE if draw by mouse is active  
	switch(msg)
	{
		case WM_CREATE:
		{   
			hMenu = setMenu(hwnd);
			setIcon(hwnd);
			hBitmap = loadImage(hwnd);
			HWND hImageView = CreateWindowEx(NULL, L"STATIC", NULL, SS_BITMAP | WS_VISIBLE | WS_CHILD, 0, 00, 500, 600, hwnd, (HMENU)hwnd, GetModuleHandle(NULL), NULL);
			SendMessage(hImageView, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
		}
		break;
		case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
            SelectObject(ps.hdc, GetStockObject(HOLLOW_BRUSH)); 
			if (fDrawEllipse) 
            {
                Ellipse(ps.hdc, rcTarget.left, rcTarget.top, 
                    rcTarget.right, rcTarget.bottom); 
                fDrawEllipse = FALSE; 
                rcTarget.left = rcTarget.right = 0; 
                rcTarget.top = rcTarget.bottom = 0; 
            } 
            if (fDrawChord) 
            {
                Chord(ps.hdc, rcTarget.left, rcTarget.top, 
                    rcTarget.right, rcTarget.bottom,pt.x, pt.y, 
                    pt2.x, pt2.y); 
                fDrawChord = FALSE; 
                rcTarget.left = rcTarget.right = 0; 
                rcTarget.top = rcTarget.bottom = 0; 
            } 
            hdcMem = CreateCompatibleDC(hdc);
            oldBitmap = SelectObject(hdcMem, hBitmap);
            GetObject(hBitmap, sizeof(bitmap), &bitmap);
            BitBlt(hdc, 5, 5, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
            SelectObject(hdcMem, oldBitmap);
            DeleteDC(hdcMem);
            EndPaint(hwnd, &ps);
		}
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case ID_FILE_EXIT:
					PostMessage(hwnd, WM_CLOSE, 0, 0);
				break;
				case ID_CHORD_GENERATE:
					HDC hdc = GetDC(hwnd);
					RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
					generateChord(hdc,hwnd);
					ReleaseDC(hwnd, hdc);
					PostMessage(hwnd, WM_PAINT, 0, 0);
					MessageBox(hwnd, "Chord randomized", "Done", MB_OK);
				break;
                case ID_CHORD_CREATE:
					fSizeChordEllipse = TRUE;
				break;
				case ID_ELLIPSE_GENERATE:
					fSizeEllipse = TRUE; 
					hdc = GetDC(hwnd);
					RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
					generateEllipse(hdc,hwnd);
					ReleaseDC(hwnd, hdc);
					PostMessage(hwnd, WM_PAINT, 0, 0);
					MessageBox(hwnd, "Ellipse randomized", "Done", MB_OK);
				break;
				case ID_ELLIPSE_CREATE:
					fSizeEllipse = TRUE;
				break;
				case ID_ARC_GENERATE:
					hdc = GetDC(hwnd);
					RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
					generateArc(hdc,hwnd);
					ReleaseDC(hwnd, hdc);
					PostMessage(hwnd, WM_PAINT, 0, 0);
					MessageBox(hwnd, "Arc randomized", "Done", MB_OK);
				break;
                case ID_TOOL_NONE:                                    
                    CheckMenuRadioItem(hMenu,ID_TOOL_NONE, ID_TOOL_ERASER, ID_TOOL_NONE, MF_BYCOMMAND);
                    EraserTool = FALSE;
                    BrushTool = FALSE;
                break;
                case ID_TOOL_BRUSH:                                    
                    CheckMenuRadioItem(hMenu,ID_TOOL_NONE, ID_TOOL_ERASER, ID_TOOL_BRUSH, MF_BYCOMMAND);
                    EraserTool = FALSE;
                    BrushTool = TRUE;
                break;
                case ID_TOOL_ERASER:                                    
                    CheckMenuRadioItem(hMenu,ID_TOOL_NONE, ID_TOOL_ERASER, ID_TOOL_ERASER, MF_BYCOMMAND);
                    BrushTool = FALSE;
                    EraserTool = TRUE;
                break;
                case ID_COLOR_PALETTE:
                    // Initialize CHOOSECOLOR 
                    ZeroMemory(&cc, sizeof(cc));
                    cc.lStructSize = sizeof(cc);
                    cc.hwndOwner = hwnd;
                    cc.lpCustColors = (LPDWORD) acrCustClr;
                    cc.rgbResult = rgbCurrent;
                    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
                    
                    if (ChooseColor(&cc)==TRUE) 
                    {
                        rgbCurrent = cc.rgbResult; 
                    }
                break;
            }
        break;
		case WM_SIZE: 
 
            // Convert the client coordinates of the client area  
            // rectangle to screen coordinates and save them in a  
            // rectangle. The rectangle is passed to the ClipCursor  
            // function during WM_LBUTTONDOWN processing.  
 
            GetClientRect(hwnd, &rcClient); 
            ptClientUL.x = rcClient.left; 
            ptClientUL.y = rcClient.top; 
            ptClientLR.x = rcClient.right; 
            ptClientLR.y = rcClient.bottom; 
            ClientToScreen(hwnd, &ptClientUL); 
            ClientToScreen(hwnd, &ptClientLR); 
            SetRect(&rcClient, ptClientUL.x, ptClientUL.y, 
                ptClientLR.x, ptClientLR.y); 
            return 0; 
 
        case WM_LBUTTONDOWN: 
 
            // Restrict the cursor to the client area.  
            // This ensures that the window receives a matching  
            // WM_LBUTTONUP message.  
 
            ClipCursor(&rcClient); 
 
            // Save the coordinates of the cursor.  
 
            pt.x = (LONG) LOWORD(lParam); 
            pt.y = (LONG) HIWORD(lParam); 
 
            // If the user chooses one of the filled shapes,  
            // set the appropriate flag to indicate that the  
            // shape is being sized.  
 
            if (fDrawEllipse) fSizeEllipse = TRUE; 
            if (fDrawChord) fSizeChordEllipse = TRUE;
            if (BrushTool || EraserTool) {
                fDraw = TRUE;
                return 0L; 
            }
            else fDraw = FALSE;
            return 0; 
 
        case WM_MOUSEMOVE: 
            if (fDraw) 
            { 
                hdc = GetDC(hwnd);
                MoveToEx(hdc, pt.x, pt.y, NULL);
                if(EraserTool) SetROP2(hdc, R2_WHITE);  
                HPEN hPen = CreatePen(PS_DASHDOTDOT, 5, rgbCurrent);
                SelectObject(hdc,hPen);
	            LineTo(hdc, pt.x = LOWORD(lParam), pt.y = HIWORD(lParam)); 
                pt.x = LOWORD(lParam);
                pt.y = HIWORD(lParam);
                ReleaseDC(hwnd, hdc); 
            } 
            // If one of the "size" flags is set, draw  
            // the target rectangle as the user drags  
            // the mouse.  

            if ((wParam && MK_LBUTTON)){
                hdc = GetDC(hwnd); 
                // Set the mixing mode so that the pen color is the  
                // inverse of the background color. The previous  
                // rectangle can then be erased by drawing  
                // another rectangle on top of it.  
                SetROP2(hdc, R2_NOTXORPEN); 
                if(fSizeEllipse || fSizeChordEllipse){  
                    

                    // If a previous target rectangle exists, erase  
                    // it by drawing another rectangle on top.  

                    if (!IsRectEmpty(&rcTarget)) 
                    {
                        Ellipse(hdc, rcTarget.left, rcTarget.top, rcTarget.right, rcTarget.bottom); 
                    }

                    // Save the coordinates of the target rectangle.  
                    // Avoid invalid rectangles by ensuring that the  
                    // value of the left coordinate is greater than  
                    // that of the right, and that the value of the  
                    // bottom coordinate is greater than that of  
                    // the top.  
                    if ((pt.x < (LONG) LOWORD(lParam)) && 
                            (pt.y > (LONG) HIWORD(lParam))) 
                    {
                        SetRect(&rcTarget, pt.x, HIWORD(lParam), 
                            LOWORD(lParam), pt.y); 
                    } 

                    else if ((pt.x > (LONG) LOWORD(lParam)) && 
                            (pt.y > (LONG) HIWORD(lParam))) 
                    {
                        SetRect(&rcTarget, LOWORD(lParam), 
                            HIWORD(lParam), pt.x, pt.y); 
                    }

                    else if ((pt.x > (LONG) LOWORD(lParam)) && 
                            (pt.y < (LONG) HIWORD(lParam))) 
                    {
                        SetRect(&rcTarget, LOWORD(lParam), pt.y, 
                            pt.x, HIWORD(lParam)); 
                    }
                    else 
                    {
                        SetRect(&rcTarget, pt.x, pt.y, LOWORD(lParam), 
                            HIWORD(lParam)); 
                    }

                    // Draw the new target rectangle.  

                    Ellipse(hdc, rcTarget.left, rcTarget.top, rcTarget.right, rcTarget.bottom); 
                }
                if(fSizeChord){
                    pt2.x=LOWORD(lParam);
                    pt2.y=HIWORD(lParam);
                    MoveToEx(hdc, pt.x, pt.y, (LPPOINT) NULL); 
                    LineTo(hdc,pt2.x,pt2.y);
                }
                ReleaseDC(hwnd, hdc); 
            } 
            return 0; 
 
        case WM_LBUTTONUP: 
 
            // If one of the "size" flags is TRUE, reset it to FALSE,  
            // and then set the corresponding "draw" flag. Invalidate  
            // the appropriate rectangle and issue a WM_PAINT message.  
 
            if (fDraw) { 
                hdc = GetDC(hwnd); 
                MoveToEx(hdc, pt.x, pt.y, NULL); 
                if(EraserTool) SelectObject(ps.hdc, GetStockObject(WHITE_BRUSH)); 
                LineTo(hdc, LOWORD(lParam), HIWORD(lParam)); 
                ReleaseDC(hwnd, hdc); 
            } 
            fDraw = FALSE; 

            if (fSizeEllipse) 
            { 
                fSizeEllipse = FALSE; 
                fDrawEllipse = TRUE; 
            } 
 
            if (fDrawEllipse) 
            { 
                InvalidateRect(hwnd, &rcTarget, TRUE); 
                UpdateWindow(hwnd); 
            }
            if (fSizeChord) 
            { 
                fSizeChord = FALSE; 
                fDrawChord = TRUE; 
            }
            if (fDrawChord){
                InvalidateRect(hwnd, &rcTarget, TRUE); 
                UpdateWindow(hwnd);
            } 
            if (fSizeChordEllipse) 
            { 
                fSizeChordEllipse = FALSE; 
                InvalidateRect(hwnd, &rcTarget, TRUE); 
                fSizeChord = TRUE;
            }

            // Release the cursor.  
 
            ClipCursor((LPRECT) NULL); 
            return 0; 
 
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			DeleteDC(hdcCompat); 
			DeleteObject(hBitmap);
            PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Image editor",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
		NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

void setIcon(HWND hwnd){
	HICON hIcon, hIconSm;
	hIcon = (HICON)LoadImage(NULL, "brush.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	if(hIcon)
	{
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	}
	else
	{
		MessageBox(hwnd, "Could not load large icon! Is it in the current working directory?", "Error", MB_OK | MB_ICONERROR);
	}

	hIconSm = (HICON)LoadImage(NULL, "brush.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	if(hIconSm)
	{
		SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
	}
	else
	{
		MessageBox(hwnd, "Could not load small icon! Is it in the current working directory?", "Error", MB_OK | MB_ICONERROR);
	}
}
