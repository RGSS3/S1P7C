# S1P7C

```c
#include "../s1p7c.h"
#include <stdio.h>

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
    spcg_window(w) {
        w->lpWindowName = "Hello world";
    }
    return (int)spc_loop_basic();
}
```
