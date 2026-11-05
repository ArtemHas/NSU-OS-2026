#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define LEN 10

void makeNewlinesCount(int fd, off_t** newlinesBuffer, int* linesCount, int* capacity) {
    int nextLine = 2;
    ssize_t charCount = 0;
    char* buffer = (char*)malloc(sizeof(char) * LEN);

    if (buffer == NULL) {
        perror("malloc() caused an error - buffer ");
        exit(EXIT_FAILURE);
    }

    ssize_t charsRead;
    while ((charsRead = read(fd, buffer, LEN)) != 0) {
        charCount += charsRead;
        for (ssize_t i = 0; i < charsRead; i++) {
            if (nextLine >= (*capacity)) {
                (*newlinesBuffer) = (off_t*)realloc(*newlinesBuffer, sizeof(off_t) * (*capacity) * 2);
                (*capacity) *= 2;
            }
            if (buffer[i] == '\n') {
                (*linesCount)++;
                (*newlinesBuffer)[nextLine++] = charCount - (charsRead - i) + 1;
            }
        }
    }

    if (charsRead < 0) {
        perror("read() caused an error - charsRead ");
        exit(EXIT_FAILURE);
    }
    free(buffer);
}

int main(int argc, char* argv[]) {
    int fd;
    off_t* linesBuffer = (off_t*)calloc(LEN, sizeof(off_t));
    int capacity = LEN;

    if (argc < 2) {
        printf("You should give me a text file\n");
        exit(EXIT_FAILURE);
    }

    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror("Could not open the file ");
        exit(EXIT_FAILURE);
    }

    int linesCount = 0;
    makeNewlinesCount(fd, &linesBuffer, &linesCount, &capacity);

    while (1) {
        printf("Enter the line number - ");
        
        int line;
        int c; 
        if ((scanf("%d", &line) != 1) || ((c = getchar()) != '\n')) {
            printf("Could not read the number of the line\n");
            while ((c = getchar()) != '\n') {
                continue;
            }
            continue;
        }

        if (line < 0 || linesCount < line) {
            printf("There's no such line\n");
            continue;
        }

        if (line == 0) {
            break;
        }

        off_t offset = linesBuffer[line];
        if (lseek(fd, offset, SEEK_SET) == -1) {
            perror("lseek() caused an error");
            exit(EXIT_FAILURE);
        }

        ssize_t len = linesBuffer[line + 1] - linesBuffer[line];
        char* readLineBuffer = (char*)malloc(sizeof(char) * (len + 1)); // +1 for \0
        ssize_t bytesRead = 0;
        while (bytesRead < len) {
            bytesRead += read(fd, readLineBuffer, len - bytesRead);
            if (bytesRead == -1) {
                perror("read() caused an error - tried to read a line");
                exit(EXIT_FAILURE);
            }
        }

        readLineBuffer[len - 1] = '\0';
        printf("%s\n", readLineBuffer);
        free(readLineBuffer);

    }

    free(linesBuffer);
    close(fd);
    exit(EXIT_SUCCESS);

}