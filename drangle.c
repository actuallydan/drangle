#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/extensions/Xrandr.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define META Mod4Mask   // Meta key (usually the "Windows" key)
#define CTRL ControlMask
#define LEFT_ARROW XK_Left

void move_window(Display *dpy, Window win, int x, int y, int width, int height) {
    XMoveResizeWindow(dpy, win, x, y, width, height);
    XFlush(dpy); // Force X to apply changes immediately
}

Window get_active_window(Display *dpy) {
    Window root = DefaultRootWindow(dpy);
    Atom atom = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", True);
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    Window *active_window = NULL;

    int status = XGetWindowProperty(dpy, root, atom, 0, (~0L), False, AnyPropertyType,
                                    &actual_type, &actual_format, &nitems, &bytes_after,
                                    (unsigned char **)&active_window);

    if (status == Success && active_window) {
        Window win = *active_window;
        XFree(active_window);
        return win;
    }
    return 0;
}

void get_screen_size(Display *dpy, int *width, int *height) {
    Window root = DefaultRootWindow(dpy);
    XRRScreenSize *sizes;
    int num_sizes;
    XRRScreenConfiguration *conf = XRRGetScreenInfo(dpy, root);
    Rotation current_rotation;

    sizes = XRRConfigSizes(conf, &num_sizes);
    if (sizes) {
        // Get the width and height of the primary screen
        *width = DisplayWidth(dpy, DefaultScreen(dpy));
        *height = DisplayHeight(dpy, DefaultScreen(dpy));

        fprintf(stdout, "width: %i height: %i\n", *width, *height);
    }
    XRRFreeScreenConfigInfo(conf);
}

int main() {
    Display *dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        fprintf(stderr, "Unable to open display\n");
        return 1;
    }

    Window root = DefaultRootWindow(dpy);
    XEvent ev;

    // Release any previous key grabs for the same key combination
    XUngrabKey(dpy, XKeysymToKeycode(dpy, LEFT_ARROW), CTRL | META, root);

    // Grab the key combination
    XGrabKey(dpy, XKeysymToKeycode(dpy, LEFT_ARROW), CTRL | META, root, True, GrabModeAsync, GrabModeAsync);

    int screen_width, screen_height;
    get_screen_size(dpy, &screen_width, &screen_height);

    while (1) {
        XNextEvent(dpy, &ev);
        if (ev.type == KeyPress) {
            if ((ev.xkey.state & CTRL) && (ev.xkey.state & META) && ev.xkey.keycode == XKeysymToKeycode(dpy, LEFT_ARROW)) {
                Window active_win = get_active_window(dpy);
                if (active_win) {
                    // Ensure the window is resized to the left-most 1/3 of the screen and full height
                    int third_width = screen_width / 3;
                    move_window(dpy, active_win, 0, 0, third_width, screen_height); // Full height
                }
            }
        }
    }

    XCloseDisplay(dpy);
    return 0;
}
