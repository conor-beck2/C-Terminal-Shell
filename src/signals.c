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
#include "sfish.h"
#include "debug.h"
#include "builtin.h"
#include "wd.h"
#include "signals.h"

//void handler1(int sig){
    //signal(SIGINT, handler1);
    //kill(pids[numPids], SIGKILL);
//}