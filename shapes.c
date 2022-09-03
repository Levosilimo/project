#include <math.h>
#include <time.h>
#include <windows.h>
int pos[8];
RECT rect;
void generateChord(HDC hdc, HWND hwnd){
	RECT rect;
	GetWindowRect(hwnd, &rect);
	HBRUSH hbrOld = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
    srand(time(NULL));  
	pos[2]=(int)(rand()%rect.right);
	pos[3]=(int)(rand()%rect.bottom);
	pos[0]=(int)(rand()%(pos[2]-(pos[2]/50)));
	pos[1]=(int)(rand()%(pos[3]-(pos[3]/50)));
	pos[4]=(int)(rand()%(pos[2]-(pos[2]/50)));
	pos[5]=(int)(rand()%(pos[3]-(pos[3]/50)));
	pos[6]=(int)(rand()%(pos[2]-(pos[2]/50)));
	pos[7]=(int)(rand()%(pos[3]-(pos[3]/50)));
	Chord(hdc,pos[0],pos[1],pos[2],pos[3],pos[4],pos[5],pos[6],pos[7]);
}
void generateEllipse(HDC hdc, HWND hwnd){
	GetWindowRect(hwnd, &rect);
	HBRUSH hbrOld = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
    srand(time(NULL));  
	pos[2]=(int)(rand()%rect.right);
	pos[3]=(int)(rand()%rect.bottom);
	pos[0]=(int)(rand()%(pos[2]-(pos[2]/50)));
	pos[1]=(int)(rand()%(pos[3]-(pos[3]/50)));
	Ellipse(hdc,pos[0],pos[1],pos[2],pos[3]);
}
void createEllipse(HDC hdc, HWND hwnd){
	GetWindowRect(hwnd, &rect);
	HBRUSH hbrOld = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
    srand(time(NULL));  
	pos[2]=(int)(rand()%rect.right);
	pos[3]=(int)(rand()%rect.bottom);
	pos[0]=(int)(rand()%(pos[2]-(pos[2]/50)));
	pos[1]=(int)(rand()%(pos[3]-(pos[3]/50)));
	Ellipse(hdc,pos[0],pos[1],pos[2],pos[3]);
}
void generateArc(HDC hdc, HWND hwnd){
	RECT rect;
	GetWindowRect(hwnd, &rect);
	HBRUSH hbrOld = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
    srand(time(NULL));  
	pos[2]=(int)(rand()%rect.right);
	pos[3]=(int)(rand()%rect.bottom);
	pos[0]=(int)(rand()%(pos[2]-(pos[2]/50)));
	pos[1]=(int)(rand()%(pos[3]-(pos[3]/50)));
	pos[4]=(int)(rand()%(pos[2]-(pos[2]/50)));
	pos[5]=(int)(rand()%(pos[3]-(pos[3]/50)));
	pos[6]=(int)(rand()%(pos[2]-(pos[2]/50)));
	pos[7]=(int)(rand()%(pos[3]-(pos[3]/50)));
	Arc(hdc,pos[0],pos[1],pos[2],pos[3],pos[4],pos[5],pos[6],pos[7]);
}