#include <Mamuna.h>
#include <stdio.h>
#include <time.h>

typedef struct Mamuna_Window {
	size_t index;
} Mamuna_Window;

Mamuna_Hashmap_Int* windows = NULL;

size_t openWindows = 0;
size_t createdWindows = 0;

bool globKeys[MAMUNA_KEY_LAST] = {0};
int gloState = 0;

#if defined(_WIN32)
#include <windows.h>
#include <windowsx.h>

#define WGL_CONTEXT_DEBUG_BIT_ARB         0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB       0x2093
#define WGL_CONTEXT_FLAGS_ARB             0x2094
#define ERROR_INVALID_VERSION_ARB         0x2095

#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define WGL_NUMBER_PIXEL_FORMATS_ARB      0x2000
#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_DRAW_TO_BITMAP_ARB            0x2002
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_NEED_PALETTE_ARB              0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB       0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB        0x2006
#define WGL_SWAP_METHOD_ARB               0x2007
#define WGL_NUMBER_OVERLAYS_ARB           0x2008
#define WGL_NUMBER_UNDERLAYS_ARB          0x2009
#define WGL_TRANSPARENT_ARB               0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB     0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB   0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB    0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB   0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB   0x203B
#define WGL_SHARE_DEPTH_ARB               0x200C
#define WGL_SHARE_STENCIL_ARB             0x200D
#define WGL_SHARE_ACCUM_ARB               0x200E
#define WGL_SUPPORT_GDI_ARB               0x200F
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_STEREO_ARB                    0x2012
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_RED_BITS_ARB                  0x2015
#define WGL_RED_SHIFT_ARB                 0x2016
#define WGL_GREEN_BITS_ARB                0x2017
#define WGL_GREEN_SHIFT_ARB               0x2018
#define WGL_BLUE_BITS_ARB                 0x2019
#define WGL_BLUE_SHIFT_ARB                0x201A
#define WGL_ALPHA_BITS_ARB                0x201B
#define WGL_ALPHA_SHIFT_ARB               0x201C
#define WGL_ACCUM_BITS_ARB                0x201D
#define WGL_ACCUM_RED_BITS_ARB            0x201E
#define WGL_ACCUM_GREEN_BITS_ARB          0x201F
#define WGL_ACCUM_BLUE_BITS_ARB           0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB          0x2021
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_AUX_BUFFERS_ARB               0x2024
#define WGL_NO_ACCELERATION_ARB           0x2025
#define WGL_GENERIC_ACCELERATION_ARB      0x2026
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_SWAP_EXCHANGE_ARB             0x2028
#define WGL_SWAP_COPY_ARB                 0x2029
#define WGL_SWAP_UNDEFINED_ARB            0x202A
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_TYPE_COLORINDEX_ARB           0x202C

#define WGL_SAMPLE_BUFFERS_ARB            0x2041
#define WGL_SAMPLES_ARB                   0x2042

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB  0x20A9

#define WGL_TRANSPARENT_ARB               0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB     0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB   0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB    0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB   0x203A


typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTPROC) (HDC hDc);
typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef BOOL (WINAPI * PFNWGLDELETECONTEXTPROC) (HGLRC oldContext);
typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef BOOL (WINAPI * PFNWGLMAKECURRENTPROC) (HDC hDc, HGLRC newContext);



typedef PROC (WINAPI * PFNWGLGETPROCADDRESSPROC) (LPCSTR lpszProc);

typedef struct Mamuna_Context {
	HGLRC context;
} Mamuna_Context;

typedef struct WindowNode {
	HWND win;
	bool shouldClose;
	bool isActive;
} WindowNode;

PFNWGLCREATECONTEXTPROC p_wglCreateContext = NULL;
PFNWGLDELETECONTEXTPROC p_wglDeleteContext = NULL;
PFNWGLCREATECONTEXTATTRIBSARBPROC p_wglCreateContextAttribsARB = NULL;
PFNWGLGETPROCADDRESSPROC p_wglGetProcAddress = NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC p_wglChoosePixelFormatARB = NULL;
PFNWGLMAKECURRENTPROC p_wglMakeCurrent = NULL;

bool init = false;

WNDCLASSEXA class = {0};
ATOM Aclass = 0;
RECT sizeHints = {0};
HMODULE dynWGL = NULL;


int winCreationHints[] = {
	WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
	WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
	WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
	WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
	WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
	WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,

	WGL_COLOR_BITS_ARB, 24,

	WGL_STEREO_ARB, GL_FALSE,

	WGL_RED_BITS_ARB, 8,
	WGL_GREEN_BITS_ARB, 8,
	WGL_BLUE_BITS_ARB, 8,
	WGL_ALPHA_BITS_ARB, 0,
	WGL_DEPTH_BITS_ARB, 24,
	WGL_STENCIL_BITS_ARB, 8,
	WGL_SAMPLE_BUFFERS_ARB, 0,
	WGL_SAMPLES_ARB, 0,
	WGL_TRANSPARENT_ARB, GL_FALSE,
	WGL_TRANSPARENT_RED_VALUE_ARB, 0,
	WGL_TRANSPARENT_GREEN_VALUE_ARB, 0,
	WGL_TRANSPARENT_BLUE_VALUE_ARB, 0,
	WGL_TRANSPARENT_ALPHA_VALUE_ARB, 0,
	0,
};

PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | 
		PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	24,
	8, 0, 8, 0, 8, 0, 0, 0,
	0, 0, 0, 0, 0,
	24, 8, 0,
	PFD_MAIN_PLANE,
	0, 0, 0, 0,
};


