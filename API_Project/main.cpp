#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	//�ܼ�â ���

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Split(HDC hDC, HDC memDC, int id, RECT rt, struct Draw paint[], int, BOOL);	//���� ������ ���
void Mix(int piece, struct Paint pos[]);	//���� ������ ��ġ�� �������� ����

HINSTANCE hInst;

struct Draw
{
	HBITMAP hBitmap;
	int bx;
	int by;
};

struct Paint
{
	int wx, wy, px, py;
};
//�ر׸��� ������ ���� ����ü

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wndClass;
	TCHAR className[11] = L"Class Name";
	TCHAR titleName[10] = L"���ýǽ� 5-2";
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
	static int id,	//��Ʈ���� ���� ����
		piece = 3;		//������ ����
	static RECT rt;	//�������� �۾������� ũ�� ����
	BITMAP bit;
	static char c;
	static struct Draw paint[2];
	static BOOL numClick = TRUE;

	switch (uMsg)
	{
	case WM_CREATE:
		paint[0].hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));	//�ر׸� 1
		paint[1].hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));	//�ر׸� 1

		//��Ʈ���� ������ �˾Ƴ���.
		for (int i = 0; i < 2; ++i)
		{
			GetObject(paint[i].hBitmap, sizeof(BITMAP), &bit);
			paint[i].bx = bit.bmWidth;
			paint[i].by = bit.bmHeight;
		}

		//�۾������� ũ�� �˾Ƴ���
		GetClientRect(hWnd, &rt);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'F':
		case 'f':		//��ü �׸� �����ֱ�
			c = 'f';
			break;
		case '1':		//�ر׸� 1
			id = 0;
			break;		//�ر׸� 2
		case '2':	
			id = 1;
			break;
		case 's':
		case 'S':
			c = 's';	//���� �ε�
			break;
		case '3':
			piece = 3;	//3x3����
			break;
		case '4':
			piece = 4;	//4x4����
			break;
		case '5':		//5x5����
			piece = 5;
			break;
		case 'q':		//����
		case 'Q':
			PostQuitMessage(0);
			break;
		}

		if(wParam == '1' || wParam=='2'||wParam=='3'|| wParam == '4')
			numClick = TRUE;

		InvalidateRect(hWnd, NULL, true);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		memDC = CreateCompatibleDC(hDC);	//�޸𸮵����memedc����

		(HBITMAP)SelectObject(memDC, paint[id].hBitmap);	//��Ʈ�� ����
		//BitBlt(hDC,0, 0, paint[id].bx, paint[id].by, memDC, 0, 0, SRCCOPY);	//�׳� ���

		switch (c)
		{
		case 'f':
			StretchBlt(hDC, rt.left, rt.top, rt.right, rt.bottom, memDC, 0, 0, paint[id].bx, paint[id].by, SRCCOPY); //ȭ�鿡 ���缭 ���
			break;
		case 's':
			Split(hDC, memDC, id, rt, paint, piece, numClick);
			numClick = FALSE;
			break;
		}


		DeleteDC(memDC);				//memDC����
		DeleteDC(hDC);					//memDC����
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

void Split(HDC hDC, HDC memDC, int id, RECT rt, struct Draw paint[], int piece, BOOL numClick)
{
	int ww = rt.right / piece, wh = rt.bottom / piece,	//ȭ���� �ʺ�� ����
		pw = paint[id].bx / piece, ph = paint[id].by / piece;	//�׸��� �ʺ�� ����
	static struct Paint pos[25];

	//�ѤѤ������� �������� �����ѤѤ�
	if (numClick)	//����Ű�� ���������� ���� ���̵��� ����
		Mix(piece, pos);

	//�ѤѤѸ��콺�� ������ �̵���Ŵ�ѤѤ�






	//�ѤѤ������� ��¤ѤѤѤ�
	for (int i = 1; i < piece*piece; ++i)	//0�� ������ ��� ����
		StretchBlt(hDC, ww * pos[i].wx, wh * pos[i].wy, ww, wh,		//��ġ
			memDC, paint[id].bx / piece * pos[i].px , paint[id].by / piece*pos[i].py, pw, ph, SRCCOPY);	//����
}

void Mix(int piece, struct Paint pos[])	//���� ������ ��ġ�� �������� ����
{
	int rn, temp, num, k = 0;

	for (int i = 0; i < piece; ++i)	//������ ��ġ�� ������ ������ �׸� ������� ����ü�� ����
	{
		for (int j = 0; j < piece; ++j)
		{
			pos[k].wx = i;
			pos[k].wy = j;
			pos[k].px = i;
			pos[k].py = j;
			k++;
		}
	}

	for (int i = 0; i < piece*piece - 1; ++i)	//������ ��ġ�� ���´�
	{
		srand(time(NULL));
		rn = rand() % (piece*piece - i) + i; // i ���� num-1 ���̿� ������ ���� ���� 

		temp = pos[i].wx; 
		pos[i].wx = pos[rn].wx; 
		pos[rn].wx = temp; 

		temp = pos[i].wy;
		pos[i].wy = pos[rn].wy;
		pos[rn].wy = temp;
	}

}