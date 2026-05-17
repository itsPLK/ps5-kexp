#ifndef IOMMU_H
#define IOMMU_H

#include "types.h"

#define IOMMU_CMD_COMPLETION_WAIT 1
#define IOMMU_MMIO_BASE 0xfdd80000
#define IOMMU_QUEUE_SIZE 0x2000

typedef struct {
  uint32_t s : 1;
  uint32_t i : 1;
  uint32_t f : 1;
  uint32_t address0 : 29;
  uint32_t address1 : 20;
  uint32_t rsrv : 8;
  uint32_t op : 4;
  uint32_t data0 : 32;
  uint32_t data1 : 32;
} iommu_cmd_completion_wait;

void iommu_init();
ssize_t iommu_write(uintptr_t vaddr, uint64_t value, size_t sz);
ssize_t iommu_write_pa(uintptr_t paddr, uint64_t value);
#endif