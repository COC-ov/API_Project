#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include "resource.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	//콘솔창 띄움

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInst;

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
	static int bx, by;	//비트맵의 정보 저장
	static RECT rt;	//윈도우의 작업영역의 크기 저장
	BITMAP bit;
	static int r;
	static char c;
	static DWORD keyW;

	switch (uMsg)
	{
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));	//처음엔 안되었다가 껐다가키니까됨, 비트맵 이미지를 로드
		//비트맵의 정보를 알아낸다.
		GetObject(hBitmap, sizeof(BITMAP), &bit);
		bx = bit.bmWidth;	
		by = bit.bmHeight;
		break;																//hInst는 응용프로그램의 인스턴스 값

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
			GetClientRect(hWnd, &rt);	//작업영역의 크기 알아내기
			break;
		case '2':
			c = '2';
			GetClientRect(hWnd, &rt);	//작업영역의 크기 알아내기
			break;
		case '3':
			c = '3';
			GetClientRect(hWnd, &rt);	//작업영역의 크기 알아내기
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
		hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);		//memdc에 hBitmap을 서정
		if (r % 2 == 1)
			keyW = 0x00330008;
		else
			keyW = 0x00CC0020;

		if (c == 'a')
			StretchBlt(hDC, rt.left, rt.top, rt.right, rt.bottom, memDC, 0, 0, bx, by, keyW); //그림을 화면에 맞춰 출력
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
		DeleteDC(memDC);					//memDC삭제
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}