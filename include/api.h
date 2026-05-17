#ifndef API_H
#define API_H

#include "syscalls.h"
#include "types.h"

#define API(ret, name, args) COMMON ret(*name) args;
#include "libc-imports.h"
#include "libkernel-imports.h"
#undef API

void init_libkernel_api();
void init_libc_api();

#endif