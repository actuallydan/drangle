## How it Works:

1. Getting the Active Window: This is done using the `_NET_ACTIVE_WINDOW` property, which retrieves the currently focuse`d window.
2. Resizing and Moving the Window: Once the active window is identified, it's resized and moved using `XMoveResizeWindow`.
3. Screen Dimensions: The Xrandr extension is used to get the screen's resolution, so the window can be resized to exactly 1/3 of the screen width.
4. Keyboard Shortcut Detection: The program grabs the `Ctrl` + `Meta` + `Left Arrow key` combination and listens for it, applying the window move action when pressed.

## Compilation:

Make sure you have X11 and Xrandr development libraries installed. You can compile the program like this:

```bash
gcc -o drangle drangle.c -lX11 -lXrandr
```

## Running the Program:

Run the program from the terminal:

```bash
./drangle
```

Now, when you press `Ctrl` + `Meta` + `Left Arrow`, the active window will move and resize to occupy the leftmost 1/3 of your screen.

You can extend this program to handle other key combinations and window arrangements (right side, center, etc.).