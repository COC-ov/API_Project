#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include "resource.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	//�ܼ�â ���

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wndClass;
	TCHAR className[11] = L"Class Name";
	TCHAR titleName[10] = L"���ýǽ�";
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
	hWnd = CreateWindow(className, titleName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
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
	static int bx, by;	//��Ʈ���� ���� ����
	static RECT rt;	//�������� �۾������� ũ�� ����
	BITMAP bit;
	static int r;
	static char c;
	static DWORD keyW;

	switch (uMsg)
	{
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));	//ó���� �ȵǾ��ٰ� ���ٰ�Ű�ϱ��, ��Ʈ�� �̹����� �ε�
		//��Ʈ���� ������ �˾Ƴ���.
		GetObject(hBitmap, sizeof(BITMAP), &bit);
		bx = bit.bmWidth;	
		by = bit.bmHeight;
		break;																//hInst�� �������α׷��� �ν��Ͻ� ��

	case WM_CHAR:
		switch (wParam)
		{
		case 'r':
		case'R':
			r++;
			break;
		case 'a':
		case 'A':
			c = 'a';
			GetClientRect(hWnd, &rt);	//�۾������� ũ�� �˾Ƴ���
			break;
		case '2':
			c = '2';
			GetClientRect(hWnd, &rt);	//�۾������� ũ�� �˾Ƴ���
			break;
		case '3':
			c = '3';
			GetClientRect(hWnd, &rt);	//�۾������� ũ�� �˾Ƴ���
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
		memDC = CreateCompatibleDC(hDC);	//�޸𸮵����memedc����
		hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);		//memdc�� hBitmap�� ����
		if (r % 2 == 1)
			keyW = 0x00330008;
		else
			keyW = 0x00CC0020;

		if (c == 'a')
			StretchBlt(hDC, rt.left, rt.top, rt.right, rt.bottom, memDC, 0, 0, bx, by, keyW); //�׸��� ȭ�鿡 ���� ���
		else if (c == '2')
		{
			StretchBlt(hDC, rt.left, rt.top, rt.right / 2, rt.bottom / 2, memDC, 0, 0, bx, by, keyW); //1
			StretchBlt(hDC, rt.right / 2, rt.bottom / 2, rt.right / 2, rt.bottom / 2, memDC, 0, 0, bx, by, keyW); //2
			StretchBlt(hDC, rt.right / 2, rt.top, rt.right / 2, rt.bottom / 2, memDC, 0, 0, bx, by, keyW); //3
			StretchBlt(hDC, rt.left, rt.bottom / 2, rt.right / 2, rt.bottom / 2, memDC, 0, 0, bx, by, keyW); //4
		}
		else if (c == '3')
		{
			StretchBlt(hDC, rt.left, rt.top, rt.right / 3, rt.bottom / 3, memDC, 0, 0, bx, by, keyW); //1
			StretchBlt(hDC, rt.right / 3, rt.top, rt.right / 3, rt.bottom / 3, memDC, 0, 0, bx, by, keyW); //2
			StretchBlt(hDC, rt.right / 3 * 2, rt.top, rt.right / 3, rt.bottom / 3, memDC, 0, 0, bx, by, keyW); //3
			StretchBlt(hDC, rt.left, rt.bottom / 3, rt.right / 3, rt.bottom / 3, memDC, 0, 0, bx, by, keyW); //4
			StretchBlt(hDC, rt.right / 3, rt.bottom / 3, rt.right / 3, rt.bottom / 3, memDC, 0, 0, bx, by, keyW); //5
			StretchBlt(hDC, rt.right / 3 * 2, rt.bottom / 3, rt.right / 3, rt.bottom / 3, memDC, 0, 0, bx, by, keyW); //6
			StretchBlt(hDC, rt.left, rt.bottom / 3 * 2, rt.right / 3, rt.bottom / 3, memDC, 0, 0, bx, by, keyW); //7
			StretchBlt(hDC, rt.right / 3, rt.bottom / 3 * 2, rt.right / 3, rt.bottom / 3, memDC, 0, 0, bx, by, keyW); //8
			StretchBlt(hDC, rt.right / 3 * 2, rt.bottom / 3 * 2, rt.right / 3, rt.bottom / 3, memDC, 0, 0, bx, by, keyW); //9
		}
		else
			BitBlt(hDC, 0, 0, bx, by, memDC, 0, 0, keyW);
		
		SelectObject(memDC, hOldBitmap);
		DeleteDC(memDC);					//memDC����
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}