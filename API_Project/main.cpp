#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include "resource.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	//�ܼ�â ���

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
	TCHAR titleName[10] = L"���ýǽ�";
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
	//--- ������ Ŭ������ ����Ѵ�.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW; // Ŭ���� ��Ÿ��
	wc.lpfnWndProc = WndProc; // ������ ���ν��� ����
	wc.cbClsExtra = 0; // ������Ŭ���� ������ ����
	wc.cbWndExtra = 0; // �������� ������ ����
	wc.hInstance = hInstance; // �ν��Ͻ� �ڵ�
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // ������ �ڵ�
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ����� Ŀ�� �ڵ�
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ������ �귯�� �ڵ�
	wc.lpszMenuName = NULL; // �޴� �̸�
	wc.lpszClassName = L"ParentClass"; // ������ Ŭ���� �̸�
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc); // ������ Ŭ������ ���
	//--- ���ϵ� ������ Ŭ������ ����Ѵ�. 
	wc.hCursor = LoadCursor(NULL, IDC_HELP);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszClassName = L"ChildClass"; // ���ϵ� ������ Ŭ���� �̸�
	wc.lpfnWndProc = ChildProc; // ���ϵ� ������ ���ν��� ����
	RegisterClassEx(&wc); // �ڽ� ������ Ŭ������ ���
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
	case WM_LBUTTONDOWN: // ���콺 ���� ��ư�� ���� ���
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
//		GetClientRect(hWnd, &rt);	//ȭ���� ũ�� ���ϱ�
//		break;
//	case WM_PAINT:
//		hDC = BeginPaint(hWnd, &ps);
//		SelectObject(hDC, (HBRUSH)GetStockObject(GRAY_BRUSH));	//�귯�� ���
//		hPen = (HPEN)CreatePen(PS_SOLID, 2, RGB(172, 172, 172));
//		(HPEN)SelectObject(hDC, hPen);
//		//�ѤѤ� ȭ�� �� ��� ���ڼ��׸��� �ѤѤ�
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

//void Draw2()	//������׷� �׸���
//{
//	HDC hDC;
//}