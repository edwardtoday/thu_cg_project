#include <windows.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glaux.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include "Vector3.h"
#include "Vector4.h"
#include "Triangles.h"
#include "resource.h"
#include <cstdlib>
#include "mine\materials.h"
#include "mine\heap.h"
#include "mine\mesh.h"
using namespace std;

#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "opengl32.lib")

HDC	g_HDC;
bool fullScreen = false;
bool done;

const double step = 0.1;
float ambientLight[] = {0.8, 0.8, 0.8, 1.0};
float diffuseLight[] = {0.9, 0.9, 0.9, 1.0};
float lightPosition[] = {0.0, 0.0, 1.0, 0.0};

float matAmbient[] = {0.5, 0.5, 0.5, 1.0};
float matDiff[] = {1.2, 1.3, 1.4, 1.0};

float lightpos0[] = {0.0, 0.0, 1.0, 0.0};
float lightpos1[] = {0.0, 0.0, 0.0, 1.0};
float materialambient[3] = {0.6, 0.6, 0.6};

Specular::Watch currentEye(2, 0, 0, 0, 1, 0);
double angle;

HWND MainHwnd;
int mouseX, mouseY;
bool IsMoving = false;
Triangles::Objects model;

bool ShowFrame = false;
bool ShowBlue = false;

HINSTANCE hInst;

INT_PTR CALLBACK Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
 	glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);	
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos1);	
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialambient);
}

void DrawCoor() {
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);

		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 10.0);
	glEnd();
	glEnable(GL_LIGHTING);
}

void SetName() {
	Specular::Vector3 eye = currentEye.GetEye();
	Specular::Vector3 top = currentEye.GetTop();
	WCHAR str[1000];
	swprintf(str, L"%g, %g, %g, %g, %g, %g", eye.x, eye.y, eye.z, top.x, top.y, top.z);
	SetWindowText(MainHwnd, str);
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	SetName();
	currentEye.LookAt();
	lightpos0[0] = currentEye.Eye[0];
	lightpos0[1] = currentEye.Eye[1];
	lightpos0[2] = currentEye.Eye[2];
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);			

	glPushMatrix();
		//angle++;
		glRotatef(angle, 1.0, 0.0, 0.0);
		glRotatef(angle, 0.0, 1.0, 0.0);
		glRotatef(angle, 0.0, 0.0, 1.0);
		model.Show();
	glPopMatrix();

	glFlush();
	SwapBuffers(g_HDC);
}

void SetupPixelFormat(HDC hDC) {
	int nPixelFormat;
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0};
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	static HGLRC hRC;
	static HDC hDC;
	int width, height;
	int tempX, tempY;

	switch (message) {
		case WM_COMMAND:
			switch (wparam) {
				case ID_HELP_KEY:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, Dialog);
					break;
				case ID_WHAT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, Dialog);
					break;
				case ID_LINE:
					ShowFrame = true;
					break;
				case ID_FACE: 
					ShowFrame = false;
					ShowBlue = false;
					break;
				case ID_LINE_FACE:
					ShowFrame = false;
					ShowBlue = true;
					break;
			}
			break;
		case WM_CREATE:
			hDC = GetDC(hwnd);
			g_HDC = hDC;
			SetupPixelFormat(hDC);

			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);
			return 0;
			break;
		case WM_LBUTTONDOWN:
			IsMoving = true;
			mouseX = LOWORD(lparam);
			mouseY = HIWORD(lparam);
			break;
		case WM_MOUSEMOVE:
			if (IsMoving) {
				tempX = LOWORD(lparam);
				tempY = HIWORD(lparam);
				currentEye.MoveLeft((tempX - mouseX) / 5);
				currentEye.MoveUp((tempY - mouseY) / 5);
				mouseX = tempX;
				mouseY = tempY;
			}
			break;
		case WM_LBUTTONUP:
			IsMoving = false;
			break;
		case WM_KEYDOWN:
			switch (wparam) {
				case VK_SPACE:
					done = true;
					break;
				case VK_UP:
					currentEye.MoveUp(1.0);
					break;
				case VK_DOWN:
					currentEye.MoveDown(1.0);
					break;
				case VK_LEFT:
					currentEye.MoveLeft(1.0);
					break;
				case VK_RIGHT:
					currentEye.MoveRight(1.0);
					break;
				case 'W':
					currentEye.MoveTowards(step);
					break;
				case 'S':
					currentEye.MoveTowards(-step);
					break;
				case 'D':
					model.DeleteVertex();
					break;
				case 'M':
					ShowFrame = !ShowFrame;
					break;
				case 'N':
					ShowBlue = !ShowBlue;
					break;
				default:
					break;
			}
			break;
		case WM_CLOSE:
			wglMakeCurrent(hDC, NULL);
			wglDeleteContext(hRC);
			PostQuitMessage(0);
			return 0;
			break;
		case WM_SIZE:
			height = HIWORD(lparam);
			width = LOWORD(lparam);
			if (height == 0) height = 1;

			glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			gluPerspective(54.0, (GLfloat)width / (GLfloat)height, 0.01, 1000.0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			return 0;
			break;
		default:
			break;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}

