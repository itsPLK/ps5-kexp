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

  int mib[2] = {1, 46}; // kern.sdk_version
  size_t size = sizeof(version);

  if (sysctl(mib, 2, &version, &size, 0, 0)) {
    return -1;
  }

  return version;
}