int contextCreationHints[] = {
	WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
	WGL_CONTEXT_MINOR_VERSION_ARB, 6,
	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
	0,
};

LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
	switch(msg){
		case WM_CLOSE:
			{
				WindowNode* node = Mamuna_HashmapIntGetElement(windows, (size_t)hwnd);
				if(!node){
					return 0;
				}
				node->shouldClose = true;
				return 0;
			}
		case WM_ACTIVATE:
			{
				WindowNode* node = Mamuna_HashmapIntGetElement(windows, (size_t)hwnd);
				if(!node){
					return 0;
				}

				node->isActive = !LOWORD(wparam) ? true : false;

				return 0;
			}
		default:
			return DefWindowProcA(hwnd, msg, wparam, lparam);
	}

	return 0;
}

void Mamuna_SetCreationHint(MAMUNA_ENUM hint, int value){
	int index = hint - MAMUNA_WIN_STEREO_HINT + 15;
	if(index < 0 && hint <= MAMUNA_WIN_TRANSPARENT_ALPHA_VALUE_HINT) return;
	if((index & 1) == 0){
		++index;
	}
	int cIndex = hint - MAMUNA_CONTEXT_GL_MAJOR_VERSION_HINT + 1;
	if(cIndex < 0 && hint >= MAMUNA_CONTEXT_GL_MAJOR_VERSION_HINT &&
			hint <= MAMUNA_CONTEXT_GL_FLAGS_HINT) return;
	if((cIndex & 1) == 0){
		++cIndex;
	}
	switch(hint){
		case MAMUNA_WIN_STEREO_HINT:
		case MAMUNA_WIN_SAMPLE_BUFFERS_HINT:
		case MAMUNA_WIN_TRANSPARENT_TYPE_HINT:
			winCreationHints[index] = value ? GL_TRUE : GL_FALSE;
			break;
		case MAMUNA_WIN_RED_SIZE_HINT:
		case MAMUNA_WIN_GREEN_SIZE_HINT:
		case MAMUNA_WIN_BLUE_SIZE_HINT:
			winCreationHints[index] = value;
			winCreationHints[13] = winCreationHints[17] +
				winCreationHints[19] + winCreationHints[22];
			break;
		case MAMUNA_WIN_ALPHA_SIZE_HINT:
		case MAMUNA_WIN_DEPTH_SIZE_HINT:
		case MAMUNA_WIN_STENCIL_SIZE_HINT:
		case MAMUNA_WIN_SAMPLES_HINT:
		case MAMUNA_WIN_TRANSPARENT_RED_VALUE_HINT:
		case MAMUNA_WIN_TRANSPARENT_GREEN_VALUE_HINT:
		case MAMUNA_WIN_TRANSPARENT_BLUE_VALUE_HINT:
		case MAMUNA_WIN_TRANSPARENT_ALPHA_VALUE_HINT:
			winCreationHints[index] = value;
			break;

		case MAMUNA_WIN_POS_X_SIZE_HINT:
			sizeHints.left = value;
			break;
		case MAMUNA_WIN_POS_Y_SIZE_HINT:
			sizeHints.top = value;
			break;
		case MAMUNA_WIN_WIDTH_SIZE_HINT:
			sizeHints.right = value;
			break;
		case MAMUNA_WIN_HEIGHT_SIZE_HINT:
			sizeHints.bottom = value;
			break;
		case MAMUNA_CONTEXT_GL_MAJOR_VERSION_HINT:
		case MAMUNA_CONTEXT_GL_MINOR_VERSION_HINT:
		case MAMUNA_CONTEXT_GL_PROFILE_MASK_HINT:
		case MAMUNA_CONTEXT_GL_FLAGS_HINT:
			contextCreationHints[cIndex] = value;
			break;
	
	}	
}