int WINAPI __WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	WNDCLASS	windowClass;
	HWND		hwnd;
	MSG			msg;

	DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT		windowRect;

	int width = 800;
	int height = 600;
	int bits = 32;

	windowRect.left = (long)0;
	windowRect.right = (long)width;
	windowRect.top = (long)0;
	windowRect.bottom = (long)height;

	windowClass.style			= sizeof(WNDCLASS);
	windowClass.lpfnWndProc		= WndProc;
	windowClass.cbClsExtra		= 0;
	windowClass.cbWndExtra		= 0;
	windowClass.hInstance		= hInstance;
	windowClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground	= NULL;
	windowClass.lpszMenuName	= (LPCWSTR)IDR_MENU1;
	windowClass.lpszClassName	= L"MyClass";

	if (!RegisterClass(&windowClass)) return 0;

	if (fullScreen) {
		DEVMODE dmScreenSetting;
		memset(&dmScreenSetting, 0, sizeof(dmScreenSetting));
		dmScreenSetting.dmSize = sizeof(dmScreenSetting);
		dmScreenSetting.dmPelsWidth = width;
		dmScreenSetting.dmPelsHeight = height;
		dmScreenSetting.dmBitsPerPel = bits;
		dmScreenSetting.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;
		
		if (ChangeDisplaySettings(&dmScreenSetting, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			MessageBox(NULL, L"Display mode failed", NULL, MB_OK);
			fullScreen = false;
		}
	}

	if (fullScreen) {
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(true);
	}
	else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	hwnd = CreateWindow(
		L"MyClass",
		L"OpenGL Name",									// Change Title Name
		dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		500, 200,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL, NULL,
		hInstance, NULL);

	if (!hwnd) return 0;
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	done = false;
	MainHwnd = hwnd;
	Initialize();
	while (!done) {
		PeekMessage(&msg, hwnd, NULL, NULL, PM_REMOVE);
		if (msg.message == WM_QUIT) {
			done = true;
		}
		else {
			Render();
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	if (fullScreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}
	return msg.wParam;
}

INT_PTR CALLBACK Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void OutputUsage() {
	cout << "输入格式：" << endl;
	cout << "MeshSimp 输入.obj -w"  << endl;
	cout << "\t\t显示相应的obj文件"  << endl;
	cout << "MeshSimp 输入.obj 输出.obj  简化比（一个小数，例如0.3）" << endl;
	cout << "\t\t简化模型并显示" << endl;
	cout << "MeshSimp 输入.obj 输出.obj  简化比（一个小数，例如0.3）-m" << endl;
	cout << "\t\t简化模型(自己实现算法)并显示" << endl;
}

int main(int argc, char *argv[]) {
	int temp_c;
	char input_name[256];
	char output_name[256];
	float c;
	if (argc == 1) {
		OutputUsage();
	}
	else if (argc == 3) {
		if ((temp_c = strcmp(argv[2], "-w")) != 0)
			OutputUsage();
		else {
			strcpy(input_name, argv[1]);
			model.InitializeAll(input_name);
			hInst = GetModuleHandle(NULL);
			__WinMain(hInst, NULL, NULL, SW_SHOW);
		}
	}
	else if (argc == 4) {
		strcpy(input_name, argv[1]);
		strcpy(output_name, argv[2]);
		sscanf(argv[3], "%f", &c);
		cout << argv[3] << endl;
		cout << c << endl;

		model.InitializeAll(input_name);

		int opt_faces = model.sizeofvalidfaces * c;
		while (model.sizeofvalidfaces > opt_faces) {
			model.DeleteVertex();
		}
		model.Output(output_name);
		model.InitializeAll(output_name);
		hInst = GetModuleHandle(NULL);
		__WinMain(hInst, NULL, NULL, SW_SHOW);
	}
	else if (argc == 5) {
		if ((temp_c = strcmp(argv[4], "-m")) != 0)
			OutputUsage();
		else{
			mesh object(argv[1]);

			object.init();
			double rate = atof(argv[3]);
			int total=object.vSize;
			for(int i=0;i<total*(1-rate);++i)
			{
				object.Delete();
				cout<<(double)i*100/total*(1-rate)<<"\r";
			}
			strcpy(output_name, argv[2]);
			object.storeObj(output_name);
			model.InitializeAll(output_name);
			hInst = GetModuleHandle(NULL);
			__WinMain(hInst, NULL, NULL, SW_SHOW);
		}
	}
	else {
		OutputUsage();
	}
	return 0;
}