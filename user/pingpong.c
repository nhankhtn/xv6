#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int p1[2], p2[2];
    char buf[1];
    int pid;

    // Tạo hai pipe
    pipe(p1);
    pipe(p2);

    if ((pid = fork()) == 0) {
        // Tiến trình con
        read(p1[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(p2[1], buf, 1);
        exit(0);
    } else {
        // Tiến trình cha
        write(p1[1], "x", 1);
        read(p2[0], buf, 1);
        printf("%d: received pong\n", getpid());
        wait(0);
        exit(0);
    }
}

