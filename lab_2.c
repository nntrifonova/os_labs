#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char* tzname[];

main()
{
    time_t now;
    //кол-во секунд от 00:00:00 UTC 1 января 1970
    struct tm* struct_pointer;
    //структура содержащая календарное время


    if (putenv("TZ=America/Los_Angeles"))
    {
        perror("Not enough memory.");
    };

    // функция  int putenv(char*) кладет указатель на строку в extern char** environ.
    // putenv возвращает 0 в случае успеха

    (void)time(&now);


    printf("%s", ctime(&now));

    /*  char* ctime(time_t* now) преобразует now в строку формата date и возвращает указатель на нее. 
      Корректировка временного пояса происходит перед генерацией строки.*/

    struct_pointer = localtime(&now);
   
    /* struct tm* localtime(time_t clock) заполняет значениями поля структуры tm.
    1) вызывает void tzset(void), которая по информации в TZ переписывает внешние переменные
    2) также tzset инициализирует tzname[]
    3) конвертирует clock в поля структуры tm и возвращает на нее указатель*/

    if (struct_pointer == NULL)
    {
        perror("Can't print info.");
    }

    printf("%d/%d/%02d %d:%02d %s\n",
        struct_pointer->tm_mon + 1, struct_pointer->tm_mday,struct_pointer->tm_year,
        struct_pointer->tm_hour, struct_pointer->tm_min, tzname[struct_pointer->tm_isdst]);

    return 0;
}