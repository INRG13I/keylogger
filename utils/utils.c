#include <unistd.h>
#include <stdio.h>

#include "utils.h"

char* getHostName() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return *hostname;
    } else {
        perror("gethostname");
    }
}

int createLog(FILE* logfile){
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

    logfile = fopen(logFileName, "a");
    return 0;
}
