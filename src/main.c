#include "api.h"
#include "iommu.h"
#include "kernel.h"
#include "loader.h"
#include "utils.h"

ENTRY int main(payload_args_t *args) {
  if (args == 0)
    return -1;

  init_libkernel_api();
  init_libc_api();

  init_karw(args);
  init_kaddrs(args->allproc);
  patch_qa_flags();
  init_loader(args->elf, args->elf_sz);
  init_loader_args();
  run_loader();

  return 0;
}
