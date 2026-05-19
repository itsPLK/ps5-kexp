#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "types.h"

extern pid_t getpid(void);
extern int dlsym(SceKernelModule handle, const char *symbol, void **addrp);
extern ssize_t read(int fd, void *buf, size_t nbytes);
extern ssize_t write(int fd, const void *buf, size_t nbytes);
extern void *mmap(void *addr, size_t len, int prot, int flags, int fd,
                  off_t offset);
extern int mprotect(void *addr, size_t len, int prot);
extern int munmap(void *addr, size_t len);
extern int socket(int domain, int type, int protocol);
extern int setsockopt(int s, int level, int optname, const void *optval,
                      socklen_t optlen);
extern int pipe2(int fildes[2], int flags);
extern int close(int fd);

#endif
