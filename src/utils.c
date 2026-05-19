#include "utils.h"

void notify(const char *fmt, ...) {
  NotificationRequest req;

  memset(&req, 0, sizeof(req));

  va_list args;

  va_start(args, fmt);
  vsnprintf(req.message, sizeof(req.message), fmt, args);
  va_end(args);

  sceKernelSendNotificationRequest(0, &req, sizeof(req), 0);
}

uint32_t get_fw_version() {
  uint32_t version;

  size_t size = sizeof(version);

  if (sysctlbyname("kern.sdk_version", &version, &size, 0, 0) == -1) {
    return -1;
  }

  return version;
}