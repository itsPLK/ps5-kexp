#ifndef LOGGER_H
#define LOGGER_H

#define STDOUT_FD 1
#define STDERR_FD 2

int logger_init();
void log(const char *fmt, ...);
void notify(const char *fmt, ...);

#endif