bool Mamuna_Init(){
	dynWGL = LoadLibraryW(L"opengl32.dll");
	if(!dynWGL){
		printf("NotFound opengl32.dll\n");
		return false;
	}

	class.cbSize = sizeof(WNDCLASSEX);
	class.style = CS_OWNDC; //CS_HREDRAW | CS_VREDRAW;
	class.lpfnWndProc = WndProc;
	class.hInstance = GetModuleHandle(NULL);
	class.hIcon = NULL;
	class.hCursor = NULL;
	class.hbrBackground = CreateSolidBrush(RGB(0,0,0));
	class.lpszMenuName = NULL;
	class.lpszClassName = "Mamuna";
	class.hIconSm = NULL;

	p_wglGetProcAddress = (PFNWGLGETPROCADDRESSPROC)GetProcAddress(dynWGL, "wglGetProcAddress");
	if(!p_wglGetProcAddress){
		printf("wglGetProcAddress\n");
		return false;
	}

	p_wglCreateContext = (PFNWGLCREATECONTEXTPROC)GetProcAddress(dynWGL, "wglCreateContext");
	if(!p_wglCreateContext){
		printf("wglCreateContext");
		return false;
	}

	p_wglDeleteContext = (PFNWGLDELETECONTEXTPROC)GetProcAddress(dynWGL, "wglDeleteContext");
	if(!p_wglDeleteContext){
		printf("wglDeleteContext\n");
		return false;
	}

	p_wglMakeCurrent = (PFNWGLMAKECURRENTPROC)GetProcAddress(dynWGL, "wglMakeCurrent");
	if(!p_wglMakeCurrent){
		printf("wglMakeCurrent\n");
		return false;
	}


	Aclass = RegisterClassExA(&class);
	if(!Aclass){
		printf("Failed Register window class\n");
		return false;
	}

	HWND tmp = CreateWindowExA(0, "Mamuna", "tmp", 0, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
	if(!tmp){
		init = true;
		Mamuna_Quit();
		return false;

	}
	HDC hdc = GetDC(tmp);

	int iPixel = ChoosePixelFormat(hdc, &pfd);
	if(!SetPixelFormat(hdc, iPixel, &pfd)){
		init = true;
		DestroyWindow(tmp);
		Mamuna_Quit();
		return false;

	}

	HGLRC dummy = p_wglCreateContext(hdc);
	if(!p_wglMakeCurrent(hdc, dummy)){
		init = true;
		DestroyWindow(tmp);
		Mamuna_Quit();
		return false;
	}

	p_wglCreateContextAttribsARB = 
		(PFNWGLCREATECONTEXTATTRIBSARBPROC)p_wglGetProcAddress("wglCreateContextAttribsARB");
	if(!p_wglCreateContextAttribsARB){
		init = true;
		DestroyWindow(tmp);
		Mamuna_Quit();
		return false;
	}

	p_wglChoosePixelFormatARB =
		(PFNWGLCHOOSEPIXELFORMATARBPROC)p_wglGetProcAddress("wglChoosePixelFormatARB");
	if(!p_wglChoosePixelFormatARB){
		init = true;
		DestroyWindow(tmp);

		Mamuna_Quit();

		return false;
	}

	windows = Mamuna_CreateHashmapInt(sizeof(WindowNode), 10); //key to HWND uintptr_t

	DestroyWindow(tmp);
	init = true;
	return true;
}

void Mamuna_Quit(){
	if(!init) return;
	init = false;

	

	UnregisterClassA("Mamuna", GetModuleHandle(NULL));

	if(windows){
		size_t count;
		WindowNode* wins = Mamuna_DestroyHashmapInt(windows, &count);
		for(size_t i = 0; i < count; ++i){
			DestroyWindow(wins[i].win);
		}
		free(wins);
	}

	FreeLibrary(dynWGL);

}

Mamuna_Window* Mamuna_CreateWindow(const char* title, bool fullscreen){
	if(!init || !title) return NULL;

	DWORD styleEX = WS_EX_APPWINDOW;
	DWORD style = 0;
	if(fullscreen){
		style = WS_POPUP | WS_VISIBLE;
	} else{
		style = WS_CAPTION | WS_VISIBLE;
	}
	
	HWND win = CreateWindowExA(styleEX, "Mamuna", title, style, sizeHints.left, sizeHints.top,
			sizeHints.right, sizeHints.bottom, NULL, NULL, GetModuleHandle(NULL), NULL);
	if(!win) return NULL;

	HDC hdc = GetDC(win);

	int buff[256];
	UINT count;

	if(!p_wglChoosePixelFormatARB(hdc, winCreationHints, NULL, 256, buff, &count) || !count){
		DestroyWindow(win);
		return NULL;
	}

	if(!DescribePixelFormat(hdc, buff[0], sizeof(PIXELFORMATDESCRIPTOR), &pfd)){
		DestroyWindow(win);
		return NULL;
	}

	if(!SetPixelFormat(hdc, buff[0], &pfd)){
		DestroyWindow(win);
		return NULL;
	}

	WindowNode node = {0};
	node.win = win;

	Mamuna_Window* window = calloc(sizeof(Mamuna_Window), 1);
	window->index = (size_t)win;
	Mamuna_HashmapIntAddElement(windows, window->index, &node);
	++openWindows;
	return window;
}

void Mamuna_DestroyWindow(Mamuna_Window* window){
	if(!init || !window) return;

	WindowNode* win = Mamuna_HashmapIntRemoveElement(windows, window->index);
	if(!win) return;

	DestroyWindow(win->win);
	free(win);
	free(window);
	--openWindows;
}

MAMUNA_ENUM Mamuna_MoveWindow(Mamuna_Window* window, int x, int y){
	if(!init || !window) return MAMUNA_ERROR;

	WindowNode* node = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!node) return MAMUNA_IS_NOT_EXIST;

	SetWindowPos(node->win, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

	return MAMUNA_SUCCESS;
}

MAMUNA_ENUM Mamuna_RenameWindow(Mamuna_Window* window, const char* name){
	if(!init || !window || !name) return MAMUNA_ERROR;

	WindowNode* node = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!node){
		return MAMUNA_IS_NOT_EXIST;
	}

	SetWindowTextA(node->win, name);

	return MAMUNA_SUCCESS;
}

void Mamuna_WindowGetSize(Mamuna_Window* window, int* width, int* height){
	if(!init || !window) return;

	WindowNode* node = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!node) return;

	RECT tmp;
	GetWindowRect(node->win, &tmp);

	if(width) *width = tmp.right - tmp.left;
	if(height) *height = tmp.bottom - tmp.top;
}

void Mamuna_PollEvents(){
	if(!init) return;

	MSG msg;
	while(PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Mamuna_WindowSwapBuffers(Mamuna_Window* window){
	if(!init || !window) return;

	WindowNode* node = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!node) return;

	HDC hdc = GetDC(node->win);



	SwapBuffers(hdc);

	glFinish();
	glFlush();
}

bool Mamuna_IsRunning(){
	return openWindows;
}

bool Mamuna_WindowShouldClose(Mamuna_Window* window){
	if(!init || !window) return true;

	WindowNode* node = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!node) return true;

	return node->shouldClose;
}

