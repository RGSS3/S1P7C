#include "../s1p7c.h"
#include <stdio.h>
#include <GL/gl.h>
#include <GL/wglext.h>
#define LOAD_GL(t, n) PFN##t##PROC n=(PFN##t##PROC)wglGetProcAddress(#n);
spcfn_window(wndproc, h, m, w, l) {
    (void)h;
    (void)w;
    (void)l;
    if (m == WM_LBUTTONDOWN) {
        MessageBox(0, "Hello", 0, 16);
        return TRUE;
    }
    return FALSE;
}

int main() {
    spcg_class(w) {
        w->lpfnWndProc = wndproc;
    }
    spcg_window(w, SPC_USE_OPENGL) {
        w->lpWindowName = "Hello OpenGL";
    }
    //LOAD_GL(WGLSWAPINTERVALEXT, wglSwapIntervalEXT); wglSwapIntervalEXT(1);
    while(1){
		MSG msg;
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){
			if(msg.message==WM_QUIT) return 0;
			TranslateMessage(&msg); DispatchMessage(&msg);
		}
		glClearColor(0.4f, 0.8f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		HDC hdc=GetDC(SPC_HWND);
		SwapBuffers(hdc);
    }
    return 0;
}
