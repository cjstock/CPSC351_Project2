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
    struct timeval end_time;
    struct timeval elapsed_time;

    struct timeval *shm_ptr;

    //Setup a chunk of shared memory
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, sizeof(struct timeval), 0666|IPC_CREAT);
    shm_ptr = shmat(shmid, (void*)0, 0);

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

        //Write to the shared memory segment
        gettimeofday(shm_ptr, 0);

        execvp(args[0], args);

    }
    else    //Back to parent process
    {
        //Wait for the child process to exit before continuing
        waitpid(pid, &status, 0);
        gettimeofday(&end_time, 0);

        //Calculate the run time
        timersub(&end_time, shm_ptr, &elapsed_time);

        printf("Start run time: %ld.%ld\n", shm_ptr->tv_sec, shm_ptr->tv_usec);
        printf("End run time: %ld.%ld\n", end_time.tv_sec, end_time.tv_usec);
        printf("Elapsed run time: %ld.%ld seconds\n", elapsed_time.tv_sec, elapsed_time.tv_usec);
        exit(0);
    }
}

