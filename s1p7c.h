#ifndef S1P7C_H_
#define S1P7C_H_

#include <windows.h>
#define using(TYPE, OBJECT, CREATE, ...) \
    for (int S1P7C_init = 1; S1P7C_init; )\
    for (TYPE OBJECT = (CREATE); S1P7C_init; \
 (void)(S1P7C_init = 0),\
 (void)(__VA_ARGS__)(OBJECT))

#define SPCDefaultClass TEXT("SPC Window")
#define SPCDefStruct(a) typedef struct a a; struct a

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
};

#define scoped_pointer(a) a
scoped_pointer(WNDCLASSEX*)spcDefaultClass();
ATOM spcf_class_register(WNDCLASSEX *);
scoped_pointer(spcs_create_window_ex_t *)spcDefaultWindow();
#define spcg_class(w)\
    using(WNDCLASSEX *, w, spcDefaultClass(), spcf_class_register)



HWND
spcf_window_create(spcs_create_window_ex_t *);
WPARAM spc_loop_basic(void);
LRESULT CALLBACK SPCMainWindowProcRoutine(WNDPROC, HWND, UINT, WPARAM, LPARAM);
#define spcg_window(w) \
    using(spcs_create_window_ex_t *, w, spcDefaultWindow(), spcf_window_create)

#define spcfn_window(name, h, m, w, l) \
    LRESULT CALLBACK name (HWND h, UINT m, WPARAM w, LPARAM l) {\
 LRESULT CALLBACK name##_handler (HWND h, UINT m, WPARAM w, LPARAM l);\
 return SPCMainWindowProcRoutine(name##_handler, h, m, w, l);\
    }\
    LRESULT CALLBACK name##_handler (HWND h, UINT m, WPARAM w, LPARAM l)

#  ifndef S1P7C_NO_IMPLEMENTATION
scoped_pointer(WNDCLASSEX*)spcDefaultClass() {
    WNDCLASSEX *w = (WNDCLASSEX *)     calloc(1, sizeof(*w));
    w->cbSize = sizeof(*w);
    w->style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    w->lpfnWndProc = DefWindowProc;
    w->lpszClassName = SPCDefaultClass;
    w->hCursor = LoadCursor(NULL, IDC_ARROW);
    w->hbrBackground = (HBRUSH)(COLOR_WINDOW);
    w->hIcon = LoadIcon(NULL, IDI_APPLICATION);
    w->hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    return w;
}

ATOM spcf_class_register(WNDCLASSEX *w) {
    ATOM    ret = RegisterClassEx(w);
    free(w);
    return ret;
}

scoped_pointer(spcs_create_window_ex_t*)spcDefaultWindow() {
    spcs_create_window_ex_t *w = (spcs_create_window_ex_t*) malloc(sizeof(*w));
    w->dwExStyle = 0;
    w->lpClassName = SPCDefaultClass;
    w->lpWindowName = TEXT("");
    w->dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    w->x = CW_USEDEFAULT;
    w->y = CW_USEDEFAULT;
    w->nWidth = 640;
    w->nHeight = 480;
    w->hWndParent = (HWND)     0;
    w->hMenu = (HMENU)0;
    w->hInstance = 0;
    w->lpParam = 0;
    return w;
}

HWND spcf_window_create(spcs_create_window_ex_t *w) {
    HWND    h =
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
    free(w);
    return h;
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

#  endif



#endif