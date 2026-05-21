#include "logger.h"
#include "api.h"

int logger_init() {
  int fd = open("/dev/console", O_WRONLY);
  if (fd == -1) {
    notify("unable to redirect klog !!");
    return -1;
  }

  if (dup2(fd, STDOUT_FD) == -1) {
    notify("unable to dup %#x to %#x !!", fd, STDOUT_FD);
    return -1;
  }

  if (dup2(STDOUT_FD, STDERR_FD) == -1) {
    notify("unable to dup %#x to %#x !!", STDOUT_FD, STDERR_FD);
    return -1;
  }

  return 0;
}

void log(const char *fmt, ...) {
  const char tag[] = "[kexp] ";
  va_list args, args_copy;

  va_start(args, fmt);
  va_copy(args_copy, args);

  int len = vsnprintf(0, 0, fmt, args_copy);
  va_end(args_copy);

  if (len <= 0) {
    va_end(args);
    return;
  }

  int tag_len = sizeof(tag) - 1;
  int total_len = tag_len + len + 1; // +1 for \n

  char *buf = malloc(total_len);
  if (!buf) {
    va_end(args);
    return;
  }

  memcpy(buf, tag, tag_len);
  vsnprintf(buf + tag_len, len + 1, fmt, args);
  va_end(args);

  buf[tag_len + len] = '\n';

  ssize_t n = write(STDOUT_FD, buf, total_len);
  if (n == -1) {
    free(buf);
    return;
  }
}

void notify(const char *fmt, ...) {
  NotificationRequest req;
  va_list args;

  memset(&req, 0, sizeof(req));

  va_start(args, fmt);

  vsnprintf(req.message, sizeof(req.message), fmt, args);

  va_end(args);

  sceKernelSendNotificationRequest(0, &req, sizeof(req), 0);
}