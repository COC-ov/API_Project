#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include "resource.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	//콘솔창 띄움

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InBitmap(int, int, int, int);
HINSTANCE hInst;

//struct Draw
//{
//};
////밑그림의 정보를 담을 구조체

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wndClass;
	TCHAR className[11] = L"Class Name";
	TCHAR titleName[10] = L"윈플실습";
	hInst = hInstance;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = className;
	RegisterClass(&wndClass);
	hWnd = CreateWindow(className, titleName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		300, 50, 500, 600,
		NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC, memDC;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap, hOldBitmap;
	static int bx, by,	//비트맵의 정보 저장
		wx, wy, ww, wh;	//윈도우의 크기 정보 저장
	static RECT rt;	//윈도우의 작업영역의 크기 저장
	BITMAP bit;
	static char c;

	switch (uMsg)
	{
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));	//밑그림 1

		//비트맵의 정보를 알아낸다.
		GetObject(hBitmap, sizeof(BITMAP), &bit);
		bx = bit.bmWidth;	
		by = bit.bmHeight;

		//작업영역의 크기 알아내기
		GetClientRect(hWnd, &rt);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'F':
		case 'f':
			c = 'f';
			break;
		case '2':
			hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));	//밑그림 2
			GetObject(hBitmap, sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			break;
		case '1':
			hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));	//밑그림 1
			break;
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			break;
		}

		InvalidateRect(hWnd, NULL, true);
		break;		

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		memDC = CreateCompatibleDC(hDC);	//메모리디시인memedc생성
		hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap
		);

		if (c == 'f')
			StretchBlt(hDC, rt.right, rt.top, rt.left, rt.bottom, memDC, 0, 0, bx, by, SRCCOPY); //전체그림 보기

		SelectObject(memDC, hOldBitmap);
		DeleteDC(memDC);					//memDC삭제
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
