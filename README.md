## How it Works:

1. Getting the Active Window: This is done using the `_NET_ACTIVE_WINDOW` property, which retrieves the currently focused window.
2. Resizing and Moving the Window: Once the active window is identified, it's resized and moved using `XMoveResizeWindow`.
3. Screen Dimensions: The Xrandr extension is used to get the screen's resolution, so the window can be resized to exactly 1/3 of the screen width.
4. Keyboard Shortcut Detection: The program listens for various key combinations to control window positioning and sizing.

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

## Available Shortcuts:

### Window Positioning
- **Thirds**: Use `Ctrl` + `Meta` + `D/F/G` to position window in left/middle/right third
- **Halves**: Use `Ctrl` + `Meta` + `Left/Right Arrow` to split window to left/right half
- **Quarters**: Use `Ctrl` + `Meta` + `U/I/O/P` to position window in quarters from left to right

You can extend this program to handle other key combinations and window arrangements as needed.