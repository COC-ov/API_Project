#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include "resource.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	//콘솔창 띄움

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InBitmap(int, int, int, int);
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
	hWnd = CreateWindow(className, titleName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
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
	static int bx, by,	//비트맵의 정보 저장
		mx, my;			//마우스 좌표 저장
	static RECT rt;	//윈도우의 작업영역의 크기 저장
	BITMAP bit;
	static int r, h, w, num, select;	//색 반전, 그림의 출력 높이, 너비, 그림의 번호, 선택 여부
	static char c;
	static DWORD keyW;
	HBRUSH myBrush, oldBrush;
	static int move[9];	//(x 1,y 1) (x 2, y2)...

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
		case 'A':
		case 'a':
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

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		InvalidateRgn(hWnd, NULL, TRUE);
		break;
		
	case WM_KEYDOWN:
		if (wParam == VK_LEFT) move[select] -= 40;
		if (wParam == VK_RIGHT) move[select] += 40;
		InvalidateRect(hWnd, NULL, true);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		memDC = CreateCompatibleDC(hDC);	//메모리디시인memedc생성
		hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);		//memdc에 hBitmap을 서정
		myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		oldBrush = (HBRUSH)SelectObject(hDC, myBrush);
		if (r % 2 == 1)
			keyW = 0x00330008;
		else
			keyW = 0x00CC0020;

		if (c == 'a')
		{
			StretchBlt(hDC, rt.left+move[0], rt.top, rt.right, rt.bottom, memDC, 0, 0, bx, by, keyW); //전체그림 보기

			if (rt.left + move[0] + rt.right > rt.right)
				StretchBlt(hDC, rt.left + move[0] - rt.right, rt.top, rt.right, rt.bottom, memDC, 0, 0, bx, by, keyW);
			else if (rt.left + move[0] < 0)
				StretchBlt(hDC, rt.left + move[0] + rt.right, rt.top, rt.right, rt.bottom, memDC, 0, 0, bx, by, keyW);
		}
		else if (c == '2')
		{
			w = rt.right / 2;
			h = rt.bottom / 2;

			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{	
					StretchBlt(hDC, w * j + move[num], h * i, w, h, memDC, 0, 0, bx, by, keyW);
					if (InBitmap(mx - w * j, my - h * i, w, h))
						select = num;

					if (w * j + move[num]+w > rt.right)
						StretchBlt(hDC,	w * j + move[num]-rt.right, h * i, w, h, memDC, 0, 0, bx, by, keyW);
					else if (w * j + move[num] < 0)
						StretchBlt(hDC, rt.right +w * j + move[num], h * i, w, h, memDC, 0, 0, bx, by, keyW);

					if (select == num)
					{
						Rectangle(hDC, w * j + move[num], h * i, w * (j + 1) + move[num], h * (i + 1));
						if (w * j + move[num] + w > rt.right)
							Rectangle(hDC, w * j + move[num] - rt.right, h * i,w * (j + 1) + move[num] - rt.right, h * (i + 1));
						else if (w * j + move[num] < 0)
							Rectangle(hDC, rt.right + w * j + move[num], h * i, rt.right + w * (j + 1) + move[num], h * (i + 1));
					}
					num++;
				}
			}
			num = 0;
		}
		else if (c == '3')
		{
			w = rt.right / 3;
			h = rt.bottom / 3;

			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					StretchBlt(hDC, w * j + move[num], h * i, w, h, memDC, 0, 0, bx, by, keyW);
					if (InBitmap(mx - w * j, my - h * i, w, h))
						select = num;

					if (w * j + move[num] + w > rt.right)
						StretchBlt(hDC, w * j + move[num] - rt.right, h * i, w, h, memDC, 0, 0, bx, by, keyW);
					else if (w * j + move[num] < 0)
						StretchBlt(hDC, rt.right + w * j + move[num], h * i, w, h, memDC, 0, 0, bx, by, keyW);

					if (select == num)
					{
						Rectangle(hDC, w * j + move[num], h * i, w * (j + 1) + move[num], h * (i + 1));
						if (w * j + move[num] + w > rt.right)
							Rectangle(hDC, w * j + move[num] - rt.right, h * i, w * (j + 1) + move[num] - rt.right, h * (i + 1));
						else if (w * j + move[num] < 0)
							Rectangle(hDC, rt.right + w * j + move[num], h * i, rt.right + w * (j + 1) + move[num], h * (i + 1));
					}
					num++;
				}
			}
			num = 0;
		}
		else
			BitBlt(hDC, move[0], 0, bx, by, memDC, 0, 0, keyW);
		
		SelectObject(memDC, hOldBitmap);
		DeleteDC(memDC);					//memDC삭제

		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL InBitmap(int x, int y, int w, int h)
{
	if (0<x&&x<w&&0<y&&y<h)
		return TRUE;
	else return FALSE;
}