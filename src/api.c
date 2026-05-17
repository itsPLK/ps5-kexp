#include "api.h"

#define API(ret, name, args) DATA ret(*name) args;
#include "libc-imports.h"
#include "libkernel-imports.h"
#undef API

void init_libkernel_api() {
  void *addrp;

#define API(ret, name, args)                                                   \
  do {                                                                         \
    if (dlsym(LIBKERNEL_HANDLE, #name, &addrp))                                \
      __builtin_trap();                                                        \
    name = (ret(*) args)(addrp);                                               \
  } while (0);

#include "libkernel-imports.h"

#undef API
}

void init_libc_api() {
  void *addrp;

#define API(ret, name, args)                                                   \
  do {                                                                         \
    if (dlsym(LIBC_HANDLE, #name, &addrp))                                     \
      __builtin_trap();                                                        \
    name = (ret(*) args)(addrp);                                               \
  } while (0);

#include "libc-imports.h"

#undef API
}