bool Mamuna_WindowIsActive(Mamuna_Window* window){
	if(!init || !window) return false;

	WindowNode* node = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!node) return false;

	return node->isActive;
}

uint64_t Mamuna_GetPerformanceFrequency(){
	LARGE_INTEGER tmp;
	QueryPerformanceFrequency(&tmp);
	return (uint64_t)tmp.QuadPart;
}

uint64_t Mamuna_GetPerformanceCounter(){
	LARGE_INTEGER tmp;
	QueryPerformanceCounter(&tmp);
	return (uint64_t)tmp.QuadPart;
}

Mamuna_Context* Mamuna_CreateContext(Mamuna_Window* window){
	if(!init || !window) return NULL;
	
	WindowNode* node = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!node) return NULL;

	HDC hdc = GetDC(node->win);

	HGLRC con = p_wglCreateContextAttribsARB(hdc, NULL, contextCreationHints);
	if(!con){
		printf("context\n");
		return NULL;
	}
	
	Mamuna_Context* context = calloc(1, sizeof(Mamuna_Context));
	context->context = con;

	return context;
}
void Mamuna_DestroyContext(Mamuna_Context* context){
	if(!init || !context) return;

	p_wglDeleteContext(context->context);
	free(context);
}

MAMUNA_ENUM Mamuna_MakeCurrent(Mamuna_Window* window, Mamuna_Context* context){
	if(!init || !window || !context) return MAMUNA_ERROR;

	WindowNode* node = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!node) return MAMUNA_IS_NOT_EXIST;
	HDC hdc = GetDC(node->win);
	p_wglMakeCurrent(hdc, context->context);

	return MAMUNA_SUCCESS;
}

GLADapiproc Mamuna_GetProcAddress(const char* name){
	GLADapiproc proc = (GLADapiproc)p_wglGetProcAddress((const GLubyte*)name);
	if(!proc){
		proc = (GLADapiproc)GetProcAddress(dynWGL, name);
	}

	return proc;
}


#elif defined(__linux__)

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <dlfcn.h>

#define GLX_USE_GL		1
#define GLX_BUFFER_SIZE		2
#define GLX_LEVEL		3
#define GLX_RGBA		4
#define GLX_DOUBLEBUFFER	5
#define GLX_STEREO		6
#define GLX_AUX_BUFFERS		7
#define GLX_RED_SIZE		8
#define GLX_GREEN_SIZE		9
#define GLX_BLUE_SIZE		10
#define GLX_ALPHA_SIZE		11
#define GLX_DEPTH_SIZE		12
#define GLX_STENCIL_SIZE	13
#define GLX_ACCUM_RED_SIZE	14
#define GLX_ACCUM_GREEN_SIZE	15
#define GLX_ACCUM_BLUE_SIZE	16
#define GLX_ACCUM_ALPHA_SIZE	17

#define GLX_BAD_SCREEN		1
#define GLX_BAD_ATTRIBUTE	2
#define GLX_NO_EXTENSION	3
#define GLX_BAD_VISUAL		4
#define GLX_BAD_CONTEXT		5
#define GLX_BAD_VALUE       	6
#define GLX_BAD_ENUM		7

#define GLX_VENDOR		1
#define GLX_VERSION		2
#define GLX_EXTENSIONS 		3

#define GLX_CONFIG_CAVEAT		0x20
#define GLX_DONT_CARE			0xFFFFFFFF
#define GLX_X_VISUAL_TYPE		0x22
#define GLX_TRANSPARENT_TYPE		0x23
#define GLX_TRANSPARENT_INDEX_VALUE	0x24
#define GLX_TRANSPARENT_RED_VALUE	0x25
#define GLX_TRANSPARENT_GREEN_VALUE	0x26
#define GLX_TRANSPARENT_BLUE_VALUE	0x27
#define GLX_TRANSPARENT_ALPHA_VALUE	0x28
#define GLX_WINDOW_BIT			0x00000001
#define GLX_PIXMAP_BIT			0x00000002
#define GLX_PBUFFER_BIT			0x00000004
#define GLX_AUX_BUFFERS_BIT		0x00000010
#define GLX_FRONT_LEFT_BUFFER_BIT	0x00000001
#define GLX_FRONT_RIGHT_BUFFER_BIT	0x00000002
#define GLX_BACK_LEFT_BUFFER_BIT	0x00000004
#define GLX_BACK_RIGHT_BUFFER_BIT	0x00000008
#define GLX_DEPTH_BUFFER_BIT		0x00000020
#define GLX_STENCIL_BUFFER_BIT		0x00000040
#define GLX_ACCUM_BUFFER_BIT		0x00000080
#define GLX_NONE			0x8000
#define GLX_SLOW_CONFIG			0x8001
#define GLX_TRUE_COLOR			0x8002
#define GLX_DIRECT_COLOR		0x8003
#define GLX_PSEUDO_COLOR		0x8004
#define GLX_STATIC_COLOR		0x8005
#define GLX_GRAY_SCALE			0x8006
#define GLX_STATIC_GRAY			0x8007
#define GLX_TRANSPARENT_RGB		0x8008
#define GLX_TRANSPARENT_INDEX		0x8009
#define GLX_VISUAL_ID			0x800B
#define GLX_SCREEN			0x800C
#define GLX_NON_CONFORMANT_CONFIG	0x800D
#define GLX_DRAWABLE_TYPE		0x8010
#define GLX_RENDER_TYPE			0x8011
#define GLX_X_RENDERABLE		0x8012
#define GLX_FBCONFIG_ID			0x8013
#define GLX_RGBA_TYPE			0x8014
#define GLX_COLOR_INDEX_TYPE		0x8015
#define GLX_MAX_PBUFFER_WIDTH		0x8016
#define GLX_MAX_PBUFFER_HEIGHT		0x8017
#define GLX_MAX_PBUFFER_PIXELS		0x8018
#define GLX_PRESERVED_CONTENTS		0x801B
#define GLX_LARGEST_PBUFFER		0x801C
#define GLX_WIDTH			0x801D
#define GLX_HEIGHT			0x801E
#define GLX_EVENT_MASK			0x801F
#define GLX_DAMAGED			0x8020
#define GLX_SAVED			0x8021
#define GLX_WINDOW			0x8022
#define GLX_PBUFFER			0x8023
#define GLX_PBUFFER_HEIGHT              0x8040
#define GLX_PBUFFER_WIDTH               0x8041
#define GLX_RGBA_BIT			0x00000001
#define GLX_COLOR_INDEX_BIT		0x00000002
#define GLX_PBUFFER_CLOBBER_MASK	0x08000000

