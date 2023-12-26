#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p[2];
    char buf[2];
    char *msg1 = "a";
    char *msg2 = "b";
    pipe(p);
    if (fork() == 0) { //child process
        if (read(p[0], buf, 1) != 1) {
            fprintf(2, "Cannot read from parent\n");
            exit(1);
        }
        close(p[0]);
        printf("%d: received ping\n", getpid());
        if (write(p[1], msg1, 1) != 1) {
            fprintf(2, "Cannot write to parent\n");
            exit(1);
        }
        close(p[1]);
        
        exit(0);
    } else {
        if (write(p[1], msg2, 1) != 1) {
            fprintf(2, "Cannot write to child\n");
            exit(1);
        }
        close(p[1]);
        wait(0);

        if (read(p[0], buf, 1) != 1){
            fprintf(2, "Can't read from child!");
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        exit(0);
    }
}