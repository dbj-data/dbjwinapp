// DBJ added
#pragma comment(user, "dbj@dbj.org compiled on " __DATE__ " at " __TIME__)

// minimum build env is WIN10
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "ComDlg32")

#define NOMINMAX
#define STRICT 1
// #define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifndef min
#define min(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

#define DBJ_APP_NAME "dbj_font_one"

// on failure return R
#define DBJ_CHK(X, R)                                                                                                                                 \
	do                                                                                                                                                \
	{                                                                                                                                                 \
		if (!(X))                                                                                                                                     \
		{                                                                                                                                             \
			MessageBoxA(NULL, #X "\n\n" __FILE__ "\n\n" _CRT_STRINGIZE(__LINE__), DBJ_APP_NAME " -- Expression Failed!", MB_OK | MB_ICONEXCLAMATION); \
			return R;                                                                                                                                 \
		}                                                                                                                                             \
	} while (0)

// eof DBJ added

#include "resource.h"

static struct
{
	const char *const class_name;
	HFONT font_;
	BOOL opaque_;
	COLORREF text_colour_;
	COLORREF background_colour_;
	COLORREF custom_colour_[16];
} GLOBAL = {DBJ_APP_NAME, NULL, TRUE, RGB(0, 0, 0), RGB(255, 255, 255), {0}};

#if 0
// https://stackoverflow.com/a/6724559/10870835
BOOL CALLBACK set_child_font_(HWND hwndChild, LPARAM lParam)
{
	HFONT hFont = (HFONT)lParam;
	SendMessage(hwndChild, WM_SETFONT, (WPARAM)hFont, TRUE);
	return TRUE ;
}

void DoSelectFont(HWND hwnd)
{
	CHOOSEFONT cf = {sizeof(CHOOSEFONT)};
	LOGFONT lf;
	GetObject(GLOBAL.font_, sizeof(LOGFONT), &lf);

	cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
	cf.hwndOwner = hwnd;
	cf.lpLogFont = &lf;
	cf.rgbColors = GLOBAL.text_colour_;

	ChooseFont(&cf);
	HFONT hf;
	if ((hf = CreateFontIndirect(&lf)))
	{
		GLOBAL.font_ = hf;
	}
	GLOBAL.text_colour_ = cf.rgbColors;
}
#endif // 0

static void show_this_window_client_size_(HDC hdc, RECT *rect_ptr_, HFONT hf)
{
	HFONT hfOld = (HFONT)SelectObject(hdc, hf);

	SetBkColor(hdc, GLOBAL.background_colour_);
	SetTextColor(hdc, GLOBAL.text_colour_);

	if (GLOBAL.opaque_)
		SetBkMode(hdc, OPAQUE);
	else
		SetBkMode(hdc, TRANSPARENT);

	DrawText(hdc, "These are the dimensions of your client area:", -1, rect_ptr_, DT_WORDBREAK);

	char text_[0xFF] = {0};
	wsprintf(text_, "{%4d, %4d, %4d, %4d }", rect_ptr_->left, rect_ptr_->top, rect_ptr_->right, rect_ptr_->bottom);
	DrawText(hdc, text_, -1, rect_ptr_, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	SelectObject(hdc, hfOld);
}

/*
WM_KEYDOWN
VK_OEM_PLUS -- For any country/region, the '+' key
VK_CONTROL  -- CTRL key
*/
//

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_ACCELERATOR40001:
		(void)MessageBoxA(NULL, "CONTROL+PLUS\n\n" __FILE__ "\n\n" _CRT_STRINGIZE(__LINE__), DBJ_APP_NAME " -- Shortcut Caught!", MB_OK | MB_ICONEXCLAMATION);
        break;
        }
	break;
	case WM_CREATE:
		GLOBAL.font_ = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_PAINT:
	{
		RECT rcClient;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rcClient);

		show_this_window_client_size_(hdc, &rcClient, GLOBAL.font_);

		EndPaint(hwnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		DeleteObject(GLOBAL.font_);

		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	// wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = GLOBAL.class_name;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	DBJ_CHK(0 != RegisterClassExA(&wc), EXIT_FAILURE);

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		GLOBAL.class_name,
		DBJ_APP_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
		NULL, NULL, hInstance, NULL);

	DBJ_CHK(hwnd != NULL, EXIT_FAILURE);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	HACCEL dbj_accel_table_ = NULL ;
		
	DBJ_CHK( (
	dbj_accel_table_ =	LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1))
	), EXIT_FAILURE );

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		if (!TranslateAccelerator(hwnd, dbj_accel_table_, &Msg))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
	return Msg.wParam;
}
