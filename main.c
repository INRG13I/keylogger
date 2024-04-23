#include <stdio.h>

#ifdef _WIN32
const int os_code = 1; // Windows
#elif __APPLE__
#include <TargetConditionals.h>
  #if TARGET_OS_MAC
    const int os_code = 2; // macOS
  #endif
#else
const int os_code = 3; // Linux
#endif

int main() {
    switch (os_code) {
        case 1:
            printf("Operating System: Windows\n");
            printf("Code: 1\n");
            break;
        case 2:
            printf("Operating System: macOS\n");
            printf("Code: 2\n");
            break;
        case 3:
            printf("Operating System: Linux\n");
            printf("Code: 3\n");
            break;    

}
    return 0;
}


/*
 * Conventions:
 * all individual keyloggers take the logfile as a input
 *
 *
 *
 * Error codes general:
 * 1-unable to get host name
 * 2-unable to open logfile
 *
 *
 * Error codes macos:
 * 10-unable to create event tap.
 *
 *
 *
 * */
