#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>

#include "sfish.h"
#include "debug.h"
#include "builtin.h"

void helps(){
    printf("Built in commands:\nhelp displays help menu\nexit exits the shell\ncd [path] changes the directory to the specified path\ncd - returns to the previous directory\ncd [no args] returns to the home directory\ncd . refers to the current directory\ncd .. goes up a directory\npwd shows the current directory\n");
}

void pwdB(){
    printf("%s\n", actualDirectory);
}