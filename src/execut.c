#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <sys/stat.h>
#include "sfish.h"
#include "debug.h"
#include "builtin.h"
#include "wd.h"
#include "execut.h"

int exe(char* arg[]){

    return 1;
}
int fileSearc(char* tok, int countP, char* paths[]){

    int exists = 1;
    int found = 1;
    struct stat st;
    char* contains = strchr(tok, '/');
    if(contains != NULL){
        exists = (stat(tok, &st) == 0);
        if(exists != 0){
            printf(EXEC_NOT_FOUND, tok);
            return -1;
        }
        else{
            return 0;
        }
    }
    else{

       // printf("%d\n", countP);
        for(int j = 0; j < countP; j++){
            //printf("%s\n", paths[j]);
            char pathBuffer[strlen(tok) + strlen(paths[j]) + 4];
            strcpy(pathBuffer, paths[j]);
            strcat(pathBuffer, "/");
            strcat(pathBuffer, tok);

            exists = (stat(pathBuffer, &st));
            if(exists == 0){
                found = 0;
            }
        }
        if(found != 0){
            found = 1;
            printf(EXEC_NOT_FOUND, tok);
            return -1;
            }
        else{
            return 0;
        }
    }
}