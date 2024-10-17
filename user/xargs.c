#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#include "kernel/fs.h"

#define BUF_SIZE 512

int main(int argc, char* argv[]) {
    char buf[BUF_SIZE];
    char* cmd[MAXARG];
    int i, n, pid;
    int cmd_argc = argc - 1;
    // xargs echo => cmd_argc = 1
    // Copy the command arguments
    // xargs echo
    for (i = 0; i < cmd_argc; i++) {
        cmd[i] = argv[i + 1];
    }
    while ((n = read(0, buf, sizeof(buf))) > 0) {
        int start = 0;
        for (i = 0; i < n; i++) {
            if (buf[i] == '\n') {
                buf[i] = '\0';
                cmd[cmd_argc] = &buf[start]; //his sets the cmd[cmd_argc] to point to the start of the current argument in the buffer buf.
                cmd[cmd_argc + 1] = 0; // marking the end of the arguments array for the exec function.

                //This line creates a new process by calling fork(). The fork() function returns 0 
                // to the child process and the child's PID to the parent process.
                if ((pid = fork()) == 0) {
                    exec(cmd[0], cmd);
                    printf("exec %s failed\n", cmd[0]);
                    exit(1);
                } else if (pid > 0) {
                    wait(0);  //The parent process waits for the child process to finish.

                } else { //If fork() returns a negative value, it means the fork call failed.
                    printf("fork failed\n");
                    exit(1);
                }

                start = i + 1;
            }
        }
    }

    if (n < 0) {
        printf("read error\n");
        exit(1);
    }

    exit(0);
}
