#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	//�ܼ�â ���

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
	TCHAR titleName[10] = L"���ýǽ� 5-3";
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
	if (y <= R || y >= crt.bottom - R)	//x,y�� ���� �߽� ���� ȭ�� ������ �������� �Ҷ� -1�� ���Ͽ� ������ �ٲ�
		yi *= -1;

	x += xi;
	y += yi;

	SetRect(&rt, x - R - 5, y - R - 5, x + R + 5, y + R + 5);	//x�� y�� �ִ� �̵��Ÿ�+5�ȼ� �κи� �ٽñ׸����� ����
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
	case WM_CREATE:		//(50,50)���� �����ؼ� 25�ʸ��� x������ 4�ȼ� y������ 5�ȼ� �� �����̰� ��
		x = 50;
		y = 50;
		xi = 4;
		yi = 5;
		SetTimer(hWnd, 1, 25, NULL);	//Ÿ�̸� ��ȣ, �ֱ�, Ÿ�̸� �Լ�
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
			LineTo(hDC, i, crt.bottom);		//10�������� ���μ��� ����

		}
		for (i = 0; i < crt.bottom; i += 10)
		{
			MoveToEx(hDC, 0, i, NULL);
			LineTo(hDC, crt.right, i);		//10�������� ���μ��� ����
		}

		//�ѤѤ� ���� �׸� �ѤѤ�
		hPen = CreatePen(PS_INSIDEFRAME, 5, RGB(255, 0, 0));	//���� ����, ����, ����
		oldPen = (HPEN)SelectObject(hDC, hPen);
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, x - R, y - R, x + R, y + R);				//�߽��� x,y�� ���� �׸�
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