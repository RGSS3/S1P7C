#include "../s1p7c.h"

spcfn_window(WndProc, h, m, w, l) {
    (void)h, (void)m, (void)w, (void)l;
    return DefWindowProc(h, m, w, l);
}


int main(void) {
    spcg_class(w) {
       w->lpfnWndProc = WndProc;
    }
    spcg_window(w) {
       w->lpWindowName = "Hello world";
    }
    /*
    spcg_window(w, SPC_USE_OPENGL) {
       w->lpWindowName = "Hello world";
    }
    */
    return (int)spc_loop_basic();
}