#define GLX_SAMPLE_BUFFERS              0x186a0 /*100000*/
#define GLX_SAMPLES                     0x186a1 /*100001*/

#define GLX_CONTEXT_DEBUG_BIT_ARB         0x00000001
#define GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define GLX_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB     0x2092
#define GLX_CONTEXT_FLAGS_ARB             0x2094

#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define GLX_CONTEXT_PROFILE_MASK_ARB      0x9126

typedef struct __GLXcontextRec *GLXContext;
typedef XID GLXPixmap;
typedef XID GLXDrawable;
typedef struct __GLXFBConfigRec *GLXFBConfig;
typedef XID GLXFBConfigID;
typedef XID GLXContextID;
typedef XID GLXWindow;
typedef XID GLXPbuffer;
typedef void (*__GLXextFuncPtr)(void);

typedef GLXFBConfig * (* PFNGLXGETFBCONFIGSPROC) (Display *dpy, int screen, int *nelements);
typedef GLXFBConfig * (* PFNGLXCHOOSEFBCONFIGPROC) (Display *dpy, int screen, const int *attrib_list, int *nelements);
typedef int (* PFNGLXGETFBCONFIGATTRIBPROC) (Display *dpy, GLXFBConfig config, int attribute, int *value);
typedef XVisualInfo * (* PFNGLXGETVISUALFROMFBCONFIGPROC) (Display *dpy, GLXFBConfig config);
typedef GLXWindow (* PFNGLXCREATEWINDOWPROC) (Display *dpy, GLXFBConfig config, Window win, const int *attrib_list);
typedef void (* PFNGLXDESTROYWINDOWPROC) (Display *dpy, GLXWindow win);
typedef GLXPixmap (* PFNGLXCREATEPIXMAPPROC) (Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attrib_list);
typedef void (* PFNGLXDESTROYPIXMAPPROC) (Display *dpy, GLXPixmap pixmap);
typedef GLXPbuffer (* PFNGLXCREATEPBUFFERPROC) (Display *dpy, GLXFBConfig config, const int *attrib_list);
typedef void (* PFNGLXDESTROYPBUFFERPROC) (Display *dpy, GLXPbuffer pbuf);
typedef void (* PFNGLXQUERYDRAWABLEPROC) (Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
typedef GLXContext (* PFNGLXCREATENEWCONTEXTPROC) (Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
typedef Bool (* PFNGLXMAKECONTEXTCURRENTPROC) (Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
typedef GLXDrawable (* PFNGLXGETCURRENTREADDRAWABLEPROC) (void);
typedef Display * (* PFNGLXGETCURRENTDISPLAYPROC) (void);
typedef int (* PFNGLXQUERYCONTEXTPROC) (Display *dpy, GLXContext ctx, int attribute, int *value);
typedef void (* PFNGLXSELECTEVENTPROC) (Display *dpy, GLXDrawable draw, unsigned long event_mask);
typedef void (* PFNGLXGETSELECTEDEVENTPROC) (Display *dpy, GLXDrawable draw, unsigned long *event_mask);
typedef __GLXextFuncPtr (* PFNGLXGETPROCADDRESSPROC) (const GLubyte *procName);

typedef void (* PFNGLXSWAPINTERVALEXTPROC)(Display*, GLXDrawable, int);

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
typedef void (*glXSwapBuffersProc)(Display*, GLXDrawable);
typedef void (*glXWaitXProc)(void);
typedef void (*glXWaitGLProc)(void);
typedef void (*glXDestroyContextProc)(Display*, GLXContext);
typedef bool (*glXMakeCurrentProc)(Display*, GLXDrawable, GLXContext);
typedef bool (*glXQueryVersionProc)(Display*, int*, int*);

typedef struct Mamuna_Context {
	GLXContext glCon;
} Mamuna_Context;

typedef struct WindowNode {
	bool keys[MAMUNA_KEY_LAST];
	bool winShouldClose;
	bool winIsActive;

	Window win;
	GLXWindow glWin;
	GLXFBConfig fbConf;
} WindowNode;



static int winCreationHints[] = {
	GLX_FBCONFIG_ID, GLX_DONT_CARE,
	GLX_RENDER_TYPE, GLX_RGBA_BIT,
	GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
	GLX_X_RENDERABLE, True,
	GLX_X_VISUAL_TYPE, GLX_DONT_CARE,
	GLX_CONFIG_CAVEAT, GLX_NONE,
	GLX_DOUBLEBUFFER, True,

	GLX_BUFFER_SIZE, 0,
	GLX_LEVEL, 0,
	GLX_STEREO, False,
	GLX_RED_SIZE, 8,
	GLX_GREEN_SIZE, 8,
	GLX_BLUE_SIZE, 8,
	GLX_ALPHA_SIZE, 0,
	GLX_DEPTH_SIZE, 24,
	GLX_STENCIL_SIZE, 8,
	GLX_SAMPLE_BUFFERS, 0,
	GLX_SAMPLES, 0,
	GLX_TRANSPARENT_TYPE, GLX_NONE,
	GLX_TRANSPARENT_RED_VALUE, GLX_DONT_CARE,
	GLX_TRANSPARENT_GREEN_VALUE, GLX_DONT_CARE,
	GLX_TRANSPARENT_BLUE_VALUE, GLX_DONT_CARE,
	GLX_TRANSPARENT_ALPHA_VALUE, GLX_DONT_CARE,
	None,
};

int contextCreationHint[] = {
	GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
	GLX_CONTEXT_MINOR_VERSION_ARB, 6,
	GLX_CONTEXT_FLAGS_ARB, 0, //GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
	GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
	None,
};

XSizeHints hints = {0};


GLXContext glCon;


Display* disp = NULL;
Atom atom = 0;
void* dynGLX = NULL;


glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
PFNGLXGETPROCADDRESSPROC glXGetProcAddress = NULL;
PFNGLXCHOOSEFBCONFIGPROC glXChooseFBConfig = NULL;
PFNGLXGETVISUALFROMFBCONFIGPROC glXGetVisualFromFBConfig = NULL;
PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = NULL;
PFNGLXCREATEWINDOWPROC glXCreateWindow = NULL;
PFNGLXDESTROYWINDOWPROC glXDestroyWindow = NULL;
glXSwapBuffersProc glXSwapBuffers = NULL;
glXDestroyContextProc glXDestroyContext = NULL;
glXMakeCurrentProc glXMakeCurrent = NULL;
glXQueryVersionProc glXQueryVersion = NULL;

bool Mamuna_Init(){
	disp = XOpenDisplay(NULL);

	dynGLX = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL);
	if(!dynGLX) return false;

	glXGetProcAddress = (PFNGLXGETPROCADDRESSPROC)dlsym(dynGLX, "glXGetProcAddress");
	if(!glXGetProcAddress) return false;

	glXChooseFBConfig = (PFNGLXCHOOSEFBCONFIGPROC)dlsym(dynGLX, "glXChooseFBConfig");
	if(!glXChooseFBConfig) return false;

	glXGetVisualFromFBConfig = (PFNGLXGETVISUALFROMFBCONFIGPROC)dlsym(dynGLX, "glXGetVisualFromFBConfig");
	if(!glXGetVisualFromFBConfig) return false;

	glXSwapBuffers = (glXSwapBuffersProc)dlsym(dynGLX, "glXSwapBuffers");
	if(!glXSwapBuffers) return false;

	glXDestroyContext = (glXDestroyContextProc)dlsym(dynGLX, "glXDestroyContext");
	if(!glXDestroyContext) return false;

	glXMakeCurrent = (glXMakeCurrentProc)dlsym(dynGLX, "glXMakeCurrent");
	if(!glXMakeCurrent) return false;

	glXQueryVersion = (glXQueryVersionProc)dlsym(dynGLX, "glXQueryVersion");
	if(!glXQueryVersion) return false;

	glXCreateWindow = (PFNGLXCREATEWINDOWPROC)dlsym(dynGLX, "glXCreateWindow");
	if(!glXCreateWindow) return false;

	glXDestroyWindow = (PFNGLXDESTROYWINDOWPROC)dlsym(dynGLX, "glXDestroyWindow");
	if(!glXDestroyWindow) return false;

	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
		glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
	if(!glXCreateContextAttribsARB) return false;

	glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((const GLubyte*)"glXSwapIntervalEXT");

	int maj, min;
	if(!glXQueryVersion(disp, &maj, &min)){
		XCloseDisplay(disp);
		disp = NULL;
		fprintf(stderr, "Error with GLX\n");
		return false;
	}

	if(maj != 1 || min != 4){
		XCloseDisplay(disp);
		disp = NULL;
		fprintf(stderr, "TOO old GLX version\n");
		return false;
	}

	windows = Mamuna_CreateHashmapInt(sizeof(WindowNode), 10);
	atom = XInternAtom(disp, "WM_DELETE_WINDOW", False);


	return true;
}

void Mamuna_Quit(){
	if(!disp) return;

	if(windows){
		size_t count;
		WindowNode* wins = Mamuna_DestroyHashmapInt(windows, &count);
		for(size_t i = 0; i < count; ++i){
			XDestroyWindow(disp, wins[i].win);
		}
		free(wins);
	}

	XCloseDisplay(disp);

	dlclose(dynGLX);
}

int Mamuna_EnumGLX(MAMUNA_ENUM en){
	switch(en){
		case MAMUNA_NONE:
			return GLX_NONE;
		case MAMUNA_TRANSPARENT_RGB:
			return GLX_TRANSPARENT_RGB;
	}

	return MAMUNA_ERROR;
}

void Mamuna_SetCreationHint(MAMUNA_ENUM hint, int value){
	int index = hint - MAMUNA_WIN_DEPTH_BUFFER_SIZE_HINT + 13;
	if((index & 1) == 0){
		++index;
	}
	int cIndex = hint - MAMUNA_CONTEXT_GL_MAJOR_VERSION_HINT + 1;
	if((cIndex & 1) == 0){
		++cIndex;
	}
	int glxVal = Mamuna_EnumGLX(value);
	switch(hint){
		case MAMUNA_WIN_DEPTH_BUFFER_SIZE_HINT:
		case MAMUNA_WIN_FB_LEVEL_HINT:
		case MAMUNA_WIN_RED_SIZE_HINT:
		case MAMUNA_WIN_GREEN_SIZE_HINT:
		case MAMUNA_WIN_BLUE_SIZE_HINT:
		case MAMUNA_WIN_ALPHA_SIZE_HINT:
		case MAMUNA_WIN_DEPTH_SIZE_HINT:
		case MAMUNA_WIN_STENCIL_SIZE_HINT:
		case MAMUNA_WIN_SAMPLE_BUFFERS_HINT:
		case MAMUNA_WIN_SAMPLES_HINT:
		case MAMUNA_WIN_TRANSPARENT_RED_VALUE_HINT:
		case MAMUNA_WIN_TRANSPARENT_GREEN_VALUE_HINT:
		case MAMUNA_WIN_TRANSPARENT_BLUE_VALUE_HINT:
		case MAMUNA_WIN_TRANSPARENT_ALPHA_VALUE_HINT:
			winCreationHints[index] = value;
			break;


		case MAMUNA_WIN_STEREO_HINT:
			winCreationHints[index] = value ? True : False;
			break;

		case MAMUNA_WIN_TRANSPARENT_TYPE_HINT:
			winCreationHints[index] = glxVal;
			break;

		case MAMUNA_WIN_POS_X_SIZE_HINT:
			hints.x = value;
			hints.flags |= PPosition;
			break;
		case MAMUNA_WIN_POS_Y_SIZE_HINT:
			hints.y = value;
			hints.flags |= PPosition;
			break;
		case MAMUNA_WIN_WIDTH_SIZE_HINT:
			hints.width = value;
			hints.min_width = value;
			hints.max_width = value;
			hints.flags |= PSize | PMinSize | PMaxSize;
			break;
		case MAMUNA_WIN_HEIGHT_SIZE_HINT:
			hints.height = value;
			hints.min_height = value;
			hints.max_height = value;
			hints.flags |= PSize | PMinSize | PMaxSize;
			break;
		case MAMUNA_CONTEXT_GL_MAJOR_VERSION_HINT:
		case MAMUNA_CONTEXT_GL_MINOR_VERSION_HINT:
		case MAMUNA_CONTEXT_GL_PROFILE_MASK_HINT:
		case MAMUNA_CONTEXT_GL_FLAGS_HINT:
			contextCreationHint[cIndex] = value;
			break;

	}
}

Mamuna_Window* Mamuna_CreateWindow(const char* title, bool fullscreen){
	if(!hints.width || !hints.height || !title || !disp || !windows) return NULL;

	int screenID = XDefaultScreen(disp);

	int count;
	GLXFBConfig* fbConf = glXChooseFBConfig(disp, screenID, winCreationHints, &count);
	if(!fbConf || count <= 0){
		return NULL;
	}


	XVisualInfo* vInfo = glXGetVisualFromFBConfig(disp, fbConf[0]);
	if(!vInfo) return NULL;

	Window root = XRootWindow(disp, screenID);


	XSetWindowAttributes attrs = {0};
	attrs.bit_gravity = ForgetGravity;
	attrs.colormap = XCreateColormap(disp, root, vInfo->visual, AllocNone);



	Window win = XCreateWindow(disp, root, hints.x, hints.y, hints.width, hints.height, 1,
			vInfo->depth, InputOutput, vInfo->visual, CWBitGravity | CWColormap, &attrs);
	XStoreName(disp, win, title);
	XSetWMProtocols(disp, win, &atom, 1);
	XSelectInput(disp, win, FocusChangeMask | KeyPressMask | KeyReleaseMask |
			ExposureMask | ButtonMotionMask | ButtonPressMask |
			ButtonReleaseMask | KeymapStateMask |
			StructureNotifyMask | PropertyChangeMask | PointerMotionMask);

	XSetWMNormalHints(disp, win, &hints);

	++openWindows;

	WindowNode node = {0};
	node.win = win;
	node.glWin = glXCreateWindow(disp, fbConf[0], win, NULL);
	node.fbConf = fbConf[0];

	XMapWindow(disp, win);
	XEvent ev;
	while(!XCheckTypedWindowEvent(disp, win, MapNotify, &ev)){
        	XSync(disp, False);
    	}

	XFree(fbConf);
	XFree(vInfo);

	Mamuna_HashmapIntAddElement(windows, win, &node);

	Mamuna_Window* window = calloc(1, sizeof(Mamuna_Window));
	window->index = win;

	
	return window;
}

void Mamuna_DestroyWindow(Mamuna_Window* window){
	if(!window || !disp || !windows) return;

	WindowNode* win = Mamuna_HashmapIntRemoveElement(windows, window->index);	
	free(window);

	if(!win){
		return;
	}

	XUnmapWindow(disp, win->win);
	glXDestroyWindow(disp, win->glWin);
	XDestroyWindow(disp, win->win);
	free(win);

	--openWindows;
}

MAMUNA_ENUM Mamuna_MoveWindow(Mamuna_Window* window, int x, int y){
	if(!window || !disp || !windows) return MAMUNA_ERROR;
	
	WindowNode* win = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!win){
		return MAMUNA_IS_NOT_EXIST;
	}

	XMoveWindow(disp, win->win, x, y);

	return MAMUNA_SUCCESS;
}

MAMUNA_ENUM Mamuna_RenameWindow(Mamuna_Window* window, const char* name){
	if(!window || !name || !disp || !windows) return MAMUNA_ERROR;
	
	WindowNode* win = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!win){
		return MAMUNA_IS_NOT_EXIST;
	}

	XStoreName(disp, win->win, name);
	return MAMUNA_SUCCESS;
}

