#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#define MASSAGE_SIZE 40
int flag = 1;

void s2() {
    printf("PROCESS DEAD!\n");
}

void s1() {
    flag = 0;
}

void Cowsay() {
  printf("     \\  ^__^\n");
  printf("      \\ (oo)\\_____\n");
  printf("        (__)\\     )\\/\\\n");
  printf("           ||----w |\n");
  printf("           ||     ||\n");
}

int main() {
    int fd[2];
    pid_t pid1, pid2, pid;
    char MAS[MASSAGE_SIZE];
    long int id, id_p;
    int status;
    signal(SIGUSR1, s1);
    signal(SIGCHLD, s2);
    if (pipe(fd) == -1) {
        perror("Pipe error!");
        exit(0);
    }
    printf("HELLO!!!! THIS PROGRAMM VIEW CONTROL PROCESS FUCNTION!\n");
    Cowsay();
    printf("P0 START\n");
    pid = fork();
    if (pid == 0) {
        printf("P1 CREATED!\n");
        pid = fork();
        if (pid == 0) {
            printf("P2 CREATED\n");
            printf("P2: WAIT SIGNAL FROM P1\n");
            while(flag) pause;
            strcpy(MAS, "HELLO FROM PROCESS P2!!");
            id = getpid();
            printf("P2: SENT MASSAGE IN K1\n");
            write(fd[1], &id, sizeof(id));
            write(fd[1], MAS, sizeof(MAS));
            pid1 = getppid();
            kill(pid1, SIGUSR1);
            printf("P2: SENT SIGNAL TO P1\n");
            printf("P2 FINISHED\n");
            exit(0);
        } else {
            pid2 = pid;
            id = getpid();
            printf("P1: SENT MASSAGE IN K1!\n");
            strcpy(MAS, "HELLO FROM PROCESS P1!!");
            write(fd[1], &id, sizeof(id));
            write(fd[1], MAS, sizeof(MAS));
            printf("P1: SENT SIGNAL FOR P2\n");
            kill(pid2, SIGUSR1);
            printf("P1: WAIT SIGNAL FROM P2\n");
            while(flag) pause;
            strcpy(MAS, "HELLO FROM PROCESS P1 AGAIN!!");
            printf("P1: SENT MASSAGE IN K1\n");
            write(fd[1], &id, sizeof(id));
            write(fd[1], MAS, sizeof(MAS));
            wait(&status);
            printf("P1 FINISHED!\n");
            exit(0);
        }
    } else {
        pid1 = pid;
        printf("P0: READING MASSAGE\n");
        for (int i = 0; i < 3; i++) {
            read(fd[0], &id, sizeof(id));
            read(fd[0], &MAS, sizeof(MAS));
            printf("P0: MASSAGE: %s, ID:, %ld\n", MAS, id);
        }
    }
    wait(NULL);
    printf("P0 FINISHED!\n");
    return 0;
}