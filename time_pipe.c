#include <sys/time.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main(int argc, char* argv[])
{
    //Create a copy of argv starting from index 1
    char** args = argv + 1;
    int argsc = argc - 1;

    struct timeval start_time;
    struct timeval end_time;
    struct timeval elapsed_time;

    struct timeval *shm_ptr;

    int p[2];
    int FILESIZE = sizeof(struct timeval);

    if (pipe(p) < 0)
    {
        exit(1);
    }

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
        write(p[1], &start_time, FILESIZE);

        execvp(args[0], args);

    }
    else    //Back to parent process
    {
        //Wait for the child process to exit before continuing
        waitpid(pid, &status, 0);
        gettimeofday(&end_time, 0);

        //Calculate the run time
        read(p[0], &start_time, FILESIZE);
        timersub(&end_time, &start_time, &elapsed_time);

        printf("Start run time: %ld.%ld\n", start_time.tv_sec, start_time.tv_usec);
        printf("End run time: %ld.%ld\n", end_time.tv_sec, end_time.tv_usec);
        printf("Elapsed run time: %ld.%ld seconds\n", elapsed_time.tv_sec, elapsed_time.tv_usec);
        exit(0);
    }
}


