#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

#define DIRNAME "."
#define OFNAME  "out.log"


int main(int argc, char** argv) {
    DIR* d;
    FILE*fp;
    char cwd[512];
    struct dirent *dir;
    if (argv[1] == NULL) {
        printf("Error: extension is empty!\n");
        exit(0);
    }
    d = opendir(DIRNAME);
    if ((fp = fopen(OFNAME, "w+")) == NULL) {
        printf("Error: file log not find!\n");
        exit(0);
    }
    fprintf(fp, "FIND FILE WITH EXTENSION %s\n", argv[1]);
    do {
        d = opendir(DIRNAME);
        int flag = 0;
        if (getcwd(cwd, sizeof(cwd)) != NULL);
        fprintf(fp, "Directory: %s\n", cwd);
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (strstr(dir->d_name, argv[1]) != NULL && dir->d_name[0] != '.') {
                    flag = 1;
                    fprintf(fp, "File: %s\n", dir->d_name);
                }
            }
            if (flag == 0) {
                fprintf(fp, "FILE WITH %s NOT FOUND\n", argv[1]);
            }
            chdir("..");
            fprintf(fp, "\n\n");
            closedir(d);
        }
    } while ((int)cwd[1] != 0);
    fclose(fp);
    return 0;
}