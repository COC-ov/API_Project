#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"

//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	//콘솔창 띄움

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Split(HDC hDC, HDC memDC, int id, RECT rt, struct Draw paint[],
	int piece, BOOL numClick, int md, BOOL* mousePiece, BOOL);	//퍼즐 조각을 띄움
void Mix(int piece, struct Paint pos[]);	//퍼즐 조각의 위치를 랜덤으로 섞음
int Mouse_Direct(int, int, int, int);	//마우스의 이동방향을 반환
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
//밑그림의 정보를 담을 구조체

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wndClass;
	TCHAR className[11] = L"Class Name";
	TCHAR titleName[10] = L"윈플실습 5-2";
	hInst = hInstance;


	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);	//메뉴정의
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
	static int id,	//비트맵의 정보 저장
		piece = 3,		//퍼즐의 조각
		mx, my,			//마우스 좌표저장
		md;				//마우스의 방향
	static RECT rt;	//윈도우의 작업영역의 크기 저장
	BITMAP bit;
	static char c;
	static struct Draw paint[2];
	static BOOL numClick = TRUE,
		movePiece = FALSE, end = FALSE;

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

	case WM_COMMAND:		//메뉴 입력
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
			c = 's';	//퍼즐 로드
			break;
		case ID_split3:
			piece = 3;	//3x3퍼즐
			break;
		case ID_split4:
			piece = 4;	//4x4퍼즐
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

	case WM_CHAR:		//키보드 입력
		switch (wParam)
		{
		case 'F':
		case 'f':		//전체 그림 보여주기
			c = 'f';
			break;
		case '1':		//밑그림 1
			id = 0;
			break;		//밑그림 2
		case '2':	
			id = 1;
			break;
		case 's':
		case 'S':
			c = 's';	//퍼즐 로드
			break;
		case '3':
			piece = 3;	//3x3퍼즐
			break;
		case '4':
			piece = 4;	//4x4퍼즐
			break;
		case '5':		//5x5퍼즐
			piece = 5;
			break;
		case 'q':		//종료
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
		memDC = CreateCompatibleDC(hDC);	//메모리디시인memedc생성

		(HBITMAP)SelectObject(memDC, paint[id].hBitmap);	//비트맵 선택
		//BitBlt(hDC,0, 0, paint[id].bx, paint[id].by, memDC, 0, 0, SRCCOPY);	//그냥 출력

		switch (c)
		{
		case 'f':
			StretchBlt(hDC, rt.left, rt.top, rt.right, rt.bottom, memDC, 0, 0, paint[id].bx, paint[id].by, SRCCOPY); //화면에 맞춰서 출력
			break;
		case 's':
			Split(hDC, memDC, id, rt, paint, piece, numClick, md, &movePiece, end);
			numClick = FALSE;
			break;
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

void Split(HDC hDC, HDC memDC, int id, RECT rt, struct Draw paint[], int piece, BOOL numClick, int md, BOOL* movePiece, BOOL end)
{
	int ww = rt.right / piece, wh = rt.bottom / piece,	//화면의 너비와 높이
		pw = paint[id].bx / piece, ph = paint[id].by / piece;	//그림의 너비와 높이
	static struct Paint pos[25];
	BOOL mixPuzzle = TRUE;

	//ㅡㅡㅡ퍼즐을 랜덤으로 섞음ㅡㅡㅡ
	if (numClick)	//숫자키를 눌렀을때만 값이 섞이도록 조절
		Mix(piece, pos);

	//ㅡㅡㅡ퍼즐을 출력ㅡㅡㅡㅡ
	for (int i = 1; i < piece * piece; ++i)	//0번 조각은 출력 안함
		StretchBlt(hDC, ww * pos[i].wx, wh * pos[i].wy, ww, wh,		//위치
			memDC, paint[id].bx / piece * pos[i].px, paint[id].by / piece * pos[i].py, pw, ph, SRCCOPY);	//조각

	if ((pos[0].wx == 0 &&md == 3) || (pos[0].wx == piece - 1 && md == 4)|| 
		(pos[0].wy == 0 && md == 2)|| (pos[0].wy == piece - 1 && md == 1))
		mixPuzzle = FALSE;


	//ㅡㅡㅡ마우스로 퍼즐을 이동시킴ㅡㅡㅡ
	if (*movePiece == TRUE&&end==FALSE&&mixPuzzle==TRUE)
	{
		*movePiece = FALSE;
		MovePuzzle(paint, hDC, memDC, md, pos, id, piece, ww, wh, pw, ph);
	}	
}

void Mix(int piece, struct Paint pos[])	//퍼즐 조각의 위치를 랜덤으로 섞음f
{
	int rn, temp, num, k = 0;

	for (int i = 0; i < piece; ++i)	//퍼즐의 위치와 조각의 종류를 그림 순서대로 구조체에 담음
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

	for (int i = 0; i < piece*piece - 1; ++i)	//퍼즐의 위치를 섞는다
	{
		srand(time(NULL));
		rn = rand() % (piece*piece - i) + i; // i 부터 num-1 사이에 임의의 정수 생성 

		temp = pos[i].wx; 
		pos[i].wx = pos[rn].wx; 
		pos[rn].wx = temp; 

		temp = pos[i].wy;
		pos[i].wy = pos[rn].wy;
		pos[rn].wy = temp;
	}

}

int Mouse_Direct(int mxPos, int myPos, int mx, int my)	//1이면 위 2면 아래 3이면 왼쪽 4면 오른쪽 다 아니면 0
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
	int wx = 0, wy = 0,		//퍼즐이 움직인 정보
	pid = 0;		//움직이려는 퍼즐의 아이디

		switch (md)
		{
		case 1:	//위로 이동
			pos[0].wy++;
			wy--;
			break;
		case 2:	//아래로 이동
			pos[0].wy--;
			wy++;
			break;
		case 3:	//왼쪽 이동
			pos[0].wx--;
			wx++;
			break;
		case 4:	//오른쪽 이동
			pos[0].wx++;
			wx--;
			break;
		}

		for (int i = 1; i < piece * piece; ++i)
			if (pos[0].wx == pos[i].wx && pos[0].wy == pos[i].wy)
				pid = i;

		if (md != 0 )
		{
			//ㅡㅡㅡ0.01씩 이동하면서 퍼즐을 출력ㅡㅡㅡ
			for (float i = 0; i <= 1; i += 0.01)
				StretchBlt(hDC, ww * (pos[pid].wx + i * wx), wh * (pos[pid].wy + i * wy), ww, wh,		//위치
					memDC, paint[id].bx / piece * pos[pid].px, paint[id].by / piece * pos[pid].py, pw, ph, SRCCOPY);	//조각
			//ㅡㅡㅡ흔적을 지움ㅡㅡㅡ
			StretchBlt(hDC, ww * pos[0].wx, wh * pos[0].wy, ww, wh,		//위치
				memDC, paint[id].bx / piece * pos[0].px, paint[id].by / piece * pos[0].py, pw, ph, WHITENESS);	//조각
		}

		pos[pid].wx += wx;
		pos[pid].wy += wy;
}