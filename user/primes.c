#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void filterPrimes(int *pipeContent) { //pipeContent is the pipe from the parent process
    int prime, num;
    
    //read the first number in the pipe and make it the prime (it must be a prime)
    close(pipeContent[1]);
    if(read(pipeContent[0], (void *)&prime, sizeof(prime)) != sizeof(prime)){
        fprintf(2, "Read fail!\n");
        exit(1);
    }
    printf("prime %d\n", prime);

    int p[2];
    pipe(p);
    if (read(pipeContent[0], (void *)&num, sizeof(num))) {
        if (fork() == 0) {
            close(p[1]);
            close(pipeContent[0]);
            filterPrimes(p);
        } else {
            close(p[0]);
            do {
                if (num % prime != 0) {
                    write(p[1], (void *)&num, sizeof(num));
                }
            } while (read(pipeContent[0], (void *)&num, sizeof(num)));
            close(pipeContent[0]);
            close(p[1]);
            wait(0);
        } 
    }
    exit(0);
}

int 
main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    if (fork() == 0) {
        filterPrimes(p);
        exit(0);
    } else {
        close(p[0]);
        int num = 2;
        while (num <= 35) {
            if (write(p[1], (void *)&num, sizeof(num)) != sizeof(int)) {
                fprintf(2, "Cannot write to the pipe");
                exit(1);
            }
            num++;
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}