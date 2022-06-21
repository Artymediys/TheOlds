#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <sys/statvfs.h>

// for exit
int shell_active = 1;

#define SHCMD(x) int shcmd_##x (char* cmd, int paramCnt, char* params[])
#define SHCMD_EXEC(x) shcmd_##x (params[0], np, params)
#define IS_CMD(x) strncmp(#x,command,strlen(command)) == 0

char* envVar(char* inputData) {
    if(inputData[0] != '$') {
        return inputData;
    }

    char* result = getenv(inputData + 1);
    if (!result) {
        return inputData;
    } else {
        return result;
    }
}

SHCMD(pwd) {
    printf("%s\n", getenv("PWD"));
    return 0;
}

SHCMD(exit) {
    shell_active = 0;
    printf("Bye, bye!\n");
    return 0;
}

SHCMD(ls) {
    struct dirent *file;
    DIR *dir = opendir(".");

    while ((file = readdir(dir)) != NULL) {
        printf("%s\n", file->d_name);
    }
    closedir(dir);
    return 0;
}

SHCMD(ps) {
    execvp(params[0], params);
    return 0;
}

SHCMD(cd) {
    if (chdir(envVar(params[1])) == -1) {
        perror("Error changing directory!");
    }
    return 0;
}

SHCMD(link) {
    int cnd = 0, sFlag = 0;
    while((cnd = getopt(paramCnt, params, "s")) != -1) {
        if (cnd == 's') {
            sFlag = 1;
        }
    }

    char* path1 = envVar(params[1]);
    char* path2;
    if(params[2] == NULL) {
        path2 = ".";
    } else {
        path2 = envVar(params[2]);
    }

    int result = 0;
    if(sFlag) {
        result = symlink(path1, path2);
    }
    else {
        result = link(path1, path2);
    }

    if(result == -1) {
        perror("Could not link files");
        return 0;
    }

    printf("Created a link\n");
    return 0;
}

SHCMD(cp) {
    char* path1 = envVar(params[1]);
    char* path2 = envVar(params[2]);

    int fd1 = open(path1, O_RDONLY);
    if(fd1 == -1) {
        perror("Could not open file!");
        return 0;
    }

    int fd2 = open(path2, O_CREAT | O_WRONLY | S_IRUSR | S_IWUSR | O_EXCL, 0666);
    if(fd2 == -1) {
        perror("Could not create file!");
        return 0;
    }

    char buf[1024];
    int len;
    while ((len = read(fd1, buf, 1024)) != 0) {
        write(fd2, buf, len);
    }

    return 0;
}

SHCMD(df) {
    execvp(params[0], params);
    return 0;
}

SHCMD(head) {
    int fd = 0;
    if (params[1] != NULL){
        if ((fd = open(envVar(params[1]), O_RDONLY) == -1)){
            perror("Could not open file!");
            return 0;
        }
    }

    char buf[1024];
    if(read(fd, buf, 1024) == -1) {
        perror("Could not read file");
        return 0;
    }

    int n = 0;
    char* token = strtok(buf, "\n");
    while(token != NULL) {
        ++n;
        printf("%s\n", token);
        token = strtok(NULL, "\n");
        if(n == 10) { break; }
    }
    return 0;
}

void execs(char* command) {
    int np = 0;
    char *params[256];
    char *token = strtok(command, " ");;

    while(token && np < 255) {
        params[np++] = token;
        token = strtok(NULL, " ");
    }
    params[np] = NULL;

    if(IS_CMD(pwd)) {
        SHCMD_EXEC(pwd);
    } else if(IS_CMD(ls)) {
        SHCMD_EXEC(ls);
    } else if(IS_CMD(ps)) {
        SHCMD_EXEC(ps);
    } else if(IS_CMD(cd)) {
        SHCMD_EXEC(cd);
    } else if(IS_CMD(link)) {
        SHCMD_EXEC(link);
    } else if(IS_CMD(cp)) {
        SHCMD_EXEC(cp);
    } else if(IS_CMD(exit)) {
        SHCMD_EXEC(exit);
    } else {
        execvp(params[0], params);
        perror("exec");
    }
}

int convExec(char* commands[], int count, int current) {
    int fd[2];
    if (pipe(fd) < 0 ) {
        printf("Cannot create pipe\n");
        return 1;
    }

    if (count > 1 && current < count - 2) {
        // first (count - 2) commands
        if (fork() == 0) {
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            execs(commands[current]);
            exit(0);
        }

        if (fork() == 0) {
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);
            convExec(commands, count, ++current);
            exit(0);
        }
    } else { // two last commands or if only one command
        if((count == 1 && (strncmp(commands[0], "exit", 4) == 0)) || (strncmp(commands[0], "cd", 2) == 0)) {
            // no fork for exit or cd
            close(fd[0]);
            close(fd[1]);
            execs(commands[current]);
            return 0;
        }

        if (fork() == 0) {
            if (count > 1 ) {
                // if more than one command
                dup2(fd[1], 1);
            }
            close(fd[0]);
            close(fd[1]);
            execs(commands[current]);
            exit(0);
        }

        if (count > 1 && fork() == 0 ) {
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);
            execs(commands[current + 1]);
            exit(0);
        }
    }

    close(fd[0]);
    close(fd[1]);

    for (int i = 0 ; i < count ; i++) {
        wait(0);
    }
    return 0;
}

int main() {
    char commandline[1024];
    char *p, *commands[256], *token;

    while(shell_active) {
        printf("[%s]# ",getenv("PWD"));
        fflush(stdout);
        fgets(commandline, 1024, stdin);

        if ((p = strstr(commandline, "\n")) != NULL) {
            p[0] = 0;
            token = strtok(commandline, "|");
        }

        int command_count = 0;
        for (; token && command_count < 256; command_count++) {
            commands[command_count] = strdup(token);
            token = strtok(NULL, "|");
        }

        commands[command_count] = NULL;

        if (command_count > 0) {
            convExec(commands, command_count, 0);
        }
    }
    return 0;
}
