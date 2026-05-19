#define LIBKERNEL_HANDLE 0x2001

API(int *, __error, (void))
API(void, scePthreadYield, (void))
API(void, sceKernelSendNotificationRequest,
    (int32_t device, NotificationRequest *req, size_t size, int32_t blocking))
API(int, sysctlbyname,
    (const char *name, void *oldp, size_t *oldlenp, const void *newp,
     size_t newlen))
API(int, pthread_create,
    (pthread_t *restrict thread, const pthread_attr_t *restrict attr,
     void *(*start_routine)(void *), void *restrict arg))
API(int, pthread_join, (pthread_t thread, void **value_ptr))