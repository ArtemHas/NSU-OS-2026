#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

static inline void print_id() {
    uid_t uid = getuid();
    uid_t euid = geteuid();
    printf("Real user ID: %d, Effective user id: %d\n", uid, euid);
}

static inline int open_file() {
    FILE* file = fopen("file.txt", "wt");
    if (file == NULL) {
        perror("Error when opening the file:");
        return -1;
    }
    int ret = fprintf(file, "Hello world!\n");
    int print_failed = 0;
    if (ret < 0) {
        print_failed = 1;
        perror("Error when writing to the file:");
    }
    ret = fclose(file);
    int close_failed = 0;
    if (ret == EOF) {
        close_failed = 1;
        perror("Error when closing the file:");
    }
    if (close_failed == 1 || print_failed == 1) {
        return -1;
    }
    return 0;
}

int main() {
    print_id();
    int ret = open_file();

    ret = setuid(getuid());
    if (ret == -1) {
        perror("Error when setting the effective user ID equal to user id:");
    }

    print_id();
    ret = open_file();
    if (ret == -1) {
        exit(1);
    }
    return 0;
}