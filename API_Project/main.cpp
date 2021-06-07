#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include "resource.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	//콘솔창 띄움

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wndClass;
	TCHAR className[11] = L"Class Name";
	TCHAR titleName[10] = L"윈플실습";
	g_hInst = hInstance;

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
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hWnd;
	MSG msg;
	//--- 윈도우 클래스를 등록한다.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW; // 클래스 스타일
	wc.lpfnWndProc = WndProc; // 윈도우 프로시저 지정
	wc.cbClsExtra = 0; // 윈도우클래스 데이터 영역
	wc.cbWndExtra = 0; // 윈도우의 데이터 영역
	wc.hInstance = hInstance; // 인스턴스 핸들
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 아이콘 핸들
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 사용할 커서 핸들
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 바탕색 브러쉬 핸들
	wc.lpszMenuName = NULL; // 메뉴 이름
	wc.lpszClassName = L"ParentClass"; // 윈도우 클래스 이름
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc); // 윈도우 클래스를 등록
	//--- 차일드 윈도우 클래스를 등록한다. 
	wc.hCursor = LoadCursor(NULL, IDC_HELP);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszClassName = L"ChildClass"; // 차일드 윈도우 클래스 이름
	wc.lpfnWndProc = ChildProc; // 차일드 윈도우 프로시저 지정
	RegisterClassEx(&wc); // 자식 윈도우 클래스를 등록
	hWnd = CreateWindow(L"ParentClass", NULL, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hInstance, NULL);
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
	HWND child_hWnd;
	switch (uMsg)
	{
	case WM_CREATE:
		child_hWnd = CreateWindow(L"ChildClass", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_THICKFRAME, 
			10, 10, 200, 500, hWnd, NULL, g_hInst, NULL);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_LBUTTONDOWN: // 마우스 좌측 버튼을 누른 경우
		MessageBox(hWnd, L"Left Mouse Button", L"Mouse Test", MB_OK);
			break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	HDC hDC;
//	static RECT rt;
//	PAINTSTRUCT ps;
//	HPEN hPen;
//
//	switch (uMsg)
//	{
//	case WM_CREATE:	
//		GetClientRect(hWnd, &rt);	//화면의 크기 구하기
//		break;
//	case WM_PAINT:
//		hDC = BeginPaint(hWnd, &ps);
//		SelectObject(hDC, (HBRUSH)GetStockObject(GRAY_BRUSH));	//브러쉬 등록
//		hPen = (HPEN)CreatePen(PS_SOLID, 2, RGB(172, 172, 172));
//		(HPEN)SelectObject(hDC, hPen);
//		//ㅡㅡㅡ 화면 정 가운데 십자선그리기 ㅡㅡㅡ
//		Ellipse(hDC,rt.left, rt.top, rt.right, rt.bottom);
//		MoveToEx(hDC, rt.right / 2, 0, NULL);
//		LineTo(hDC, rt.right / 2, rt.bottom);
//		EndPaint(hWnd, &ps);
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	}
//	return DefWindowProc(hWnd, uMsg, wParam, lParam);
//}

//void Draw2()	//지그재그로 그리기
//{
//	HDC hDC;
//}