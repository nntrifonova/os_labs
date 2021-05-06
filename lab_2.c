#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char* tzname[];

main()
{
    time_t now;
    //���-�� ������ �� 00:00:00 UTC 1 ������ 1970
    struct tm* struct_pointer;
    //��������� ���������� ����������� �����


    if (putenv("TZ=America/Los_Angeles"))
    {
        perror("Not enough memory.");
    };

    // �������  int putenv(char*) ������ ��������� �� ������ � extern char** environ.
    // putenv ���������� 0 � ������ ������

    (void)time(&now);


    printf("%s", ctime(&now));

    /*  char* ctime(time_t* now) ����������� now � ������ ������� date � ���������� ��������� �� ���. 
      ������������� ���������� ����� ���������� ����� ���������� ������.*/

    struct_pointer = localtime(&now);
   
    /* struct tm* localtime(time_t clock) ��������� ���������� ���� ��������� tm.
    1) �������� void tzset(void), ������� �� ���������� � TZ ������������ ������� ����������
    2) ����� tzset �������������� tzname[]
    3) ������������ clock � ���� ��������� tm � ���������� �� ��� ���������*/

    if (struct_pointer == NULL)
    {
        perror("Can't print info.");
    }

    printf("%d/%d/%02d %d:%02d %s\n",
        struct_pointer->tm_mon + 1, struct_pointer->tm_mday,struct_pointer->tm_year,
        struct_pointer->tm_hour, struct_pointer->tm_min, tzname[struct_pointer->tm_isdst]);

    return 0;
}