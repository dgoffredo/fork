// Based off of:
// <https://github.com/nginx/nginx/blob/a64190933e06758d50eea926e6a55974645096fd/src/os/unix/ngx_daemon.c>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>

extern "C" {
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
}

void before_fork();
void after_fork_parent();
void after_fork_child();

const char file[] = "/etc/file";

int main() {
    before_fork();

    switch (int child_pid = fork()) {
        case -1:
            std::perror("fork() failed");
            return 1;
        case 0:
            after_fork_child();
            return 0;
        default:
            after_fork_parent();
            waitpid(child_pid, nullptr, 0);
    }
}

void before_fork() {
    std::ifstream stream{"/etc/file"};
    if (std::ifstream(file)) {
        std::cerr << "I am the parent before fork.  The file stream is good." << std::endl;
    } else {
        std::perror("I am the parent before fork.  The file stream is bad");
    }
}


void after_fork_parent() {
    std::ifstream stream{"/etc/file"};
    if (std::ifstream(file)) {
        std::cerr << "I am the parent after fork.  The file stream is good." << std::endl;
    } else {
        std::perror("I am the parent after fork.  The file stream is bad");
    }
}

void after_fork_child() {
    if (setsid() == -1) {
        std::perror("I am the child after fork.  setid() returned error:");
        return;
    }

    umask(0);

    std::ifstream stream{"/etc/file"};
    if (std::ifstream(file)) {
        std::cerr << "I am the child after fork.  The file stream is good." << std::endl;
    } else {
        std::perror("I am the child after fork.  The file stream is bad");
    }
}


