// draw.cpp : Defines the entry point for the application.
//
#include <windows.h>
#include <gdiplus.h>
#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>
#include <queue>
using namespace Gdiplus;
#define MAX_LOADSTRING 100
#define TMR_1 1

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
INT value;
// buttons
HWND hwndButton;
struct kolejka {
	kolejka(int x, int y, int z = 1, bool going = 0);
	void add();
	int from = 1;
	int to = 1;
	int quantity = 0;
	bool is_going = 0;
};
kolejka::kolejka(int x, int y, int z, bool going) {
	from = x;
	to = y;
	quantity = z;
	is_going = going;
}
void kolejka::add() {
	quantity++;
}
std::queue <kolejka*> k;
int levels[] = { 605, 455, 305, 155, 5 };
int height = 10;
int from = 1;
int now = -5;
int to = 1;
int wait = -100;
// sent data
int col = 0;
int actualLevel = 1;
int do_it = 0;
RECT drawArea1 = { 100, 0, 1280, 950 };
RECT drawArea2 = { 50, 400, 650, 422 };

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);


int nowaSzerokosc = 30; 
int nowaWysokosc = 100;  


void addQ(int x, int y) {
	bool f = 1;
	int t = k.size();
	for (int i = 0; i < k.size(); i++) {
		if (x == k.front()->from && y == k.front()->to && !k.front()->is_going && k.front()->quantity < 8) {
			k.push(new kolejka(k.front()->from, k.front()->to, ++k.front()->quantity));
			f = 0;
		}
		else {
			if (k.front()->is_going) {
				k.push(new kolejka(k.front()->from, k.front()->to, k.front()->quantity, 1));
			}
			else {
				k.push(new kolejka(k.front()->from, k.front()->to, k.front()->quantity));
			}
		}
		k.pop();

	}
	if (f) {
		k.push(new kolejka(x, y));
	}
}

//rysowanie 
void OnPaint(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 0));
	Pen pen1(Color(255, 0, 0, 0), 3.0f);
	Pen pen2(Color(255, 255, 0, 255),2.0f);
	Pen pen3(Color(255, 0, 191, 255), 2.0f);
	Pen pen4(Color(255, 148, 0, 211), 2.0f);
	Pen pen5(Color(255, 220, 20, 60), 2.0f);
	Pen pen6(Color(255, 0, 0, 205), 2.0f);
	Image image(L"human.jpg");

	Image* miniatura = image.GetThumbnailImage(nowaSzerokosc, nowaWysokosc);


	// piętra
	graphics.DrawLine(&pen6, 50, 150, 600, 150);   //5
	graphics.DrawLine(&pen5, 880, 300, 1500, 300); //4
	graphics.DrawLine(&pen4, 50, 450, 600, 450);   //3
	graphics.DrawLine(&pen3, 880, 600, 1500, 600); //2
	graphics.DrawLine(&pen2, 50, 750, 600, 750);   //1

	// winda
	graphics.DrawRectangle(&pen, 600, 5, 280, 750);
	std::queue <kolejka*> p = k;
	
	graphics.DrawRectangle(&pen1, 610, height+25, 260, 120);


	int pointXp[] = { 620, 570, 980, 570, 980, 570 };
	int pointX[] = {620, 570, 980, 570, 980, 570};
	int pointY[] = {height+45, 650, 500, 350, 200, 50};
		while (!p.empty()) {
			if (p.front()->is_going) {
				for (int i = 0; i < p.front()->quantity; i++) {
					graphics.DrawImage(miniatura, pointX[0], pointY[0]);
					pointX[0] += 30;

				}
			}
			else {
				for (int i = 0; i < p.front()->quantity; i++) {
					graphics.DrawImage(miniatura, pointX[p.front()->from], pointY[p.front()->from]);
					if ((p.front()->from) % 2 == 0) {
						pointX[p.front()->from] += 30;
						if (pointX[p.front()->from] == 1220) {
							pointX[p.front()->from] = pointXp[p.front()->from];
							pointY[p.front()->from] -= 60;

						}
					}
					else {
						pointX[p.front()->from] -= 30;
						if (pointX[p.front()->from] == 330) {
							pointX[p.front()->from] = pointXp[p.front()->from];
							pointY[p.front()->from] -= 60;

						}
					}
				}
			}
			p.pop();
		}
		delete miniatura;

		
}



