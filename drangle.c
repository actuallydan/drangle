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
#define RIGHT_ARROW XK_Right
#define KEY_D XK_d
#define KEY_F XK_f
#define KEY_G XK_g
#define KEY_U XK_u
#define KEY_I XK_i
#define KEY_O XK_o
#define KEY_P XK_p

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

void get_screen_size(Display *dpy, int *width, int *height, int *taskbar_height) {
    Window root = DefaultRootWindow(dpy);
    XRRScreenResources *resources = XRRGetScreenResources(dpy, root);
    if (resources) {
        XRRCrtcInfo *crtc_info = XRRGetCrtcInfo(dpy, resources, resources->crtcs[0]);

        if (crtc_info) {
            *width = crtc_info->width; // Physical width
            *height = crtc_info->height; // Physical height
            XRRFreeCrtcInfo(crtc_info);
        }

        // Get the height of the taskbar (this is a rough estimate)
        *taskbar_height = (*height - crtc_info->height) / crtc_info->height * *height;

        fprintf(stdout, "taskbarheight: %i height: %i width: %i\n", *taskbar_height, *height, *width);
        
        XRRFreeScreenResources(resources);
    }
}

int main() {
    Display *dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        fprintf(stderr, "Unable to open display\n");
        return 1;
    }

    Window root = DefaultRootWindow(dpy);
    XEvent ev;

    // Release any previous key grabs for the same key combinations
    XUngrabKey(dpy, XKeysymToKeycode(dpy, KEY_D), CTRL | META, root);
    XUngrabKey(dpy, XKeysymToKeycode(dpy, KEY_F), CTRL | META, root);
    XUngrabKey(dpy, XKeysymToKeycode(dpy, KEY_G), CTRL | META, root);
    XUngrabKey(dpy, XKeysymToKeycode(dpy, LEFT_ARROW), CTRL | META, root);
    XUngrabKey(dpy, XKeysymToKeycode(dpy, RIGHT_ARROW), CTRL | META, root);
    XUngrabKey(dpy, XKeysymToKeycode(dpy, KEY_U), CTRL | META, root);
    XUngrabKey(dpy, XKeysymToKeycode(dpy, KEY_I), CTRL | META, root);
    XUngrabKey(dpy, XKeysymToKeycode(dpy, KEY_O), CTRL | META, root);
    XUngrabKey(dpy, XKeysymToKeycode(dpy, KEY_P), CTRL | META, root);

    // Grab the key combinations
    XGrabKey(dpy, XKeysymToKeycode(dpy, KEY_D), CTRL | META, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, KEY_F), CTRL | META, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, KEY_G), CTRL | META, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, LEFT_ARROW), CTRL | META, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, RIGHT_ARROW), CTRL | META, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, KEY_U), CTRL | META, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, KEY_I), CTRL | META, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, KEY_O), CTRL | META, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, KEY_P), CTRL | META, root, True, GrabModeAsync, GrabModeAsync);

    int screen_width, screen_height, taskbar_height;

    while (1) {
        XNextEvent(dpy, &ev);
        if (ev.type == KeyPress) {
            get_screen_size(dpy, &screen_width, &screen_height, &taskbar_height);
            Window active_win = get_active_window(dpy);
            if (active_win) {
                if ((ev.xkey.state & CTRL) && (ev.xkey.state & META)) {
                    int adjusted_height = screen_height -  80; // Adjust height for taskbar
                    if (ev.xkey.keycode == XKeysymToKeycode(dpy, KEY_D)) {
                        // Move to the left third
                        int third_width = screen_width / 3;
                        move_window(dpy, active_win, 0, 0, third_width, adjusted_height); // Full height minus taskbar
                    } else if (ev.xkey.keycode == XKeysymToKeycode(dpy, KEY_F)) {
                        // Move to the middle third
                        int third_width = screen_width / 3;
                        move_window(dpy, active_win, third_width, 0, third_width, adjusted_height); // Full height minus taskbar
                    } else if (ev.xkey.keycode == XKeysymToKeycode(dpy, KEY_G)) {
                        // Move to the right third
                        int third_width = screen_width / 3;
                        move_window(dpy, active_win, third_width * 2, 0, third_width, adjusted_height); // Full height minus taskbar
                    } else if (ev.xkey.keycode == XKeysymToKeycode(dpy, LEFT_ARROW)) {
                        int half_width = screen_width / 2;
                        move_window(dpy, active_win, 0, 0, half_width, adjusted_height);
                    } else if (ev.xkey.keycode == XKeysymToKeycode(dpy, RIGHT_ARROW)) {
                        int half_width = screen_width / 2;
                        move_window(dpy, active_win, half_width, 0, half_width, adjusted_height);
                    } else if (ev.xkey.keycode == XKeysymToKeycode(dpy, KEY_U)) {
                        int quarter_width = screen_width / 4;
                        move_window(dpy, active_win, 0, 0, quarter_width, adjusted_height);
                    } else if (ev.xkey.keycode == XKeysymToKeycode(dpy, KEY_I)) {
                        int quarter_width = screen_width / 4;
                        move_window(dpy, active_win, quarter_width, 0, quarter_width, adjusted_height);
                    } else if (ev.xkey.keycode == XKeysymToKeycode(dpy, KEY_O)) {
                        int quarter_width = screen_width / 4;
                        move_window(dpy, active_win, quarter_width * 2, 0, quarter_width, adjusted_height);
                    } else if (ev.xkey.keycode == XKeysymToKeycode(dpy, KEY_P)) {
                        int quarter_width = screen_width / 4;
                        move_window(dpy, active_win, quarter_width * 3, 0, quarter_width, adjusted_height);
                    }
                }
            }
        }
    }

    XCloseDisplay(dpy);
    return 0;
}