void Mamuna_PollEvents(){
	if(!disp) return;
	XEvent ev;

	while(XPending(disp) > 0){
		XNextEvent(disp, &ev);

		switch(ev.type){
			case ClientMessage:
			{
				if((Atom)ev.xclient.data.l[0] == atom){
					size_t count;
					WindowNode* win = Mamuna_HashmapIntGetElement(windows, ev.xclient.window);
					win->winShouldClose = true;
				}
			} break;
			case FocusIn:
			{
				size_t count;
				WindowNode* win = Mamuna_HashmapIntGetElement(windows, ev.xfocus.window);
				win->winIsActive = true;
			} break;
			case FocusOut:
			{
				size_t count;
				WindowNode* win = Mamuna_HashmapIntGetElement(windows, ev.xfocus.window);
				win->winIsActive = false;
			} break;
			case KeyPress:
			{
				globKeys[ev.xkey.keycode] = true;
				WindowNode* win = Mamuna_HashmapIntGetElement(windows, ev.xkey.window);
				if(win){
					win->keys[ev.xkey.keycode] = true;	
				}
			} break;
			case KeyRelease:
			{
				globKeys[ev.xkey.keycode] = false;
				WindowNode* win = Mamuna_HashmapIntGetElement(windows, ev.xkey.window);
				if(win){
					win->keys[ev.xkey.keycode] = false;	
				}

			} break;
		}  	
	}


}

