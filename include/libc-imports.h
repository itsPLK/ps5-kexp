#define LIBC_HANDLE 2

API(int *, __error, (void))
API(void *, malloc, (size_t size))
API(void, free, (void *ptr))
API(void, memcpy, (void *dst, const void *src, size_t num))
API(void, memset, (void *ptr, int value, size_t num))
API(int, strcmp, (const char *s1, const char *s2))
API(int, memcmp, (const void *s1, const void *s2, size_t n))
API(int, vsnprintf,
    (char *restrict str, size_t size, const char *restrict format, va_list ap))