#include<stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#define FORK_ERROR -1
#define EXEC_ERROR -1
#define WAIT_ERROR -1
#define CHECK_INIT 0
#define CHILD_PROCESS 0
#define PARENT_PROCESS 1
#define END_OF_ARGS NULL

int main(int argc, char* argv[])
{
    pid_t fork_check = CHECK_INIT;
    int status = 0;
    pid_t wait_check = CHECK_INIT;

    if (argc != 2)
    {
        printf("Usage: a.out f1\n");
        return EXIT_FAILURE;
    }

    fork_check = fork();
    if (fork_check == FORK_ERROR)
    {
        perror("Error while creating process");
        return EXIT_FAILURE;
    }

    if (fork_check == CHILD_PROCESS)
    {
        printf("\nIt is a new process\nPID: %d\nPPID:%d\n", getpid(), getppid());
        int execl_check = CHECK_INIT;
        execl_check = execl("/bin/cat", "cat", argv[1], END_OF_ARGS);

        if (execl_check == EXEC_ERROR)
        {
            perror("Error while openning cat");
            return EXIT_FAILURE;
        }
    }
    if (fork_check >= PARENT_PROCESS)
    {
        printf("It is a parent\nchild's PID: %d\nPID: %d\n", fork_check, getpid());
    }

    wait_check = wait(&status);
    if (wait_check == WAIT_ERROR)
    {
        perror("Error while waiting child determination");
        return EXIT_FAILURE;
    }

    printf("\nWait ended.\n");

    return EXIT_SUCCESS;
}