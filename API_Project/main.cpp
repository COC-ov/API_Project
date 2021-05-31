#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	//콘솔창 띄움

#define R 20
int x, y;
int xi, yi;
void OnTimer(HWND);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wndClass;
	TCHAR className[11] = L"Class Name";
	TCHAR titleName[10] = L"윈플실습 5-3";
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
		50, 40, 1000, 600,
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

void OnTimer(HWND hWnd)
{
	RECT crt;
	RECT rt;

	GetClientRect(hWnd, &crt);
	if (x <= R || x >= crt.right - R)
		xi *= -1;
	if (y <= R || y >= crt.bottom - R)	//x,y는 원의 중심 원이 화면 밖으로 나가려고 할때 -1을 곱하여 방향을 바꿈
		yi *= -1;

	x += xi;
	y += yi;

	SetRect(&rt, x - R - 5, y - R - 5, x + R + 5, y + R + 5);	//x와 y의 최대 이동거리+5픽셀 부분만 다시그리도록 수정
	InvalidateRect(hWnd, &rt, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	RECT crt;
	int i;

	switch (uMsg)
	{
	case WM_CREATE:		//(50,50)에서 시작해서 25초마다 x축으로 4픽셀 y축으로 5픽셀 씩 움직이게 함
		x = 50;
		y = 50;
		xi = 4;
		yi = 5;
		SetTimer(hWnd, 1, 25, NULL);	//타이머 번호, 주기, 타이머 함수
		break;
	case WM_TIMER:
		OnTimer(hWnd);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &crt);
		for (i = 0; i < crt.right; i += 10)
		{
			MoveToEx(hDC, i, 0, NULL);
			LineTo(hDC, i, crt.bottom);		//10간격으로 세로선을 그음

		}
		for (i = 0; i < crt.bottom; i += 10)
		{
			MoveToEx(hDC, 0, i, NULL);
			LineTo(hDC, crt.right, i);		//10간격으로 가로선을 그음
		}

		//ㅡㅡㅡ 윈을 그림 ㅡㅡㅡ
		hPen = CreatePen(PS_INSIDEFRAME, 5, RGB(255, 0, 0));	//선의 형태, 굵기, 색상
		oldPen = (HPEN)SelectObject(hDC, hPen);
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, x - R, y - R, x + R, y + R);				//중심이 x,y인 원을 그림
		DeleteObject(SelectObject(hDC, oldPen));
		DeleteObject(SelectObject(hDC, oldBrush));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
		break;
	}
	return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}