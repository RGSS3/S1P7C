#include "../s1p7c.h"
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
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
    
    glViewport(0, 0, 640, 480);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glMatrixMode(GL_MODELVIEW);  
    glLoadIdentity(); 
    
    
    float a = 0.1f;
    const float fps = 60.0f;
    const float velocity = 20.0f;
    spcgl_loop_dt(dt) {
    	if (!spc_timespan_mark(&dt, 1.0f/fps)) {
    		continue;	
    	}
    	a += velocity * dt.span;
    	glClearColor(0.4f, 0.8f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(a, 0);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(a + 100, 0);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(a + 100, 100);
		glEnd();
		if (a > 640) a = 0;
    }
    return 0;
}
