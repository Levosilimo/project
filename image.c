#include <windows.h>

HBITMAP loadImage(HWND hwnd){
	HBITMAP hBitmap = (HBITMAP) LoadImageW(NULL, L"E:\\Mods\\project\\image.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hBitmap == NULL) MessageBoxW(hwnd, L"Failed to load image", L"Error", MB_OK);
    return hBitmap;
}