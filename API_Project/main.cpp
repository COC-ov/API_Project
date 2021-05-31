#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	//콘솔창 띄움

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Expand(float expand, HDC hDC, HDC memDC, int startX, int startY, int endX, int endY, float rx, float ry, HBITMAP paint);
void paste(struct Paint* copy, RECT rt, HDC hDC, HDC memDC, HBITMAP paint, int startX, int startY, int endX, int endY);
void pasteH(struct Paint* copy, RECT rt, HDC hDC, HDC memDC, HBITMAP paint, int startX, int startY, int endX, int endY);
void pasteV(struct Paint* copy, RECT rt, HDC hDC, HDC memDC, HBITMAP paint, int startX, int startY, int endX, int endY);

HINSTANCE hInst;

struct Draw
{
	HBITMAP hBitmap;
	int bx;
	int by;
};

struct Paint
{
	float cx, cy, cw, ch;
};
//밑그림의 정보를 담을 구조체

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
	wndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC, memDC;
	PAINTSTRUCT ps;
	static int cnt, id,
		startX, startY,			//돋보기의 시작좌표(눌렸을때)
		endX, endY,			//돋보기의 끝좌표(떨어졌을때)
		dragX, dragY,		//드레그 될동안의 좌표
		Draw, mx, my, m;
	static float rx, ry,	//그림과 화면의 비율
		expand = 1;
	static RECT rt;	//윈도우의 작업영역의 크기 저장
	BITMAP bit;
	static struct Draw paint[2];
	static struct Paint copy;
	static char c;
	static BOOL Paste = FALSE;
	switch (uMsg)
	{
	case WM_CREATE:
		paint[0].hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));	//밑그림 1
		paint[1].hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));	//밑그림 2

		//비트맵의 정보를 알아낸다.
		for (int i = 0; i < 2; ++i)
		{
			GetObject(paint[i].hBitmap, sizeof(BITMAP), &bit);
			paint[i].bx = bit.bmWidth;
			paint[i].by = bit.bmHeight;
		}

		//작업영역의 크기 알아내기
		GetClientRect(hWnd, &rt);
		rx = (float)rt.right / paint[id].bx;
		ry = (float)rt.bottom / paint[id].by;
		break;
	case WM_LBUTTONDOWN:		//돋보기의 시작 좌표를 알아냄
		if (Draw == 0)
		{
			startX = dragX = LOWORD(lParam);
			startY = dragY = HIWORD(lParam);
			Draw++;
		}
		break;

	case WM_MOUSEMOVE:			//돋보기 사각형을 그릴 끝 좌표를 알아냄
		hDC = GetDC(hWnd);
		if (Draw==1)
		{
			SetROP2(hDC, R2_XORPEN);
			SelectObject(hDC, (HPEN)GetStockObject(WHITE_PEN));
			SelectObject(hDC, (HBRUSH)GetStockObject(NULL_BRUSH));
			//printf("%d %d %d %d \n", startX, startY, endX, endY);
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			Rectangle(hDC, startX, startY, dragX, dragY);
			Rectangle(hDC, startX, startY, endX, endY);
			dragX = endX;
			dragY = endY;
		}
		ReleaseDC(hWnd, hDC);
		break;

	case WM_LBUTTONUP:
		Draw++;
		break;

	case WM_KEYDOWN:
		if (Paste == TRUE)
		{
			switch (wParam)
			{
			case VK_RIGHT:   // 오른쪽 키가 눌러졌을때.
				mx += 40;
				break;
			case VK_LEFT:   // 왼쪽키가 눌러졌을때.
				mx -= 40;
				break;
			case VK_UP:    // 위쪽 키가 눌러졌을때.
				my -= 40;
				break;
			case VK_DOWN:    // 아래쪽 키가 눌러 졌을때.
				my += 40;
				break;
			}
			InvalidateRect(hWnd, NULL, true);
		}
		break;
		//ㅡㅡㅡ메뉴ㅡㅡㅡ
	case WM_COMMAND:
		hDC = GetDC(hWnd);
		memDC = CreateCompatibleDC(hDC);	//메모리디시인memedc생성

		switch (LOWORD(wParam))
		{
		case ID_Image1:	//이미지 1
			id = 0;
			Draw = 0;
			rx = (float)rt.right / paint[id].bx;
			ry = (float)rt.bottom / paint[id].by;
			startX=0, startY=0,			//돋보기의 시작좌표(눌렸을때)
				endX=0, endY = 0;			//돋보기의 끝좌표(떨어졌을때)
			Draw = 0;
			mx = 0, my = 0, m = 0;
			Paste = FALSE;
			expand = 1;
			break;
		case ID_Image2:	//이미지 2
			Draw = 0;
			rx = (float)rt.right / paint[id].bx;
			ry = (float)rt.bottom / paint[id].by;
			startX = 0, startY = 0,			//돋보기의 시작좌표(눌렸을때)
				endX = 0, endY = 0;			//돋보기의 끝좌표(떨어졌을때)
			Draw = 0;
			mx = 0, my = 0, m = 0;
			expand = 1;
			Paste = FALSE;
			break;
		case ID_Expand1:
			expand = 1.2;
			break;
		case ID_Expand2:
			expand = 1.4;
			break;
		case ID_Origin:
			expand = 1;
			break;
		case ID_C:
			Paste = TRUE;
			break;
		case ID_V:
			c = 'p';
			break;
		case ID_FullV:
			cnt++;
			break;
		}

		if (LOWORD(wParam) != ID_C)
			InvalidateRect(hWnd, NULL, true);

		DeleteDC(memDC);				//memDC삭제
		DeleteDC(hDC);					//memDC삭제
		break;

	case WM_CHAR:		//키보드 입력
		hDC = GetDC(hWnd);
		memDC = CreateCompatibleDC(hDC);	//메모리디시인memedc생성

		switch (wParam)
		{
		case '1':		//이미지 1
			id = 0;
			rx = (float)rt.right / paint[id].bx;
			ry = (float)rt.bottom / paint[id].by;
			startX = 0, startY = 0,			//돋보기의 시작좌표(눌렸을때)
				endX = 0, endY = 0;			//돋보기의 끝좌표(떨어졌을때)
			Draw = 0;
			mx = 0, my = 0, m = 0;
			Paste = FALSE;
			expand = 1;
			break;
		case '2':		//이미지 2
			id = 1;
			rx = (float)rt.right / paint[id].bx;
			ry = (float)rt.bottom / paint[id].by;
			startX = 0, startY = 0,			//돋보기의 시작좌표(눌렸을때)
				endX = 0, endY = 0;			//돋보기의 끝좌표(떨어졌을때)
			Draw = 0;
			mx = 0, my = 0, m = 0;
			expand = 1;
			Paste = FALSE;
			break;
		case 'R':
		case 'r':
			startX = 0, startY = 0,			//돋보기의 시작좌표(눌렸을때)
				endX = 0, endY = 0;			//돋보기의 끝좌표(떨어졌을때)
			Draw = 0;
			mx = 0, my = 0, m = 0;
			Paste = FALSE;
			expand = 1;
			break;
		case '3':
			expand = 1.2;
			break;
		case '4':
			expand = 1.4;
			break;
		case '0':
			expand = 1;
			break;
		case 'c':
			Paste = TRUE;
			break;
		case 'p':
			Paste = TRUE;
			c = 'p';
			break;
		case 'm':
			m+=40;
			break;
		case 'n':
			m -= 40;
			break;
		case 'f':
			cnt++;
			break;
		case 'h':
		case 'H':
			c = 'h';
			break;
		case 'v':
		case 'V':
			c = 'v';
			break;
		case 'Q':		//종료
		case 'q':
			PostQuitMessage(0);
			break;
		}

		if(wParam != 'c')
			InvalidateRect(hWnd, NULL, true);

		DeleteDC(memDC);				//memDC삭제
		DeleteDC(hDC);					//memDC삭제
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		memDC = CreateCompatibleDC(hDC);	//메모리디시인memedc생성

		SelectObject(memDC, paint[id].hBitmap);	//비트맵 선택

		if (cnt % 2 == 1)
			StretchBlt(hDC, rt.left, rt.top, rt.right, rt.bottom, memDC, copy.cx, copy.cy, copy.cw, copy.ch, SRCCOPY);
		else
		{
			StretchBlt(hDC, rt.left, rt.top, rt.right, rt.bottom, memDC, 0, 0, paint[id].bx, paint[id].by, SRCCOPY);	//이미지를 화면에 출력
			Expand(expand, hDC, memDC, startX + mx, startY + my, endX - startX+m, endY - startY+m, rx, ry, paint[id].hBitmap);
		}


		//ㅡㅡㅡ돋보기 사각형을 그림ㅡㅡㅡ

		SetROP2(hDC, R2_XORPEN);
		SelectObject(hDC, (HPEN)GetStockObject(WHITE_PEN));
		SelectObject(hDC, (HBRUSH)GetStockObject(NULL_BRUSH));
		Rectangle(hDC, startX+mx, startY+my, endX+mx+m, endY+my+m);

		printf("%c %d", c, Paste);
		if (Paste == TRUE)
		{
			copy.cx = (startX + mx) / rx;
			copy.cy = (startY + my) / ry;
			copy.cw = (endX - startX + m) / rx / expand;
			copy.ch = (endY - startY + m) / ry / expand;		//이미지 복사 정보 저장
			if (c == 'p')
				paste(&copy, rt, hDC, memDC, paint[id].hBitmap, startX, startY, endX + m, endY + m);
			if (c == 'h')
				pasteH(&copy, rt, hDC, memDC, paint[id].hBitmap, startX, startY, endX + m, endY + m);
			if (c == 'v')
				pasteV(&copy, rt, hDC, memDC, paint[id].hBitmap, startX, startY, endX + m, endY + m);
		}

		DeleteDC(memDC);				//memDC삭제
		DeleteDC(hDC);					//memDC삭제s
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		DeleteObject(paint[0].hBitmap);
		DeleteObject(paint[1].hBitmap);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Expand(float expand, HDC hDC, HDC memDC, int x, int y, int w, int h, float rx, float ry, HBITMAP paint)
{
	SelectObject(memDC, paint);	//비트맵 선택

	StretchBlt(hDC, x, y, w, h,
		memDC, x / rx, y / ry, w / rx / expand, h / ry / expand, SRCCOPY);	//이미지를 화면에 출력
}