void Mamuna_WindowSwapBuffers(Mamuna_Window* window){
	if(!window || !disp || !windows) return;

	WindowNode* node = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!node) return;
	

	glXSwapBuffers(disp, node->glWin);
	XFlush(disp);
	glFinish();
}

bool Mamuna_IsRunning(){
	return openWindows > 0;
}

bool Mamuna_WindowShouldClose(Mamuna_Window* window){
	if(!window || !disp || !windows) return true;

	WindowNode* node = Mamuna_HashmapIntGetElement(windows, window->index);
	if(node){
		return node->winShouldClose;
	}

	return true;
}

bool Mamuna_WindowIsActive(Mamuna_Window* window){
	if(!window || !disp || !windows) return false;

	WindowNode* node = Mamuna_HashmapIntGetElement(windows, window->index);
	if(node){
		return node->winIsActive;
	}

	return false;
}

void Mamuna_WindowGetSize(Mamuna_Window* window, int* width, int* height){
	if(!window || !disp || !windows) return;
	
	WindowNode* node = Mamuna_HashmapIntGetElement(windows, window->index);
	XWindowAttributes attr;
	XGetWindowAttributes(disp, node->win, &attr);
	if(width)
		*width = attr.width;
	if(height)
		*height = attr.height;
}

GLADapiproc Mamuna_GetProcAddress(const char* name){
	GLADapiproc proc = (GLADapiproc)glXGetProcAddress((const GLubyte*)name);
	if(!proc){
		proc = (GLADapiproc)dlsym(dynGLX, name);
	}

	return proc;
}