void repaintWindow(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea)
{
	if (drawArea == NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	OnPaint(hdc);
	EndPaint(hWnd, &ps);
}





int OnCreate(HWND window)
{
	return 0;
}

// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	// TODO: Place code here.

	height = levels[actualLevel - 1];
	MSG msg;
	HACCEL hAccelTable;

	value = 0;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	// create button and store the handle                                                       
	

	//przyciski pietro 5
	hwndButton = CreateWindow(TEXT("button"),                      
		TEXT("1"),                 
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  
		50, 100,                                  
		30, 30,                              
		hWnd, (HMENU)ID5_BUTTON1, hInstance, NULL);
		       
	hwndButton = CreateWindow(TEXT("button"),                     
		TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  
		50, 70,                                  
		30, 30,                              
		hWnd, (HMENU)ID5_BUTTON2, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 40,
		30, 30,
		hWnd, (HMENU)ID5_BUTTON3, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 10,
		30, 30,
		hWnd, (HMENU)ID5_BUTTON4, hInstance, NULL);


//przyciski pietro 4     
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1400, 250,
		30, 30,
		hWnd, (HMENU)ID4_BUTTON1, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1400, 220,
		30, 30,
		hWnd, (HMENU)ID4_BUTTON2, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1400, 190,
		30, 30,
		hWnd, (HMENU)ID4_BUTTON3, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("5"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1400,160 ,
		30, 30,
		hWnd, (HMENU)ID4_BUTTON4, hInstance, NULL);

	// przyciski pietro 3
		hwndButton = CreateWindow(TEXT("button"),
		TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 400,
		30, 30,
		hWnd, (HMENU)ID3_BUTTON1, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 370,
		30, 30,
		hWnd, (HMENU)ID3_BUTTON2, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 340,
		30, 30,
		hWnd, (HMENU)ID3_BUTTON3, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("5"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 310,
		30, 30,
		hWnd, (HMENU)ID3_BUTTON4, hInstance, NULL);



	// przyciski pietro 2
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1400, 550,
		30, 30,
		hWnd, (HMENU)ID2_BUTTON1, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1400, 520,
		30, 30,
		hWnd, (HMENU)ID2_BUTTON2, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1400, 490,
		30, 30,
		hWnd, (HMENU)ID2_BUTTON3, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("5"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1400, 460,
		30, 30,
		hWnd, (HMENU)ID2_BUTTON4, hInstance, NULL);


	// przyciski pietro 1
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 700,
		30, 30,
		hWnd, (HMENU)ID1_BUTTON1, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 670,
		30, 30,
		hWnd, (HMENU)ID1_BUTTON2, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 640,
		30, 30,
		hWnd, (HMENU)ID1_BUTTON3, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("5"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 610,
		30, 30,
		hWnd, (HMENU)ID1_BUTTON4, hInstance, NULL);




	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("DrawAll"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1000, 35,                                  // the left and top co-ordinates
		80, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON2,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	// create button and store the handle                                                       

	hwndButton = CreateWindow(TEXT("button"), TEXT("Timer ON"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		1100, 30, 100, 30, hWnd, (HMENU)ID_RBUTTON1, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Timer OFF"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		1100, 60, 100, 30, hWnd, (HMENU)ID_RBUTTON2, GetModuleHandle(NULL), NULL);

	OnCreate(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window (low priority)
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	if (!k.empty()) {
		switch (do_it){
		case 0:
			if (actualLevel != k.front()->from) {
				do_it = 1;
				from = actualLevel;
				to = k.front()->from;
			}
			else {
				do_it = 2;
				from = k.front()->from;
				to = k.front()->to;
				k.front()->is_going = 1;
			}
			break;
		case 1:
			if (height == levels[k.front()->from-1]) {
				do_it = 2;
				actualLevel = from;
				from = k.front()->from;
				to = k.front()->to;
				wait = value;
				k.front()->is_going = 1;
			}
			break;
		case 2:
			if (height == levels[k.front()->to-1]) {
				do_it = 0;
				k.pop();
				actualLevel = to;
				from = actualLevel;
				wait = value;
			}
			break;
		default:

			break;
		}
		if (do_it != 0 && now < value - 1 && wait < value - 50) {
			height += (abs(from - to) / (from - to)) * 5;
			now = value;
		}
		
	}
	
	else if (wait < value - 500 && actualLevel != 1) {
		k.push(new kolejka(actualLevel, 1, 0));
	}
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		//case ID5_BUTTON1 : //////////////???????????????????
		//	col++;
		//	if (col > 10)
		//		col = 0;
		//	repaintWindow(hWnd, hdc, ps, &drawArea1);
		//	break;
		case ID_BUTTON2 :
			repaintWindow(hWnd, hdc, ps, NULL);
			break;
		case ID_RBUTTON1:
			SetTimer(hWnd, TMR_1, 25, 0);
			break;
		case ID_RBUTTON2:
			KillTimer(hWnd, TMR_1);
			break;
		case ID1_BUTTON1:
			addQ(1, 2);
			break;
		case ID1_BUTTON2:
			addQ(1, 3);
			break;
		case ID1_BUTTON3:
			addQ(1, 4);
			break;
		case ID1_BUTTON4:
			addQ(1, 5);
			break;
		case ID2_BUTTON1:
			addQ(2, 1);
			break;
		case ID2_BUTTON2:
			addQ(2, 3);
			break;
		case ID2_BUTTON3:
			addQ(2, 4);
			break;
		case ID2_BUTTON4:
			addQ(2, 5);
			break;
		case ID3_BUTTON1:
			addQ(3, 1);
			break;
		case ID3_BUTTON2:
			addQ(3, 2);
			break;
		case ID3_BUTTON3:
			addQ(3, 4);
			break;
		case ID3_BUTTON4:
			addQ(3, 5);
			break;
		case ID4_BUTTON1:
			addQ(4, 1);
			break;
		case ID4_BUTTON2:
			addQ(4, 2);
			break;
		case ID4_BUTTON3:
			addQ(4, 3);
			break;
		case ID4_BUTTON4:
			addQ(4, 5);
			break;
		case ID5_BUTTON1:
			addQ(5, 1);
			break;
		case ID5_BUTTON2:
			addQ(5, 2);
			break;
		case ID5_BUTTON3:
			addQ(5, 3);
			break;
		case ID5_BUTTON4:
			addQ(5, 4);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here (not depend on timer, buttons)
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			//force window to repaint
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			value++;
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
