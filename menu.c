#include <windows.h>
#include "resource.h"
HMENU setMenu(HWND hwnd){
    HMENU hMenu, hSubMenu, hChordSubMenu, hEllipseSubMenu, hArcSubMenu;

	hMenu = CreateMenu();
    hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
    AppendMenu(hSubMenu, MF_STRING, ID_FILE_OPEN, "&Open");
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

    hSubMenu = CreatePopupMenu();
    hChordSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING | MF_POPUP, (UINT)hChordSubMenu, "&Chord");
    AppendMenu(hChordSubMenu, MF_STRING, ID_CHORD_GENERATE, "&Generate a chord");
    AppendMenu(hChordSubMenu, MF_STRING, ID_CHORD_CREATE, "&Create a Chord");
    hEllipseSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING | MF_POPUP, (UINT)hEllipseSubMenu, "&Ellipse");
    //AppendMenu(hSubMenu, MF_STRING, ID_COLOR_PALETTE, "&Color palette");
    AppendMenu(hEllipseSubMenu, MF_STRING, ID_ELLIPSE_GENERATE, "&Generate an Ellipse");
    AppendMenu(hEllipseSubMenu, MF_STRING, ID_ELLIPSE_CREATE, "&Create an Ellipse");
    hArcSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING | MF_POPUP, (UINT)hArcSubMenu, "&Arc");
    AppendMenu(hArcSubMenu, MF_STRING, ID_ARC_GENERATE, "&Generate an Arc");
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Shapes");

    hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING | MF_ENABLED, ID_TOOL_NONE, "&None");
    AppendMenu(hSubMenu, MF_STRING | MF_ENABLED, ID_TOOL_BRUSH, "&Brush");
    AppendMenu(hSubMenu, MF_STRING | MF_ENABLED, ID_TOOL_ERASER, "&Eraser");
    CheckMenuRadioItem(hMenu,ID_TOOL_NONE, ID_TOOL_ERASER, ID_TOOL_NONE, MF_BYCOMMAND);
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Tools");
    AppendMenu(hMenu, MF_STRING, ID_BRUSH_PALETTE, "&Palette");
    AppendMenu(hMenu, MF_STRING, ID_BRUSH_TRACKBAR, "&Brush Size");

    SetMenu(hwnd, hMenu);
    return hMenu;
}