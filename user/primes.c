#include "kernel/types.h"
#include "user/user.h"

void primes(int* p)__attribute__((noreturn));

int main() {
    int p[2];
    if (pipe(p) == -1) {
        printf("Create pipeline fail!");
        exit(1);
    }
    int pid = fork();

    if (pid < 0) {
        printf("Create thread fail");
        exit(1);
    } else if (pid == 0) {
        close(p[1]);
        primes(p);
    } else {
        close(p[0]);
        for (int i = 2; i <= 280; i++) {
            if (write(p[1], &i, sizeof(i)) < 0) {
                break;
            }
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}

void primes(int* p) {
    int prime;
    if (read(p[0], &prime, sizeof(prime)) <= 0) {
        close(p[0]);
        exit(0);
    }
    printf("prime %d\n", prime);

    int p_next[2];
    if (pipe(p_next) == -1) {
        printf("Create pipeline fail!");
        exit(1);
    }

    int pid = fork();

    if (pid < 0) {
        printf("create thread fail");
        exit(0);
    } else if (pid == 0) {
        close(p[0]);
        close(p_next[1]);
        primes(p_next);
    } else {
        close(p_next[0]);
        int n;
        while (read(p[0], &n, sizeof(n)) > 0) {
            if (n % prime != 0) {
                write(p_next[1], &n, sizeof(n));
            }
        }
        close(p[0]);
        close(p_next[1]);
        wait(0);
        exit(0);
    }
}