void paste(struct Paint* copy, RECT rt, HDC hDC, HDC memDC, HBITMAP paint, int startX, int startY, int endX, int endY )
{
	SelectObject(memDC, paint);	//비트맵 선택
	StretchBlt(hDC, rt.left, rt.top, endX - startX, endY - startY, memDC, copy->cx, copy->cy, copy->cw, copy->ch, SRCCOPY);
	SelectObject(hDC, (HPEN)GetStockObject(WHITE_PEN));
	SelectObject(hDC, (HBRUSH)GetStockObject(NULL_BRUSH));
	Rectangle(hDC, rt.left, rt.top, endX - startX, endY - startY);
}
//--- 상하 반전출력 ---
void pasteH(struct Paint* copy, RECT rt, HDC hDC, HDC memDC, HBITMAP paint, int startX, int startY, int endX, int endY )
{
	SelectObject(memDC, paint);	//비트맵 선택
	StretchBlt(hDC, rt.left, rt.top, endX - startX, endY - startY, memDC, copy->cx + copy->cw, copy->cy + copy->ch, -copy->cw, -copy->ch, SRCCOPY);
	SelectObject(hDC, (HPEN)GetStockObject(WHITE_PEN));
	SelectObject(hDC, (HBRUSH)GetStockObject(NULL_BRUSH));
	Rectangle(hDC, rt.left, rt.top, endX - startX, endY - startY);
}
//--- 좌우 반전출력 ---
void pasteV(struct Paint* copy, RECT rt, HDC hDC, HDC memDC, HBITMAP paint, int startX, int startY, int endX, int endY)
{
	SelectObject(memDC, paint);	//비트맵 선택
	StretchBlt(hDC, rt.left, rt.top, endX - startX, endY - startY, memDC, copy->cx + copy->cw, copy->cy , -copy->cw, copy->ch, SRCCOPY);
	SelectObject(hDC, (HPEN)GetStockObject(WHITE_PEN));
	SelectObject(hDC, (HBRUSH)GetStockObject(NULL_BRUSH));
	Rectangle(hDC, rt.left, rt.top, endX - startX, endY - startY);
}