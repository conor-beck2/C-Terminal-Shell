#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <unistd.h>
#include <sys/wait.h>
#include "execut.h"
#include "sfish.h"
#include "debug.h"
#include "builtin.h"
#include "wd.h"
#include "signals.h"


char* directory = NULL;
char* actualDirectory = NULL;
char* prevDirectory = NULL;
char* homeDirectory = NULL;
char* printBuf = NULL;
int pids[800];
char* names[800];
int numPids = 0;
int prevSet = 1;
int whichPip = 0;
int report = 0;
void handler1(int);
void handler2(int);
int main(int argc, char *argv[], char* envp[]) {
    //char* rightAng = malloc(1);
    char bu;
    int count = 0;
    int des;
    int des2;
    int exitStatus = 5;
    int converInt;
    //int first1 = 0;
    //char* inputBuffer2 = malloc(1);
    //int satisfied = 0;
    //int rightAngPos = 0;
    int pipDes[2];
    int pipDes2[2];
    char* dotLocation;
    pid_t pid;
    int execStatus = 5;
    int found = 1;
    int status;
    //int fileFound = 2;
    int count0 = 1;
    char* fileName;
    char* fileName2;
    char* dot2Location;
    //char* pathTemp = malloc(1);
    int builtInReturn = 100;
    char* tokens[800];
    char* paths[800];
    char* firstTokens[800];
    char* pipeCheck = NULL;
    char* r1Check = NULL;
    char* r2Check = NULL;
    char* home = getenv("HOME");
    char* pathTrue = getenv("PATH");
    char* path = malloc(strlen(pathTrue) + 10);
    strcpy(path, pathTrue);
    //printf("%s\n", path);
    homeDirectory = malloc(strlen(home) + 10);
    strcpy(homeDirectory, home);
    //strcat(homeDirectory, "/");
    directory = malloc(1);
    actualDirectory = malloc(1);
    prevDirectory = malloc(1);

    char* input;
    bool exited = false;


    signal(SIGINT, handler1);
    signal(SIGTSTP, handler2);
    //int exists = 1;
    //char spaceChecker = ';';

    paths[0] = strtok(path, ":");

    int countP = 1;
    char* pCheck = paths[0];
    //int getFi = 0;
    while(pCheck != NULL){
        paths[countP] = strtok(NULL, ":");
        //printf("%s\n", paths[countP]);
        pCheck = paths[countP];
        if (pCheck != NULL){
            //printf("%s\n", paths[countP]);
            //strcat(paths[countP], "/");
            countP = countP + 1;

            }
        }

    if(!isatty(STDIN_FILENO)) {
        // If your shell is reading from a piped file
        // Don't have readline write anything to that file.
        // Such as the prompt or "user input"
        if((rl_outstream = fopen("/dev/null", "w")) == NULL){
            perror("Failed trying to open DEVNULL");
            exit(EXIT_FAILURE);
        }

    }
    initialSetUp();

    //char* inputBuffer = malloc(1);
    //char* token;
    //char* token2;
    //printf("%s\n", directory);

    do {
        whichPip = 0;
        //exists = 1;
        //directory
        input = readline(directory);
        //continue;

        //write(1, "\e[s", strlen("\e[s"));
        //write(1, "\e[20;10H", strlen("\e[20;10H"));
        //write(1, "SomeText", strlen("SomeText"));
        //write(1, "\e[u", strlen("\e[u"));

        // If EOF is read (aka ^D) readline returns NULL
        //printf("dfasdas\n");
        if(input == NULL) {
            //printf("dfasdas\n");
            continue;
        }

        r1Check = strchr(input, '>');
        if(r1Check != NULL){
            char* r11Check = strrchr(input, '>');
            if(r11Check != r1Check){
                printf(SYNTAX_ERROR, "Improper syntax");
                continue;
            }
        }

        r2Check = strchr(input, '<');
        pipeCheck = strchr(input, '|');

        if(r2Check != NULL){
            char* r11Check = strrchr(input, '<');
            if(r11Check != r2Check){
                printf(SYNTAX_ERROR, "Improper syntax");
                continue;
            }
        }
        char first = *(input);
        if(first == '<' || first == '>' || first == '|'){
            printf(SYNTAX_ERROR, "Improper syntax");
            continue;
        }
        first = *(input + strlen(input)-1);
        if(first == '<' || first == '>' || first == '|'){
            printf(SYNTAX_ERROR, "Improper syntax");
            continue;
        }
        char inputBuffer[strlen(input) + 1];
        strcpy(inputBuffer, input);
        char inputBuffer2[strlen(input) + 1];
        strcpy(inputBuffer2, input);
        //token = strtok(input, " ");
        //token2 = strtok(NULL, input);
        firstTokens[0] = strtok(inputBuffer, "><|");
        count0 = 1;
        char* oCheck = firstTokens[0];
        while(oCheck != NULL){
            firstTokens[count0] = strtok(NULL, "><|");
            oCheck = firstTokens[count0];
            if (oCheck != NULL){
                count0 = count0 + 1;
            }
        }

        tokens[0] = strtok(inputBuffer2, " \t");

        count = 1;
        char* tCheck = tokens[0];
        while(tCheck != NULL){
            tokens[count] = strtok(NULL, " \t");

            tCheck = tokens[count];
            if (tCheck != NULL){
                count = count + 1;
            }
        }
        //printf("%s\n", tokens[1]);
        //token2 = strtok(NULL, input);
        if(strcmp(tokens[0], "exit") == 0){
            free(directory);
            free(actualDirectory);

            free(prevDirectory);
            free(homeDirectory);
            //free(inputBuffer);
            //free(printBuf);
            rl_free(input);
            exit(EXIT_SUCCESS);
        }
        if(strcmp(tokens[0], "jobs") == 0){
            for(int i = 1; i <= numPids; i++){
                printf(JOBS_LIST_ITEM, (i-1), names[i]);
            }
            continue;
        }
        if(strcmp(tokens[0], "fg") == 0){
            if(tokens[1] != NULL){
                char* sign = strchr(tokens[1], '%');
                if(sign == NULL){
                    printf(BUILTIN_ERROR, "No procentage");
                    continue;
                }
                else{
                    converInt = atoi(tokens[1] + 1);
                    //printf("%d\n", converInt);
                    if((converInt + 1) > numPids){
                        printf(BUILTIN_ERROR, "Not a job");
                        continue;
                    }
                    pid_t pidc = pids[converInt + 1];
                    //printf("%d\n", pidc);
                    kill(pidc, SIGCONT);
                    continue;
                }
            }
            else{
                printf(BUILTIN_ERROR, "No arg");
                continue;
            }
        }
        if(strcmp(tokens[0], "kill") == 0){
            if(tokens[1] == NULL){
                printf(BUILTIN_ERROR, "No arg");
                continue;
            }
            else{
                char* sign = strchr(tokens[1], '%');
                if(sign != NULL){
                    char c = *(tokens[1]);
                    if(c != '%'){
                        printf(BUILTIN_ERROR, "Improper format");
                        continue;
                    }
                    converInt = atoi((tokens[1] + 1));
                    if((converInt + 1) > numPids){
                        printf(BUILTIN_ERROR, "Not a job");
                        continue;
                    }
                    pid_t pidc = pids[converInt + 1];
                    kill(pidc, SIGKILL);
                    //int amount = numPids - (converInt - 1);
                    for(int i = (converInt + 1); i <= numPids; i++){
                        if(i != numPids){
                        pids[i] = pids[i+ 1];
                        }
                        if(i != numPids){
                            names[i] = names[i + 1];
                        }
                    }
                    numPids = numPids - 1;
                    continue;
                }
                else{
                    converInt = atoi((tokens[1]));
                    int index = -1;
                    for(int i = 1; i <= numPids; i++){
                        if(pids[i] == converInt){
                            index = i;
                        }
                    }
                    if(index == -1){
                        printf(BUILTIN_ERROR, "Not a process");
                        continue;
                    }
                    kill(converInt, SIGKILL);
                    for(int i = index; i <= numPids; i++){
                        if(i != numPids){
                        pids[i] = pids[i+ 1];
                        }
                        if(i != numPids){
                            names[i] = names[i + 1];
                        }
                    }
                    numPids = numPids - 1;
                    continue;
                }
            }
        }
        if(strcmp(tokens[0], "help") == 0 && r1Check == NULL && r2Check == NULL && pipeCheck == NULL){
            helps();
            //rl_free(input);
            continue;
        }
        if(strcmp(tokens[0], "pwd") == 0 && r1Check == NULL && r2Check == NULL && pipeCheck == NULL){
            pwdB();
            //rl_free(input);
            continue;
        }
        if(tokens[1] != NULL){
            if(strcmp(tokens[0], "cd") == 0){
                if(strcmp(tokens[1], "-") == 0){
                    builtInReturn = prevDir();
                    if(builtInReturn == -1){
                        printf(BUILTIN_ERROR, "Can't find previous directory");
                    }
                    else if(builtInReturn == 2){
                        printf(BUILTIN_ERROR, "Previous directory not yet set");
                    }
                    else if(builtInReturn == 0){
                        printf("%s\n", actualDirectory);
                    }
                    //rl_free(input);
                    continue;
                }
                else if(strcmp(tokens[1], ".") == 0){
                    prevDirectory = realloc(prevDirectory, strlen(actualDirectory) + 1);
                    strcpy(prevDirectory, actualDirectory);
                    prevSet = 0;
                    //rl_free(input);
                    continue;
                }
                else if(strcmp(tokens[1], "..") == 0){
                    twoP();
                    //rl_free(input);
                    continue;
                }
                else{
                    change(tokens[1]);
                    //rl_free(input);
                    continue;
                }
            }
        }
        else{
            if(strcmp(tokens[0], "cd") == 0){
                home1();
                //rl_free(input);
                continue;
            }
        }


        if((r1Check == NULL) && (pipeCheck == NULL) && (r2Check == NULL)){

           found = fileSearc(tokens[0], countP, paths);
           if(found != 0){
            continue;
           }
           char* ar[count + 1];
           //ar[0] = tokens[0];
           for(int i = 0; i < count; i++){
            ar[i] = tokens[i];
           }
           ar[count] = NULL;
           //printf("%d\n", count);
           //ar[count + 1] = NULL;
           //ar[2] = NULL;
           //ar[0] = "echo";
           //ar[1] = "hey";
           //ar[2] = NULL;
           numPids = numPids + 1;
           pid = fork();
           if(pid == 0){
                execStatus = execvp(ar[0], ar);
                if(execStatus != 0){
                    printf(EXEC_ERROR, "Exec function failed");
                    exit(EXIT_SUCCESS);
                }
           }
           else{
            pids[numPids] = pid;
            char* tem = malloc(strlen(input) + 1);
            strcpy(tem, input);
            names[numPids] = tem;
            waitpid(-1, &status, WUNTRACED);

           }
           exitStatus = WEXITSTATUS(status);
           if(exitStatus != EXIT_SUCCESS && report == 0){
            printf(EXEC_ERROR, "Program failed");
           }
           if(report == 0){
            numPids = numPids - 1;
           }
           if(report == 1){
            report = 0;
           }
           continue;
        }
        else if(r1Check != NULL && r2Check == NULL){
            if(count0 < 2){
                printf(SYNTAX_ERROR, "Incorrect syntax");
                //rl_free(input);
                continue;
            }

            char* tokenArgs = malloc(strlen(firstTokens[0]) + 1);
            strcpy(tokenArgs, firstTokens[0]);
            char* file1 = malloc(strlen(firstTokens[1]) + 1);
            strcpy(file1, firstTokens[1]);
            char* ar[count0];
            ar[0] = strtok(firstTokens[0], " \t");

            int counter = 1;
            char* pipingCheck = ar[0];
            while(pipingCheck != NULL){
                ar[counter] = strtok(NULL, " \t");
                pipingCheck = ar[counter];
                counter = counter + 1;
            }

            fileName = strtok(firstTokens[1], " \t");

            des = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            //for(int i = 0; i < count; i++){
                //if(strcmp(tokens[i], ">") == 0){
                    //ar[i] = NULL;
                    //rightAngPos = i;
                    //break;
                //}
                //ar[i] = tokens[i];
            //}
            if(strcmp(ar[0], "help") == 0){
                numPids = numPids + 1;
                pid = fork();
                if(pid == 0){
                    dup2(des, STDOUT_FILENO);
                    helps();
                    exit(EXIT_SUCCESS);
                }
                else{
                    pids[numPids] = pid;
                    char* tem = malloc(strlen(input) + 1);
                    strcpy(tem, input);
                    names[numPids] = tem;
                    waitpid(-1, &status, WUNTRACED);
                }
                if(report == 0){
                    numPids = numPids - 1;
                }
                report = 0;
                close(des);
                continue;
            }
            if(strcmp(ar[0], "pwd") == 0){
                numPids = numPids + 1;
                pid = fork();
                if(pid == 0){
                    dup2(des, STDOUT_FILENO);
                    pwdB();
                    exit(EXIT_SUCCESS);
                }
                else{
                    pids[numPids] = pid;
                    char* tem = malloc(strlen(input) + 1);
                    strcpy(tem, input);
                    names[numPids] = tem;
                    waitpid(-1, &status, WUNTRACED);
                }
                if(report == 0){
                    numPids = numPids - 1;
                }
                report = 0;
                close(des);
                continue;
            }
            found = fileSearc(ar[0], countP, paths);
            if(found != 0){
                close(des);
                continue;
            }
            numPids = numPids + 1;
            pid = fork();
            if(pid == 0){

                dup2(des, STDOUT_FILENO);
                execStatus = execvp(ar[0], ar);
                if(execStatus != 0){
                    printf(EXEC_ERROR, "Exec function failed");
                    exit(EXIT_FAILURE);
                }
            }
            else{
                char* tem = malloc(strlen(input) + 1);
                strcpy(tem, input);
                names[numPids] = tem;
                pids[numPids] = pid;
                waitpid(-1, &status, WUNTRACED);
            }
            exitStatus = WEXITSTATUS(status);
           if(exitStatus != EXIT_SUCCESS && report == 0){
            printf(EXEC_ERROR, "Program failed");
           }
           if(report == 0){
            numPids = numPids - 1;
           }
           if(report == 1){
            report = 0;
           }
           close(des);
           continue;
        }
        else if(r1Check == NULL && r2Check != NULL){
            if(count0 < 2){
                printf(SYNTAX_ERROR, "Incorrect syntax");
                //rl_free(input);
                continue;
            }
            char* ar[count0];
            ar[0] = strtok(firstTokens[0], " \t");
            int counter = 1;
            char* pipingCheck = ar[0];
            while(pipingCheck != NULL){
                ar[counter] = strtok(NULL, " \t");
                pipingCheck = ar[counter];
                counter = counter + 1;
            }
            fileName = strtok(firstTokens[1], " \t");
            des = open(fileName, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            if (pipe(pipDes) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
            //while(read(des, &bu, 1) > 0){
              //  write(STDIN_FILENO, &bu, 1);
            //}
            if(strcmp(ar[0], "help") == 0){
                close(pipDes[0]);
                close(pipDes[1]);
                numPids = numPids + 1;
                pid = fork();

                if(pid == 0){

                    helps();
                    exit(EXIT_SUCCESS);
                }
                else{
                    pids[numPids] = pid;
                    char* tem = malloc(strlen(input) + 1);
                    strcpy(tem, input);
                    names[numPids] = tem;
                    waitpid(-1, &status, WUNTRACED);
                }
                if(report == 0){
                    numPids = numPids - 1;
                }
                close(des);
                continue;
            }
            if(strcmp(ar[0], "pwd") == 0){
                close(pipDes[0]);
                close(pipDes[1]);
                numPids = numPids + 1;
                pid = fork();
                if(pid == 0){

                    pwdB();
                    exit(EXIT_SUCCESS);
                }
                else{
                    pids[numPids] = pid;
                    char* tem = malloc(strlen(input) + 1);
                    strcpy(tem, input);
                    names[numPids] = tem;
                    waitpid(-1, &status, WUNTRACED);
                }
                if(report == 0){
                    numPids = numPids - 1;
                }
                report = 0;
                close(des);
                continue;
            }
            found = fileSearc(ar[0], countP, paths);
            if(found != 0){
                close(des);
                continue;
            }
            numPids = numPids + 1;
            pid = fork();
            if(pid == 0){
                close(pipDes[1]);
                dup2(pipDes[0], STDIN_FILENO);
                //close(pipDes[1]);
                execStatus = execvp(ar[0], ar);
                if(execStatus != 0){
                    printf(EXEC_ERROR, "Exec function failed");
                    exit(EXIT_FAILURE);
                }
            }
            else{
                pids[numPids] = pid;
                char* tem = malloc(strlen(input) + 1);
                strcpy(tem, input);
                names[numPids] = tem;
                close(pipDes[0]);
                while(read(des, &bu, 1) > 0){
                    write(pipDes[1], &bu, 1);
                }
                close(pipDes[1]);
                waitpid(-1, &status, WUNTRACED);
            }
            exitStatus = WEXITSTATUS(status);
            if(exitStatus != EXIT_SUCCESS && report == 0){
                printf(EXEC_ERROR, "Program failed");
            }
            if(report == 0){
                numPids = numPids - 1;
            }
            if(report == 1){
                report = 0;
            }
           close(des);
           continue;

        }
        else if(r1Check != NULL && r2Check != NULL){
            if(count0 < 3){
                printf(SYNTAX_ERROR, "Incorrect syntax");
                //rl_free(input);
                continue;
            }
            if(r1Check > r2Check){
                // r1 >
                dotLocation = strchr(input, '.');
                dot2Location = strrchr(input, '.');
                if(dotLocation == NULL){
                    //error
                    printf(SYNTAX_ERROR, "Incorrect syntax");
                    //rl_free(input);
                    continue;
                }
                if(dotLocation < r2Check){
                    printf(SYNTAX_ERROR, "Incorrect syntax");
                    //rl_free(input);
                    continue;
                }
                if(dotLocation > r1Check){
                    printf(SYNTAX_ERROR, "Incorrect syntax");
                    //rl_free(input);
                    continue;
                }
                if(dotLocation == dot2Location){
                    printf(SYNTAX_ERROR, "Incorrect syntax");
                    //rl_free(input);
                    continue;
                }
                if(dot2Location < r1Check){
                    printf(SYNTAX_ERROR, "Incorrect syntax");
                    //rl_free(input);
                    continue;
                }
                if(dot2Location < r2Check){
                    printf(SYNTAX_ERROR, "Incorrect syntax");
                    //rl_free(input);
                    continue;
                }
                fileName = strtok(firstTokens[1], " \t");
                fileName2 = strtok(firstTokens[2], " \t");
                des = open(fileName2, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

                des2 = open(fileName, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

                //if (pipe(pipDes) == -1) {
                    //perror("pipe");
                    //exit(EXIT_FAILURE);
                //}
                //while(read(des2, &bu, 1) > 0){
                   // write(STDIN_FILENO, &bu, 1);
               // }
                char* ar[count0];
                ar[0] = strtok(firstTokens[0], " \t");
                int counter = 1;
                char* pipingCheck = ar[0];
                while(pipingCheck != NULL){
                    ar[counter] = strtok(NULL, " \t");
                    pipingCheck = ar[counter];
                    counter = counter + 1;
                }
                if(strcmp(ar[0], "help") == 0){
                numPids = numPids + 1;
                pid = fork();
                if(pid == 0){
                    dup2(des, STDOUT_FILENO);
                    helps();
                    exit(EXIT_SUCCESS);
                }
                else{
                    pids[numPids] = pid;
                    char* tem = malloc(strlen(input) + 1);
                    strcpy(tem, input);
                    names[numPids] = tem;
                    waitpid(-1, &status, WUNTRACED);
                }
                if(report != 1){
                    numPids = numPids - 1;
                }
                report = 0;
                close(des);
                continue;
            }
            if(strcmp(ar[0], "pwd") == 0){
                numPids = numPids + 1;
                pid = fork();
                if(pid == 0){
                    dup2(des, STDOUT_FILENO);
                    pwdB();
                    exit(EXIT_SUCCESS);
                }
                else{
                    pids[numPids] = pid;
                    char* tem = malloc(strlen(input) + 1);
                    strcpy(tem, input);
                    waitpid(-1, &status, WUNTRACED);
                }
                if(report != 1){
                    numPids = numPids - 1;
                }
                report = 0;
                close(des);
                continue;
            }
            found = fileSearc(ar[0], countP, paths);
            if(found != 0){
                close(des);
                close(des2);
                continue;
            }
                numPids = numPids + 1;
                pid = fork();
                if(pid == 0){
                    dup2(des2, STDIN_FILENO);
                    dup2(des, STDOUT_FILENO);
                    execStatus = execvp(ar[0], ar);
                    if(execStatus != 0){
                        printf(EXEC_ERROR, "Exec function failed");
                        exit(EXIT_SUCCESS);
                    }
                }
                else{
                    char* tem = malloc(strlen(input) + 1);
                    strcpy(tem, input);
                    names[numPids] = tem;
                    pids[numPids] = pid;
                    waitpid(-1, &status, WUNTRACED);
                }
                exitStatus = WEXITSTATUS(status);
                if(exitStatus != EXIT_SUCCESS && report == 0){
                    printf(EXEC_ERROR, "Program failed");
                }
                if(report != 1){
                    numPids = numPids - 1;
                }
                if(report == 1){
                    report = 0;
                }

                close(des);
                close(des2);
                continue;

            }
            else if(r2Check > r1Check){
                dotLocation = strchr(input, '.');
                dot2Location = strrchr(input, '.');
                if(dotLocation == NULL){
                    printf(SYNTAX_ERROR, "Incorrect syntax");
                   // rl_free(input);
                    continue;
                }
                if(dotLocation < r1Check){
                    printf(SYNTAX_ERROR, "Incorrect syntax");
                    //rl_free(input);
                    continue;
                }
                if(dotLocation > r2Check){
                    printf(SYNTAX_ERROR, "Incorrect syntax");
                    //rl_free(input);
                    continue;
                }
                if(dotLocation == dot2Location){
                    printf(SYNTAX_ERROR, "Incorrect syntax");
                    //rl_free(input);
                    continue;
                }
                if(dot2Location < r2Check){
                    printf(SYNTAX_ERROR, "Incorrect syntax");
                    //rl_free(input);
                    continue;
                }
                if(dot2Location < r1Check){
                    printf(SYNTAX_ERROR, "Incorrect syntax");
                    //rl_free(input);
                    continue;
                }
                fileName = strtok(firstTokens[1], " \t");
                fileName2 = strtok(firstTokens[2], " \t");
                des = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                des2 = open(fileName2, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                //while(read(des2, &bu, 1) > 0){
                    //write(STDIN_FILENO, &bu, 1);
                //}
                char* ar[count0];
                ar[0] = strtok(firstTokens[0], " \t");
                int counter = 1;
                char* pipingCheck = ar[0];
                while(pipingCheck != NULL){
                    ar[counter] = strtok(NULL, " \t");
                    pipingCheck = ar[counter];
                    counter = counter + 1;
                }
                if(strcmp(ar[0], "help") == 0){
                numPids = numPids + 1;
                pid = fork();
                if(pid == 0){
                    dup2(des, STDOUT_FILENO);
                    helps();
                    exit(EXIT_SUCCESS);
                }
                else{
                    pids[numPids] = pid;
                    char* tem = malloc(strlen(input) + 1);
                    strcpy(tem, input);
                    names[numPids] = tem;
                    waitpid(-1, &status, WUNTRACED);
                }
                if(report != 1){
                    numPids = numPids - 1;
                }
                report = 0;
                close(des);
                continue;
            }
            if(strcmp(ar[0], "pwd") == 0){
                numPids = numPids + 1;
                pid = fork();
                if(pid == 0){
                    dup2(des, STDOUT_FILENO);
                    pwdB();
                    exit(EXIT_SUCCESS);
                }
                else{
                    pids[numPids] = pid;
                    char* tem = malloc(strlen(input) + 1);
                    strcpy(tem, input);
                    names[numPids] = tem;
                    waitpid(-1, &status, WUNTRACED);
                }
                if(report != 1){
                    numPids = numPids - 1;
                }
                report = 0;
                close(des);
                continue;
            }
            found = fileSearc(ar[0], countP, paths);
            if(found != 0){
                close(des);
                close(des2);
                continue;
            }
                numPids = numPids + 1;
                pid = fork();
                if(pid == 0){
                    dup2(des, STDOUT_FILENO);
                    dup2(des2, STDIN_FILENO);
                    execStatus = execvp(ar[0], ar);
                    if(execStatus != 0){
                        printf(EXEC_ERROR, "Exec function failed");
                        exit(EXIT_FAILURE);
                    }
                }
                else{
                    pids[numPids] = pid;
                    char* tem = malloc(strlen(input) + 1);
                    strcpy(tem, input);
                    names[numPids] = tem;
                    waitpid(-1, &status, WUNTRACED);
                }
                exitStatus = WEXITSTATUS(status);
                if(exitStatus != EXIT_SUCCESS && report == 0){
                    printf(EXEC_ERROR, "Program failed");
                }
                if(report != 1){
                    numPids = numPids - 1;
                }
                if(report == 1){
                    report = 0;
                }
                close(des);
                close(des2);
                continue;

            }
        }
        else if(pipeCheck != NULL){

            if (pipe(pipDes) == -1) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
            if (pipe(pipDes2) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
            for(int i = 0; i < count0; i++){
                //printf("hi\n");
                char* ar[count0];
                ar[0] = strtok(firstTokens[i], " \t");
                int counter = 1;
                char* pipingCheck = ar[0];
                while(pipingCheck != NULL){
                    ar[counter] = strtok(NULL, " \t");
                    pipingCheck = ar[counter];
                    counter = counter + 1;
                }
                if(i == 0){
                    whichPip = 0;
                    if(strcmp(ar[0], "help") != 0 && strcmp(ar[0], "pwd") != 0){
                        found = fileSearc(ar[0], countP, paths);
                        if(found != 0){
                        //printf("yo\n");
                        continue;
                    }
                    }


                    //if(strcmp(ar[0], "help") != 0 && strcmp(ar[0], "pwd") != 0){
                        numPids = numPids + 1;
                    //}
                    pid = fork();
                    if(pid == 0){
                        close(pipDes[0]);
                        dup2(pipDes[1], STDOUT_FILENO);
                        if(strcmp(ar[0], "help") == 0){
                                //printf("dsafd\n");
                                helps();
                                exit(EXIT_SUCCESS);
                            }
                            else if(strcmp(ar[0], "pwd") == 0){
                                pwdB();
                                exit(EXIT_SUCCESS);
                            }
                            else{
                                execStatus = execvp(ar[0], ar);
                            }
                        if(execStatus != 0){
                            printf(EXEC_ERROR, "Exec function failed");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else{
                        //if(strcmp(ar[0], "help") != 0 && strcmp(ar[0], "pwd") != 0){
                            pids[numPids] = pid;
                            char* tem = malloc(strlen(input) + 1);
                            strcpy(tem, input);
                            names[numPids] = tem;
                        //}
                        close(pipDes[1]);
                        waitpid(-1, &status, WUNTRACED);
                    }

                    exitStatus = WEXITSTATUS(status);
                    if(exitStatus != EXIT_SUCCESS && report == 0){
                        printf(EXEC_ERROR, "Program failed");
                        continue;
                    }
                    if(report != 0){
                        numPids = numPids - 1;
                    }
                    report = 0;

                }
                else if(i == (count0 - 1)){
                    if(strcmp(ar[0], "help") != 0 && strcmp(ar[0], "pwd") != 0){
                        found = fileSearc(ar[0], countP, paths);
                        if(found != 0){
                        continue;
                    }
                    }

                    if(whichPip == 0){
                        //if(strcmp(ar[0], "help") != 0 && strcmp(ar[0], "pwd") != 0){
                        numPids = numPids + 1;
                        //}
                        pid = fork();
                        if(pid == 0){
                            close(pipDes[1]);
                            close(pipDes2[0]);
                            close(pipDes2[1]);

                            dup2(pipDes[0], STDIN_FILENO);
                            if(strcmp(ar[0], "help") == 0){
                                helps();
                                exit(EXIT_SUCCESS);
                            }
                            else if(strcmp(ar[0], "pwd") == 0){
                                pwdB();
                                exit(EXIT_SUCCESS);
                            }
                            else{
                                execStatus = execvp(ar[0], ar);
                            }
                            if(execStatus != 0){
                                printf(EXEC_ERROR, "Exec function failed");
                                exit(EXIT_FAILURE);
                            }
                        }
                        else{
                            //if(strcmp(ar[0], "help") != 0 && strcmp(ar[0], "pwd") != 0){
                                pids[numPids] = pid;
                            //}
                                char* tem = malloc(strlen(input) + 1);
                                strcpy(tem, input);
                                names[numPids] = tem;
                            close(pipDes2[0]);
                            close(pipDes2[1]);
                            close(pipDes[0]);
                            close(pipDes[1]);
                            waitpid(-1, &status, WUNTRACED);
                        }
                        exitStatus = WEXITSTATUS(status);
                        if(exitStatus != EXIT_SUCCESS && report == 0){
                            printf(EXEC_ERROR, "Program failed");
                            continue;
                        }
                        if(report != 1){
                            numPids = numPids - 1;
                        }
                        report = 0;
                    }
                    else{
                        numPids = numPids + 1;
                        pid = fork();
                        //if(strcmp(ar[0], "help") != 0 && strcmp(ar[0], "pwd") != 0){

                        //}
                        if(pid == 0){


                            close(pipDes2[1]);

                            dup2(pipDes2[0], STDIN_FILENO);
                            if(strcmp(ar[0], "help") == 0){
                                helps();
                                exit(EXIT_SUCCESS);
                            }
                            else if(strcmp(ar[0], "pwd") == 0){
                                pwdB();
                                exit(EXIT_SUCCESS);
                            }
                            else{
                                execStatus = execvp(ar[0], ar);
                            }
                            if(execStatus != 0){
                                printf(EXEC_ERROR, "Exec function failed");
                                exit(EXIT_FAILURE);
                            }
                        }
                        else{
                            //if(strcmp(ar[0], "help") != 0 && strcmp(ar[0], "pwd") != 0){
                            pids[numPids] = pid;
                            //}
                            char* tem = malloc(strlen(input) + 1);
                            strcpy(tem, input);
                            names[numPids] = tem;
                            close(pipDes2[0]);
                            close(pipDes2[1]);
                            close(pipDes[0]);
                            close(pipDes[1]);
                            waitpid(-1, &status, WUNTRACED);
                        }
                        exitStatus = WEXITSTATUS(status);
                        if(exitStatus != EXIT_SUCCESS && report == 0){
                            printf(EXEC_ERROR, "Program failed");
                            continue;
                        }
                        if(report != 1){
                            numPids = numPids - 1;
                        }
                        report = 0;
                    }
                }
                else{
                    if(strcmp(ar[0], "help") != 0 && strcmp(ar[0], "pwd") != 0){
                        found = fileSearc(ar[0], countP, paths);
                        if(found != 0){
                        continue;
                    }
                    }


                    if(whichPip == 0){
                        whichPip = 1;
                        //if(strcmp(ar[0], "help") != 0 && strcmp(ar[0], "pwd") != 0){
                        numPids = numPids + 1;
                        //}
                        pid = fork();
                        if(pid == 0){
                            close(pipDes2[0]);
                            dup2(pipDes[0], STDIN_FILENO);
                            dup2(pipDes2[1], STDOUT_FILENO);
                            if(strcmp(ar[0], "help") == 0){
                                helps();
                                exit(EXIT_SUCCESS);
                            }
                            else if(strcmp(ar[0], "pwd") == 0){
                                pwdB();
                                exit(EXIT_SUCCESS);
                            }
                            else{
                            execStatus = execvp(ar[0], ar);
                            }
                            if(execStatus != 0){
                                printf(EXEC_ERROR, "Exec function failed");
                                exit(EXIT_FAILURE);
                            }
                        }
                        else{
                            //if(strcmp(ar[0], "help") != 0 && strcmp(ar[0], "pwd") != 0){
                            pids[numPids] = pid;
                            char* tem = malloc(strlen(input) + 1);
                            strcpy(tem, input);
                            names[numPids] = tem;
                            //}
                            close(pipDes2[1]);
                            //close(pipDes[0]);
                            //close(pipDes[1]);
                            waitpid(-1, &status, WUNTRACED);
                        }
                        exitStatus = WEXITSTATUS(status);
                        if(exitStatus != EXIT_SUCCESS && report == 0){
                            printf(EXEC_ERROR, "Program failed");
                            continue;
                        }
                        if(report != 1){
                            numPids = numPids - 1;
                        }
                        report = 0;
                        if (pipe(pipDes) == -1) {
                            perror("pipe");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else{
                        whichPip = 0;
                        //if(strcmp(ar[0], "help") != 0 && strcmp(ar[0], "pwd") != 0){
                        numPids = numPids + 1;

                        pid = fork();
                        if(pid == 0){
                            close(pipDes[0]);
                            dup2(pipDes2[0], STDIN_FILENO);
                            dup2(pipDes[1], STDOUT_FILENO);
                            if(strcmp(ar[0], "help") == 0){
                                helps();
                                exit(EXIT_SUCCESS);
                            }
                            else if(strcmp(ar[0], "pwd") == 0){
                                pwdB();
                                exit(EXIT_SUCCESS);
                            }
                            else{
                            execStatus = execvp(ar[0], ar);
                            }
                        if(execStatus != 0){
                            printf(EXEC_ERROR, "Exec function failed");
                            exit(EXIT_FAILURE);
                        }
                        }
                        else{
                            //if(strcmp(ar[0], "help") != 0 && strcmp(ar[0], "pwd") != 0){
                            char* tem = malloc(strlen(input) + 1);
                            strcpy(tem, input);
                            names[numPids] = tem;
                            pids[numPids] = pid;

                            close(pipDes[1]);
                            waitpid(-1, &status, WUNTRACED);
                        }
                        exitStatus = WEXITSTATUS(status);
                        if(exitStatus != EXIT_SUCCESS && report == 0){
                            printf(EXEC_ERROR, "Program failed");
                            continue;
                        }
                        if(report != 1){
                            numPids = numPids - 1;
                        }
                        report = 0;
                        if (pipe(pipDes2) == -1) {
                            perror("pipe");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
            }
        }


        //for(int i = 0; i < count; i++){
            //if(getFi == 0){
               // getFi = 1;
                //fileFound = fileSearc(tokens[i], countP, paths);
                //if(fileFound == -1){
                    //continue;
                //}
           // }

       // }




        // Currently nothing is implemented
        //printf(EXEC_NOT_FOUND, input);

        // You should change exit to a "builtin" for your hw.
        //exited = strcmp(input, "exit") == 0;

        // Readline mallocs the space for input. You must free it.
        //rl_free(input);
        r1Check = NULL;
        r2Check = NULL;
        pipeCheck = NULL;
        //printf("suf\n");
    } while(!exited);

    debug("%s", "user entered 'exit'");

    return EXIT_SUCCESS;
}

void handler1(int sig){
    signal(SIGINT, handler1);
    numPids = numPids - 1;
    //exit(0);
    kill(pids[numPids + 1], SIGQUIT);

}
void handler2(int sig){
    report = 1;
    signal(SIGTSTP, handler2);
    kill(pids[numPids], SIGSTOP);
}