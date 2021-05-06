#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#define ERROR -1

int main(int argc, char* argv[])
{
    FILE* file;

    printf("\nReal user id: %d\n", getuid());
    // uid_t getuid(void) ���������� �������� id ������������ ��������.

    printf("Effective user id: %d\n", geteuid());
    // uid_t geteuid(void) ���������� ����������� id ������������ ��������.

    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("File not opened at first try");
    }
    else {
        printf("File opened at first try\n\n");
        fclose(file);
    }

    int id_check;
    id_check = seteuid(getuid());
    /* int setuid(uid_t uid) ����������� ������������ id ������������ �������� uid.
     �������� - �������� id ������������ ��������.*/

    if (id_check == ERROR)
    {
        perror("Can't set this id.");
    }

    printf("New real user id: %d\n", getuid());
    printf("New effective user id: %d\n", geteuid());

    file = fopen(argv[1], "r");

    /*��������, ����������� �� ����*/

    if (file == NULL) {
        perror("File cannot be opened");
    }
    else {
        printf("File opened successfully\n");
        fclose(file);
    }

    exit(0);
}
