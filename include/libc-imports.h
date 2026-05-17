#define LIBC_HANDLE 2

API(void *, malloc, (size_t size))
API(void, free, (void *ptr))
API(void, memcpy, (void *dst, const void *src, size_t num))
API(size_t, strlen, (const char *str))
API(void, memset, (void *ptr, int value, size_t num))
API(int, strcmp, (const char *s1, const char *s2))
API(int, memcmp, (const void *s1, const void *s2, size_t n))
API(char *, vsnprintf, (char *s, size_t n, const char *format, va_list arg))