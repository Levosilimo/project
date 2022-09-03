#include <windows.h>
#include "resource.h"
void setMenu(HWND hwnd){
    HMENU hMenu, hSubMenu, hChordSubMenu, hEllipseSubMenu, hArcSubMenu;

	hMenu = CreateMenu();
    hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
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

    SetMenu(hwnd, hMenu);
}