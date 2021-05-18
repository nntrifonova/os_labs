#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <glob.h>

#define SIZE 256
#define NOT_END_OF_INPUT 1
#define ERROR -1
#define SUCCESS 0
#define SLASH_FOUND -1

int slashCheck(char currentSymbol) {
    if (currentSymbol == '/') {
        fprintf(stderr, "Symbol '/' is not allowed\n");
        return SLASH_FOUND;
    }
}


int readPattern(char* pattern) {
    printf("Enter pattern: ");
    int patternLength = 0;
    int checkSlash = 0;
    while (NOT_END_OF_INPUT) {
        char currentSymbol = (char)fgetc(stdin);
        if (currentSymbol == EOF || currentSymbol == '\n') {
            break;
        }
        checkSlash = slashCheck(currentSymbol);
        if (checkSlash == SLASH_FOUND) {
            return ERROR;
        }
        pattern[patternLength] = currentSymbol;
        patternLength++;
    }
    if (patternLength == 0) {
        fprintf(stderr, "Wrong input. The pattern can't be empty\n");
        return ERROR;
    }
    pattern[patternLength] = '\0';
    return EXIT_SUCCESS;
}

int print(char* pattern) {
    glob_t result;
    int flag = glob(pattern, GLOB_NOCHECK, NULL, &result);
    if (flag != SUCCESS) {
        perror("glob error");
        globfree(&result);
        return ERROR;
    }
    for (int i = 0; i < result.gl_pathc; i++) {
        printf("%s\n", result.gl_pathv[i]);
    }
    globfree(&result);
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    char pattern[SIZE];
    if (readPattern(pattern) == ERROR) {
        exit(EXIT_FAILURE);
    }
    if (print(pattern) == ERROR) {
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}