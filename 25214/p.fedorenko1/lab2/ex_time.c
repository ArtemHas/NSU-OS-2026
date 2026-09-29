#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

extern char *tzname[];

int main() {
    time_t now; 
    struct tm *sp;
    char timezone[] = "TZ=PST8";

    if (putenv(timezone) != 0) { 
        perror("putenv");
        return 1;
    }

    if (time(&now) == (time_t)-1) { 
        perror("time");
        return 1;
    }
    
    printf("%s", ctime(&now));
    
    sp = localtime(&now);
    printf("%d/%d/%02d %d:%02d %s\n", sp->tm_mon + 1, sp->tm_mday, sp->tm_year,
            sp->tm_hour, sp->tm_min, tzname[sp->tm_isdst]);
    
    return 0;
}
