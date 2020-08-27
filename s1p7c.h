#ifndef S1P7C_H_
#define S1P7C_H_

#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define using(TYPE, OBJECT, CREATE, ...) \
    for (int S1P7C_init = 1; S1P7C_init; )\
    for (TYPE OBJECT = (CREATE); S1P7C_init; \
 (void)(S1P7C_init = 0),\
 (void)(__VA_ARGS__)(OBJECT))

#define SPCDefaultClass TEXT("SPC Window")
#define SPCDefStruct(a) typedef struct a a; struct a

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define WINASSERT(x) do{DWORD err;\
	if(!(x) || ((err=GetLastError())!=0)){\
		LPTSTR str=0;\
		if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,0,err,0,(LPTSTR)&str,1,NULL)){\
			fwprintf(stderr, L"S1P7C_ASSERT FAILED: " WIDEN(#x) " @ " WIDEN(__FILE__) " GetLastError()=%d (%s)\n",err, str);\
		}else{fwprintf(stderr, L"S1P7C_ASSERT FAILED: " WIDEN(#x) " @ " WIDEN(__FILE__) " GetLastError()=%d (SPC Unknown Error)\n",err);}abort();\
	}\
}while(0);
#define SPC_DEFAULT 0
#define SPC_USE_OPENGL 1
#define SPC_OPENGL_ENABLE_VSYNC 2

static HWND SPC_HWND=0;

SPCDefStruct(
    spcs_create_window_ex_t) {
    DWORD    dwExStyle;
    LPCTSTR    lpClassName;
    LPCTSTR    lpWindowName;
    DWORD    dwStyle;
    int    x;
    int    y;
    int    nWidth;
    int    nHeight;
    HWND    hWndParent;
    HMENU    hMenu;
    HINSTANCE    hInstance;
    LPVOID    lpParam;
    int spcFlags;
};


SPCDefStruct(spcs_timespan_t) {
	long clk;
	int used;
	float span;
};
	

#define scoped_pointer(a) a
scoped_pointer(WNDCLASSEX*)spcDefaultClass();
ATOM spcf_class_register(WNDCLASSEX *);
scoped_pointer(spcs_create_window_ex_t *)spcDefaultWindow();
#define spcg_class(w)\
    using(WNDCLASSEX *, w,\
          spcDefaultClass((WNDCLASSEX []){0}),\
          spcf_class_register)



HWND
spcf_window_create(spcs_create_window_ex_t *);
WPARAM spc_loop_basic(void);
int spc_yield(void);
void spcgl_swap_buffers();
int spc_timespan_mark(spcs_timespan_t *, float dt);
LRESULT CALLBACK SPCMainWindowProcRoutine(WNDPROC, HWND, UINT, WPARAM, LPARAM);
#define spcg_window(w, flags) \
    using(spcs_create_window_ex_t *, w,\
            spcDefaultWindow((spcs_create_window_ex_t[]){0}, flags), \
            spcf_window_create)

#define spcfn_window(name, h, m, w, l) \
    LRESULT CALLBACK name (HWND h, UINT m, WPARAM w, LPARAM l) {\
 LRESULT CALLBACK name##_handler (HWND h, UINT m, WPARAM w, LPARAM l);\
 return SPCMainWindowProcRoutine(name##_handler, h, m, w, l);\
    }\
    LRESULT CALLBACK name##_handler (HWND h, UINT m, WPARAM w, LPARAM l)

#  ifndef S1P7C_NO_IMPLEMENTATION
scoped_pointer(WNDCLASSEX*)spcDefaultClass(scoped_pointer(WNDCLASSEX*) w) {
    w->cbSize = sizeof(*w);
    w->style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    w->lpfnWndProc = DefWindowProc;
    w->lpszClassName = SPCDefaultClass;
    w->hCursor = LoadCursor(NULL, IDC_ARROW);
    w->hbrBackground = (HBRUSH)(COLOR_WINDOW);
    w->hIcon = LoadIcon(NULL, IDI_APPLICATION);
    w->hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    return w;
}

ATOM spcf_class_register(WNDCLASSEX *w) {
    ATOM   ret = RegisterClassEx(w);
    return ret;
}

scoped_pointer(spcs_create_window_ex_t*)spcDefaultWindow(
        scoped_pointer(spcs_create_window_ex_t*) w, int flags) {
    w->dwExStyle = 0;
    w->lpClassName = SPCDefaultClass;
    w->lpWindowName = TEXT("");
    w->dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    w->x = CW_USEDEFAULT;
    w->y = CW_USEDEFAULT;
    w->nWidth = 640;
    w->nHeight = 480;
    w->hWndParent = (HWND)0;
    w->hMenu = (HMENU)0;
    w->hInstance = 0;
    w->lpParam = 0;
    w->spcFlags=flags;
    return w;
}

HWND spcf_window_create(spcs_create_window_ex_t *w) {
    HWND    hwnd =
        CreateWindowEx(
            w->dwExStyle,
            w->lpClassName,
            w->lpWindowName,
            w->dwStyle,
            w->x,
            w->y,
            w->nWidth,
            w->nHeight,
            w->hWndParent,
            w->hMenu,
            w->hInstance,
            w->lpParam);
    WINASSERT(hwnd);
    if(w->spcFlags & SPC_USE_OPENGL) {
        PIXELFORMATDESCRIPTOR pfd= {
        	sizeof(pfd),
        	1,
        	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER ,
        	PFD_TYPE_RGBA,
        	32,
        	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
        };
        HDC hdc=GetDC(hwnd);
        assert(hdc);
		WINASSERT(hdc!=NULL);
        SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
        HGLRC hrc=wglCreateContext(hdc);
        WINASSERT(hrc!=NULL);
        WINASSERT(wglMakeCurrent(hdc, hrc));
        ReleaseDC(hwnd, hdc);
    }
    SPC_HWND=hwnd;
    return hwnd;
}

WPARAM
spc_loop_basic(void) {
    MSG msg;
    while(GetMessage(&msg, 0, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

#define spcs_timespan_INIT() {0, 0, 0.0f}

#define spcgl_loop_basic() for (; spc_yield(); spcgl_swap_buffers(NULL))
#define spcgl_loop_dt(dt) \
    for (spcs_timespan_t dt = spcs_timespan_INIT();\
    	             spc_yield(); spcgl_swap_buffers(&dt))

LRESULT CALLBACK SPCMainWindowProcRoutine(
    WNDPROC wndproc,
    HWND h,
    UINT m,
    WPARAM w,
    LPARAM l) {
    LRESULT r = wndproc(h, m, w, l);
    if(r) {
        return r;
    }
    if(m == WM_CREATE) {
        UpdateWindow(h);
        InvalidateRect(h, NULL, TRUE);
    }
    if(m == WM_CLOSE) {
        PostQuitMessage(0);
    }
    return DefWindowProc(h, m, w, l);
}

int spc_yield() {
	MSG msg;
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return 0;
		}
		TranslateMessage(&msg); 
		DispatchMessage(&msg);
	}
	return 1;
}


void spcgl_swap_buffers(spcs_timespan_t *dt) {
	if (!dt || dt->used) {
		HDC hdc = GetDC(SPC_HWND);
		SwapBuffers(hdc);
		ReleaseDC(SPC_HWND, hdc);
		if (dt) {
			dt->used = 1;
		}
	}
}

int spc_timespan_mark(spcs_timespan_t *dt, float interval) {
	if (dt->clk == 0) {
		dt->clk   = clock();
		dt->span  = 0;
		dt->used = 1;
		return 1;
	} else {
		float delta = (float)((double)(clock() - dt->clk) / CLOCKS_PER_SEC);
		if (delta >= interval) {
			dt->span = delta;
			dt->clk  = clock();
			dt->used = 1;
			return 1;
		}
		return 0;
	}
}


#  endif



#endif
