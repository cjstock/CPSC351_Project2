#include <sys/time.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char* argv[])
{
    //Create a copy of argv starting from index 1
    char** args = argv + 1;
    int argsc = argc - 1;

    struct timeval start_time;
    // int shmid = shmget()
    struct timeval end_time;
    struct timeval elapsed_time;
    pid_t pid, w;
    int status;

    pid = fork();

    if (pid < 0)    //Couldn't create a child process
    {
        fprintf(stderr, "Fork failed!\n");
        exit(-1);
    }
    else if (pid == 0)  //Inside child process
    {

        gettimeofday(&start_time, 0);

        execvp(args[0], args);

    }
    else    //Back to parent process
    {
        //Wait for the child process to exit before continuing
        waitpid(pid, &status, 0);
        gettimeofday(&end_time, 0);

        timersub(&end_time, &start_time, &elapsed_time);

        printf("Start run time: %ld.%ld\n", start_time.tv_sec, start_time.tv_usec);
        printf("End run time: %ld.%ld\n", end_time.tv_sec, end_time.tv_usec);
        printf("Elapsed run time: %ld.%ld\n", elapsed_time.tv_sec, elapsed_time.tv_usec);
        exit(0);
    }
}

