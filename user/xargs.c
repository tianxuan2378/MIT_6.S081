#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_ARG_LEN 1024
int main(int argc, char* argv[]) {
    char arg[MAX_ARG_LEN], *args[MAXARG];
    char buf;
    int n, arg_index = 0;

    if (argc < 2) {
        fprintf(2, "Usage: xargs <command>\n");
        exit(1);
    }

    //read arguments after xargs
    for (int i = 1; i < argc; i++) {
        args[i - 1] = argv[i]; //command after xargs
    }

    //read arguments from stdin
    while ((n = read(0, &buf, 1)) > 0) {
        if (buf == '\n') {
            arg[arg_index] = 0; //add "\0" in the end
            if (fork() == 0) {
                args[argc - 1] = arg;
                args[argc] = 0; // //add "\0" in the end
                exec(args[0], args);
            } else {
                wait(0);
                arg_index = 0;
            }
        } else {
            arg[arg_index++] = buf;
        }
    }
    exit(0);
}