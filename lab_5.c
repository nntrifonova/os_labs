#include <stdio.h>
#include <stdlib.h>
#include <errno.h>     
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>    
#include <unistd.h>   

#define LINES_COUNT_MAX 100
#define END_LINE_NUMBER 0
#define ERROR -1
#define OPEN_ERROR -1
#define LSEEK_ERROR (off_t)-1
#define GET_LINE_ERROR -1
#define READ_LINE_ERROR -1
#define PRINT_LINE_ERROR -1
#define FILL_TABLE_ERROR -1
#define SUCCESS 0
#define FAIL -1
#define NOT_EOF 1
#define NOT_END_LINE 1
#define CONSOLE_INPUT 100
#define DEFAULT_FILE_NAME "text"

//return: lines count

int BuildLinesOffsetsTable(int fileDescriptor, long* linesOffsets, int* linesLengths)
{
    int current_line = 0;
    int current_pos_in_line = 0;

    linesOffsets[1] = 0L;

    while (NOT_EOF)
    {
        char current_char;
        int read_check_error = read(fileDescriptor, &current_char, 1);

        if (read_check_error == ERROR)
        {
            perror("Reading Error");
            return FILL_TABLE_ERROR;
        }

        if (read_check_error == SUCCESS)
        { 
            break;
        }

        if (current_line > LINES_COUNT_MAX + 1)   
        {
            fprintf(stderr, "Error: file have more than %d lines\n", LINES_COUNT_MAX);
            return FILL_TABLE_ERROR;
        }

        current_pos_in_line++;

        if (current_char != '\n')
        {
            continue;
        }

        linesLengths[current_line] = current_pos_in_line;
        ++current_line;

        int currentLineOffset = lseek(fileDescriptor, 0L, SEEK_CUR);

        if (currentLineOffset == LSEEK_ERROR)
        {
            perror("Lseek Error");
            return FILL_TABLE_ERROR;
        }

        linesOffsets[current_line] = currentLineOffset;
        current_pos_in_line = 0;
    }

    return current_line;
}


int PrintLine(int fileDescriptor, int lineOffset, int linesLength)         //FIX   
{
    int currentLineOffset = lseek(fileDescriptor, lineOffset, SEEK_SET); // перемещение текущ. позиции чтения на lineOffset
    if (currentLineOffset == LSEEK_ERROR) {
        perror("Lseek Error");
        return READ_LINE_ERROR;
    }

    for (int i = 0; i < linesLength; ++i)
    {
        char currentChar;

        if (read(fileDescriptor, &currentChar, 1) == ERROR)
        {
            perror("Reading Error");
            return READ_LINE_ERROR;
        }

        if (write(STDOUT_FILENO, &currentChar, 1) == ERROR)
        {
            perror("Writing Error");
            return PRINT_LINE_ERROR;
        }
    }
    return SUCCESS;
}


int RequestingUserInputLoop(int fileDescriptor, int linesCount, long* linesOffsets, int* linesLengths)
{
    printf("%s", "Enter a line \n");

    while (NOT_END_LINE)
    {
        int lineNumber;
        char console_input[CONSOLE_INPUT];
        char* end_ptr = NULL;

        int check_read = read(STDIN_FILENO, console_input, CONSOLE_INPUT);
        if (check_read == ERROR) {
            perror("Reading error");
            return GET_LINE_ERROR;
        }
        lineNumber = strtoll(console_input, &end_ptr, 0);

        if (lineNumber == ERROR) {
            perror("Getting number error");
            return GET_LINE_ERROR;
        }

        if (lineNumber == END_LINE_NUMBER)
        {
            return SUCCESS;
        }

        if (lineNumber < 0 || lineNumber > linesCount)
        {
            perror("Out of range");
            continue;
        }

        int print_check = PrintLine(fileDescriptor, linesOffsets[lineNumber-1], linesLengths[lineNumber-1]);
        if (print_check == READ_LINE_ERROR || print_check == PRINT_LINE_ERROR) {
            return GET_LINE_ERROR;
        }
    }
    return SUCCESS;
}


int main(int argc, char* argv[])
{
    const char* file_name = (argc == 2) ? argv[1] : DEFAULT_FILE_NAME;

    //таблица отступов в файле
    long linesOffsets[LINES_COUNT_MAX];
    //длины строк для каждой строки в файле
    int linesLengths[LINES_COUNT_MAX];

    //файл, на имя которого указывает file_name, открывается для чтения
    //возвращаемое значение - файловый дескриптор
    int fileDescriptor = open(file_name, O_RDONLY);
    if (fileDescriptor == OPEN_ERROR)
    {
        perror("Can't open file");
        return FAIL;
    }


    if (fileDescriptor == ERROR)
    {
        perror("Can't open the file");
        return EIO;
    }

    int linesCount = BuildLinesOffsetsTable(fileDescriptor, linesOffsets, linesLengths);

    if (linesCount == FILL_TABLE_ERROR) {
        printf("Error with filling the table");
        return FAIL;
    }

    int inputCheck = RequestingUserInputLoop(fileDescriptor, linesCount, linesOffsets, linesLengths);
    if (inputCheck == GET_LINE_ERROR) {
        printf("Error with printing lines");
        return FAIL;
    }

    int closeCheck = close(fileDescriptor);

    if (closeCheck == ERROR)
    {
        perror("Error with closing the file");
        return FAIL;
    }

    return SUCCESS;
}