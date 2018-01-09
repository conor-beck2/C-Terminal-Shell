#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>

#include "sfish.h"
#include "debug.h"
#include "builtin.h"
#include "wd.h"
#include "signals.h"

void initialSetUp(){
size_t bufSize = 3000;
    char* prompt = " :: cmbeck >>";
    char* buf = malloc(bufSize);
    char* firstDirectory = getcwd(buf, bufSize);
    while(firstDirectory == NULL){
        bufSize = bufSize * 2;
        buf = realloc(buf, bufSize);
        firstDirectory = getcwd(buf, bufSize);
    }
    //free(buf);
    actualDirectory = realloc(actualDirectory, strlen(firstDirectory) + 10);
    strcpy(actualDirectory, firstDirectory);
    int flag;
    flag = strncmp(firstDirectory, homeDirectory, strlen(homeDirectory));
   // if(flag != 0){
     //   char nextChar = *(firstDirectory + strlen(homeDirectory) + 1);
       // if(nextChar != '/'){
         //   flag = 1;
        //}

    //}
    if(flag != 0){
        //printf("one\n");
        char bufferNot[strlen(firstDirectory) + strlen(prompt) + 10];
        strcpy(bufferNot, firstDirectory);
        strcat(bufferNot, prompt);
        directory = realloc(directory, strlen(bufferNot) + 10);
        strcpy(directory, bufferNot);
        free(buf);
    }
    else{


        char* temp = firstDirectory + strlen(homeDirectory);

        char buffer[strlen(temp + 10)];
        strcpy(buffer,  "~");
        strcat(buffer, temp);
        char buffer2[strlen(buffer) + strlen(prompt) + 10];
        strcpy(buffer2, buffer);
        strcat(buffer2, prompt);
        directory = realloc(directory, strlen(buffer2) + 10);
        strcpy(directory, buffer2);
        free(buf);
    }
}

int twoP(){
    //char* temp = malloc(strlen(actualDirectory) + 1);
    //strcpy(temp, actualDirectory);
    prevDirectory = realloc(prevDirectory, strlen(actualDirectory) + 1);
    strcpy(prevDirectory, actualDirectory);
    prevSet = 0;
    char* ptr = strrchr(actualDirectory, '/');
    char* ptr2 = strchr(actualDirectory, '/');
    int success;
    if(ptr != ptr2){
        *ptr = '\0';
        actualDirectory = realloc(actualDirectory, strlen(actualDirectory) + 1);
        promptSet();
        success = chdir(actualDirectory);

    }
    else{
        char check = *(actualDirectory + 1);
        if(check != '\0'){
            *(ptr + 1) = '\0';
            success = chdir(actualDirectory);
            actualDirectory = realloc(actualDirectory, strlen(actualDirectory) + 1);
            promptSet();
        }
    }
    //promptSet();
    //directory = realloc(actualDirectory, strlen(actualDirectory) + 1);
    //int success = chdir(actualDirectory);
    if(success == -1){
        //directory = realloc(directory, strlen(temp) + 1);
       //strcpy(actualDirectory, temp);
        //free(temp);
        return -1;
    }
    //free(temp);
    return 0;
}

int prevDir(){

    if(prevSet == 1){

        return 2;
    }

    char* temp = malloc(strlen(actualDirectory) + 1);
    strcpy(temp, actualDirectory);
    actualDirectory = realloc(actualDirectory, strlen(prevDirectory) + 1);
    strcpy(actualDirectory, prevDirectory);
    prevDirectory = realloc(prevDirectory, strlen(temp) + 1);
    strcpy(prevDirectory, temp);
    int success = chdir(actualDirectory);
    promptSet();
    free(temp);
    return success;
}
int home1(){
    prevDirectory = realloc(prevDirectory, strlen(actualDirectory) + 1);
    strcpy(prevDirectory, actualDirectory);
    prevSet = 0;
    actualDirectory = realloc(actualDirectory, strlen(homeDirectory) + 1);
    strcpy(actualDirectory, homeDirectory);
    int success = chdir(actualDirectory);
    promptSet();
    return success;
}

int change(char* path){
char* check = strchr(path, '/');
int success;
if(check != NULL){
    success = chdir(path);
    if(success != 0){
        printf(BUILTIN_ERROR, "Directory not found");
        return -1;
    }
    prevDirectory = realloc(prevDirectory, strlen(actualDirectory) + 1);
    prevSet = 0;
    strcpy(prevDirectory, actualDirectory);
    actualDirectory = realloc(actualDirectory, strlen(path) + 1);
    strcpy(actualDirectory, path);
    promptSet();
    return 0;
}
else{
    char buf[strlen(actualDirectory) + strlen(path) + 5];
    strcpy(buf, actualDirectory);
    char* guy = "/";
    strcat(buf, guy);
    strcat(buf, path);
    success = chdir(buf);
    if(success != 0){
        printf(BUILTIN_ERROR, "Directory not found");
        return -1;
    }
    prevDirectory = realloc(prevDirectory, strlen(actualDirectory) + 1);
    prevSet = 0;
    strcpy(prevDirectory, actualDirectory);
    actualDirectory = realloc(actualDirectory, strlen(path) + 1);
    strcpy(actualDirectory, buf);
    promptSet();
    return 0;
    }
}

void promptSet(){
    char* prompt = " :: cmbeck >>";
    int flag = strncmp(actualDirectory, homeDirectory, strlen(homeDirectory));
    if(flag != 0){
        char bufferNot[strlen(actualDirectory) + strlen(prompt) + 2];
        strcpy(bufferNot, actualDirectory);
        strcat(bufferNot, prompt);
        directory = realloc(directory, strlen(bufferNot) + 1);
        strcpy(directory, bufferNot);
    }
    else{
        char* temp = actualDirectory + strlen(homeDirectory);
        char buffer[strlen(temp + 3)];
        strcpy(buffer,  "~");
        strcat(buffer, temp);
        char buffer2[strlen(buffer) + strlen(prompt) + 2];
        strcpy(buffer2, buffer);
        strcat(buffer2, prompt);
        directory = realloc(directory, strlen(buffer2) + 1);
        strcpy(directory, buffer2);
    }
}