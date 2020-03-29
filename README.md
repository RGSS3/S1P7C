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

## Who's like-minded?
Single header, whole implementation.

1. https://github.com/nothings/stb
1. https://github.com/randrew/layout
1. https://github.com/vurtun/nuklear
1. https://github.com/sheredom/utf8.h
1. https://github.com/sheredom/json.h
1. https://github.com/richgel999/miniz
1. https://github.com/dr-soft/miniaudio


