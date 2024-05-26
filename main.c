// THIS MUST BE DONE TO USE THE TGS GRAPHICSLIB

//void TGSEventLoop() {
//#ifdef _WIN32
//    MSG msg;
//    while (GetMessage(&msg, NULL, 0, 0)) {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//    ReleaseDC(hwnd, hdc);
//#else
//    XEvent event;
//    while (1) {
//      XNextEvent(display, &event);
//      if (event.type == Expose) {
//          DrawSquare(100, 100, 50, 0xFF0000); // Red square
//          DrawCircle(300, 200, 30, 0x00FF00); // Green circle            <----------------- YOU MUST PUT YOUR DRAWINGS HERE!!!! 
//          DrawTriangle(400, 100, 450, 150, 500, 100, 0x0000FF); // Blue triangle
//      }
//  }
//  XCloseDisplay(display);
//#endif
//}


#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <Windows.h>
    #include <wingdi.h>
#else
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/Xos.h>
    #include <unistd.h>
#endif

// Forward declarations of drawing functions
void DrawSquare(int x, int y, int size, unsigned long color);
void DrawCircle(int x, int y, int radius, unsigned long color);
void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color);
void TGSWIN();
void TGSEventLoop();

// Global variables for window and display
#ifdef _WIN32
    HWND hwnd;
    HDC hdc;
#else
    Display *display;
    Window window;
    GC gc;
#endif

// Function to create a window
void TGSWIN() {
#ifdef _WIN32
    // Windows-specific window creation
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS wc = {0};

    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyWindowClass";

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    hwnd = CreateWindowEx(
        0,
        L"MyWindowClass",
        L"My First Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    ShowWindow(hwnd, SW_SHOW);
    hdc = GetDC(hwnd);
#else
    // Linux-specific window creation
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        return;
    }

    int screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 800, 600, 1,
                                  BlackPixel(display, screen), WhitePixel(display, screen));

    XSelectInput(display, window, ExposureMask);
    XMapWindow(display, window);
    gc = DefaultGC(display, screen);
#endif
}

// Function to draw a square
void DrawSquare(int x, int y, int size, unsigned long color) {
#ifdef _WIN32
    // Windows-specific square drawing
    RECT rect = {x, y, x + size, y + size};
    HBRUSH hBrush = CreateSolidBrush(color);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
#else
    // Linux-specific square drawing
    XSetForeground(display, gc, color);
    XFillRectangle(display, window, gc, x, y, size, size);
    XFlush(display);
#endif
}

// Function to draw a circle
void DrawCircle(int x, int y, int radius, unsigned long color) {
#ifdef _WIN32
    // Windows-specific circle drawing
    HBRUSH hBrush = CreateSolidBrush(color);
    SelectObject(hdc, hBrush);
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
    DeleteObject(hBrush);
#else
    // Linux-specific circle drawing
    XSetForeground(display, gc, color);
    XFillArc(display, window, gc, x - radius, y - radius, 2 * radius, 2 * radius, 0, 360 * 64);
    XFlush(display);
#endif
}

// Function to draw a triangle
void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color) {
#ifdef _WIN32
    // Windows-specific triangle drawing
    POINT points[3] = {{x1, y1}, {x2, y2}, {x3, y3}};
    HBRUSH hBrush = CreateSolidBrush(color);
    HPEN hPen = CreatePen(PS_SOLID, 1, color);
    SelectObject(hdc, hBrush);
    SelectObject(hdc, hPen);
    Polygon(hdc, points, 3);
    DeleteObject(hBrush);
    DeleteObject(hPen);
#else
    // Linux-specific triangle drawing
    XSetForeground(display, gc, color);
    XPoint points[] = {{x1, y1}, {x2, y2}, {x3, y3}};
    XFillPolygon(display, window, gc, points, 3, Convex, CoordModeOrigin);
    XFlush(display);
#endif
}

// Function to draw a rectangle
void DrawRectangle(int x, int y, int width, int height, unsigned long color) {
#ifdef _WIN32
    // Windows-specific rectangle drawing
    RECT rect = {x, y, x + width, y + height};
    HBRUSH hBrush = CreateSolidBrush(color);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
#else
    // Linux-specific rectangle drawing
    XSetForeground(display, gc, color);
    XFillRectangle(display, window, gc, x, y, width, height);
    XFlush(display);
#endif
}


// Function to handle the event loop
// Function to handle the event loop
void TGSEventLoop() {
#ifdef _WIN32
    // Windows-specific message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    ReleaseDC(hwnd, hdc);
#else
    // Linux-specific event loop
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            // Redraw the shapes when window is exposed
            DrawSquare(100, 100, 50, 0xFF0000); // Red square
            DrawCircle(300, 200, 30, 0x00FF00); // Green circle
            DrawTriangle(400, 100, 450, 150, 500, 100, 0x0000FF); // Blue triangle
	    DrawRectangle(200, 300, 80, 50, 0x000000);
        }
    }
    XCloseDisplay(display);
#endif
}


int main() {
    // Call TGSWIN to create a window
    TGSWIN();
  
    TGSEventLoop();

    return 0;
}
