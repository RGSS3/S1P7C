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
    
    spcgl_loop_basic() {
		glClearColor(0.4f, 0.8f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    return 0;
}
