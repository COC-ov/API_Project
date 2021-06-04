#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	//콘솔창 띄움

#define R 20
int x[10], y[10];
int xi[10], yi[10];
HBITMAP hBit;
void OnTimer(HWND);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
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

void OnTimer(HWND hWnd)		//메모리 디시를 이용해 hBit에 미리 그려 놓는 함수
{
	RECT crt;
	HDC hDC, hMemDC;
	HBITMAP oldBit;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	int i;

	GetClientRect(hWnd, &crt);
	hDC = GetDC(hWnd);

	if (hBit == NULL)
		hBit = CreateCompatibleBitmap(hDC, crt.right, crt.bottom);	//화면 크기의 비트맵을 생성
	hMemDC = CreateCompatibleDC(hDC);								//메모리디시에 만들어진 비트맵을 저장
	oldBit = (HBITMAP)SelectObject(hMemDC, hBit);					//oldBit에 저장

	FillRect(hMemDC, &crt, GetSysColorBrush(COLOR_WINDOW));

	//ㅡㅡㅡ화면에 선을 그림 ㅡㅡㅡ
	for (i = 0; i < crt.right; i += 10)
	{
		MoveToEx(hMemDC, i, 0, NULL);
		LineTo(hMemDC, i, crt.bottom);		//10간격으로 세로선을 그음

	}
	for (i = 0; i < crt.bottom; i += 10)
	{
		MoveToEx(hMemDC, 0, i, NULL);
		LineTo(hMemDC, crt.right, i);		//10간격으로 가로선을 그음
	}

	//ㅡㅡㅡ 윈을 그림 ㅡㅡㅡ
	hPen = CreatePen(PS_INSIDEFRAME, 5, RGB(255, 0, 0));	//선의 형태, 굵기, 색상
	oldPen = (HPEN)SelectObject(hMemDC, hPen);
	hBrush = CreateSolidBrush(RGB(0, 0, 255));
	oldBrush = (HBRUSH)SelectObject(hMemDC, hBrush);

	for (int ball = 0; ball < 10; ++ball)
	{
		if (x[ball] <= R || x[ball] >= crt.right - R)
			xi[ball] *= -1;
		if (y[ball] <= R || y[ball] >= crt.bottom - R)	//x,y는 원의 중심 원이 화면 밖으로 나가려고 할때 -1을 곱하여 방향을 바꿈
			yi[ball] *= -1;

		x[ball] += xi[ball];
		y[ball] += yi[ball];

		Ellipse(hMemDC, x[ball] - R, y[ball] - R, x[ball] + R, y[ball] + R);				//중심이 x,y인 원을 그림
	}

	DeleteObject(SelectObject(hMemDC, oldPen));
	DeleteObject(SelectObject(hMemDC, oldBrush));

	SelectObject(hMemDC, oldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWnd, hDC);
	InvalidateRect(hWnd, NULL, FALSE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_CREATE:		//(50,50)에서 시작해서 25초마다 x축으로 4픽셀 y축으로 5픽셀 씩 움직이게 함
		//ㅡㅡㅡ 공들의 시작좌표설정 ㅡㅡㅡ
		for (int i = 0; i < 10; ++i)
		{
			x[i] = 50;
			y[i] = 50;
		}
		//ㅡㅡㅡ 공들의 이동거리 설정 ㅡㅡㅡ
		for (int i = 0; i < 10; ++i)
		{
			xi[i] = (rand() % 20) + 1;
			yi[i] = (rand() % 20) + 1;
		}
		SetTimer(hWnd, 1, 25, NULL);	//타이머 번호, 주기, 타이머 함수
		break;
	case WM_TIMER:
		OnTimer(hWnd);	//OnTimer함수 호출
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		if (hBit) DrawBitmap(hDC, 0, 0, hBit);		//DrawBitmap함수로 그려진 비트맵을 출력함
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		if (hBit) DeleteObject(hBit);
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
		break;
	}
	return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}


//출처: https://ymcoder.tistory.com/155 [자료 저장소]