Mamuna_Context* Mamuna_CreateContext(Mamuna_Window* window){
	if(!window || !disp || !windows) return NULL;
	WindowNode* win = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!win) return NULL;
	Mamuna_Context* con = calloc(1, sizeof(Mamuna_Context));
	con->glCon = glXCreateContextAttribsARB(disp, win->fbConf, 0, True, contextCreationHint);

	//

	return con;
}

void Mamuna_DestroyContext(Mamuna_Context* context){
	if(!context || !disp) return;
	glXDestroyContext(disp, context->glCon);
	free(context);
}

MAMUNA_ENUM Mamuna_MakeCurrent(Mamuna_Window* window, Mamuna_Context* context){
	if(!window || !context || !disp || !windows) return MAMUNA_ERROR;

	WindowNode* win = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!win) return MAMUNA_IS_NOT_EXIST;

	if(!glXMakeCurrent(disp, win->glWin, context->glCon)){
		return MAMUNA_ERROR;
	}

	return MAMUNA_SUCCESS;
}

const bool* Mamuna_GetGlobalKeyboardState(){
	if(!disp) return NULL;
	return globKeys;
}

const bool* Mamuna_GetKeyboardState(Mamuna_Window* window){
	if(!disp || !windows || !window) return NULL;
	WindowNode* win = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!win) return NULL;
	return win->keys;
}

uint64_t Mamuna_GetPerformanceFrequency(){
	return 1000000000ULL;
}

uint64_t Mamuna_GetPerformanceCounter(){
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

void Mamuna_SetSwapInterval(Mamuna_Window* window, int mode){
	if(!disp || !window || !windows) return;
	
	WindowNode* node = Mamuna_HashmapIntGetElement(windows, window->index);
	if(!node) return;

	glXSwapIntervalEXT(disp, node->glWin, mode);

}

#else 
	#error "NOT COMPATIBLE SYSTEM"
#endif
