#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"

//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	//�ܼ�â ���

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Split(HDC hDC, HDC memDC, int id, RECT rt, struct Draw paint[],
	int piece, BOOL numClick, int md, BOOL* mousePiece, BOOL);	//���� ������ ���
void Mix(int piece, struct Paint pos[]);	//���� ������ ��ġ�� �������� ����
int Mouse_Direct(int, int, int, int);	//���콺�� �̵������� ��ȯ
void MovePuzzle(struct Draw paint[], HDC hDC, HDC memDC, int md,
	static struct Paint pos[], int id, int piece, int ww, int wh, int pw, int ph);

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
	wndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);	//�޴�����
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
		piece = 3,		//������ ����
		mx, my,			//���콺 ��ǥ����
		md;				//���콺�� ����
	static RECT rt;	//�������� �۾������� ũ�� ����
	BITMAP bit;
	static char c;
	static struct Draw paint[2];
	static BOOL numClick = TRUE,
		movePiece = FALSE, end = FALSE;

	switch (uMsg)
	{
	case WM_CREATE:
		paint[0].hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));	//�ر׸� 1
		paint[1].hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));	//�ر׸� 2

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
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		movePiece = TRUE;
		break;

	case WM_LBUTTONUP:
		md = Mouse_Direct(LOWORD(lParam), HIWORD(lParam), mx, my);
		//printf("%d %d %d %d -%d\n", LOWORD(lParam), HIWORD(lParam), mx, my, md);
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_COMMAND:		//�޴� �Է�
		switch (LOWORD(wParam))
		{
		case ID_paint1:
			id = 0;
			break;
		case ID_full:
			c = 'f';
			break;
		case ID_paint2:
			id = 1;
			break;
		case ID_start:
			c = 's';	//���� �ε�
			break;
		case ID_split3:
			piece = 3;	//3x3����
			break;
		case ID_split4:
			piece = 4;	//4x4����
			break;
		case ID_split5:
			piece = 5;
			break;
		case ID_end:
			end = TRUE;
			break;
		}

		if (LOWORD(wParam) == ID_paint1 || LOWORD(wParam) == ID_paint2 || LOWORD(wParam) == ID_split3 ||
			LOWORD(wParam) == ID_split4 || LOWORD(wParam) == ID_split5)
			numClick = TRUE;

		InvalidateRect(hWnd, NULL, true);
		break;

	case WM_CHAR:		//Ű���� �Է�
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
			end = TRUE;
			break;
		}

		if(wParam == '1' || wParam=='2'||wParam=='3'|| wParam == '4'|| wParam == '5')
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
			Split(hDC, memDC, id, rt, paint, piece, numClick, md, &movePiece, end);
			numClick = FALSE;
			break;
		}

		DeleteDC(memDC);				//memDC����
		DeleteDC(hDC);					//memDC����s
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

void Split(HDC hDC, HDC memDC, int id, RECT rt, struct Draw paint[], int piece, BOOL numClick, int md, BOOL* movePiece, BOOL end)
{
	int ww = rt.right / piece, wh = rt.bottom / piece,	//ȭ���� �ʺ�� ����
		pw = paint[id].bx / piece, ph = paint[id].by / piece;	//�׸��� �ʺ�� ����
	static struct Paint pos[25];
	BOOL mixPuzzle = TRUE;

	//�ѤѤ������� �������� �����ѤѤ�
	if (numClick)	//����Ű�� ���������� ���� ���̵��� ����
		Mix(piece, pos);

	//�ѤѤ������� ��¤ѤѤѤ�
	for (int i = 1; i < piece * piece; ++i)	//0�� ������ ��� ����
		StretchBlt(hDC, ww * pos[i].wx, wh * pos[i].wy, ww, wh,		//��ġ
			memDC, paint[id].bx / piece * pos[i].px, paint[id].by / piece * pos[i].py, pw, ph, SRCCOPY);	//����

	if ((pos[0].wx == 0 &&md == 3) || (pos[0].wx == piece - 1 && md == 4)|| 
		(pos[0].wy == 0 && md == 2)|| (pos[0].wy == piece - 1 && md == 1))
		mixPuzzle = FALSE;


	//�ѤѤѸ��콺�� ������ �̵���Ŵ�ѤѤ�
	if (*movePiece == TRUE&&end==FALSE&&mixPuzzle==TRUE)
	{
		*movePiece = FALSE;
		MovePuzzle(paint, hDC, memDC, md, pos, id, piece, ww, wh, pw, ph);
	}	
}

void Mix(int piece, struct Paint pos[])	//���� ������ ��ġ�� �������� ����f
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

int Mouse_Direct(int mxPos, int myPos, int mx, int my)	//1�̸� �� 2�� �Ʒ� 3�̸� ���� 4�� ������ �� �ƴϸ� 0
{
	int wide = 40;

	if (mx - wide <= mxPos && mxPos <= mx + wide && myPos < my - wide)
		return 1;
	else if (mx - wide <= mxPos && mxPos <= mx + wide && my + wide < myPos)
		return 2;
	else if (my - wide <= myPos && myPos <= my + wide && mx + wide < mxPos)
		return 3;
	else if (my - wide <= myPos && myPos <= my + wide && mxPos < mx - wide)
		return 4;
	else return 0;
}

void MovePuzzle(struct Draw paint[], HDC hDC, HDC memDC, int md,
	static struct Paint pos[], int id, int piece, int ww, int wh, int pw, int ph)
{
	int wx = 0, wy = 0,		//������ ������ ����
	pid = 0;		//�����̷��� ������ ���̵�

		switch (md)
		{
		case 1:	//���� �̵�
			pos[0].wy++;
			wy--;
			break;
		case 2:	//�Ʒ��� �̵�
			pos[0].wy--;
			wy++;
			break;
		case 3:	//���� �̵�
			pos[0].wx--;
			wx++;
			break;
		case 4:	//������ �̵�
			pos[0].wx++;
			wx--;
			break;
		}

		for (int i = 1; i < piece * piece; ++i)
			if (pos[0].wx == pos[i].wx && pos[0].wy == pos[i].wy)
				pid = i;

		if (md != 0 )
		{
			//�ѤѤ�0.01�� �̵��ϸ鼭 ������ ��¤ѤѤ�
			for (float i = 0; i <= 1; i += 0.01)
				StretchBlt(hDC, ww * (pos[pid].wx + i * wx), wh * (pos[pid].wy + i * wy), ww, wh,		//��ġ
					memDC, paint[id].bx / piece * pos[pid].px, paint[id].by / piece * pos[pid].py, pw, ph, SRCCOPY);	//����
			//�ѤѤ������� ����ѤѤ�
			StretchBlt(hDC, ww * pos[0].wx, wh * pos[0].wy, ww, wh,		//��ġ
				memDC, paint[id].bx / piece * pos[0].px, paint[id].by / piece * pos[0].py, pw, ph, WHITENESS);	//����
		}

		pos[pid].wx += wx;
		pos[pid].wy += wy;
}