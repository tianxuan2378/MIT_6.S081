#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "sleep: missing argument error\n");
        exit(1);
    }
    int num = atoi(argv[1]); //the number of sleep
    if (num < 0) {
        fprintf(2, "sleep: argument negative error\n");
        exit(1);
    }
    sleep(num);
    exit(0);
}