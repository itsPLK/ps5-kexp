#include "iommu.h"
#include "kernel.h"
#include "utils.h"

int is_kernel_pointer(uintptr_t ptr) {
  if (ptr == 0)
    return -1;

  if (ptr % 8)
    return -1;

  if ((ptr >> 0x30) != 0xffff)
    return -1;

  uintptr_t heap_prefix = (ptr >> 0x20) & 0xffff;

  if (heap_prefix == 0xffff || heap_prefix == 0)
    return -1;

  return 0;
}

uintptr_t find_softc() {
  uintptr_t ptr;
  uintptr_t mmio;
  uintptr_t sotfc;
  uintptr_t mmio_paddr;

  for (uintptr_t addr = kaddrs.allproc; addr >= kaddrs.kdata; addr -= 8) {
    if (kread(&ptr, addr, sizeof(ptr)) == -1)
      return -1;

    if (is_kernel_pointer(ptr) == -1)
      continue;

    if (kread(&mmio, ptr + 0x40, sizeof(mmio)) == -1)
      return -1;

    if (is_kernel_pointer(mmio) == -1)
      continue;

    if (kread(&mmio_paddr, ptr + 0x48, sizeof(mmio_paddr)) == -1)
      return -1;

    if (mmio_paddr == 0)
      continue;

    if ((mmio & 0xffffffff) != mmio_paddr)
      continue;

    if (kread(&sotfc, addr, sizeof(sotfc)) == -1)
      return -1;

    return sotfc;
  }

  return -1;
}

void iommu_init() {
  uintptr_t kp = pfind(KERNEL_PID);
  uintptr_t kp_pmap = get_vm_map_pmap(kp);

  uintptr_t pml4;

  kread(&pml4, kp_pmap + 0x20, sizeof(pml4));
  kread(&kaddrs.cr3, kp_pmap + 0x28, sizeof(kaddrs.cr3));

  kaddrs.dmap = pml4 - kaddrs.cr3;
  kaddrs.mmio = get_dmap(IOMMU_MMIO_BASE);

  uintptr_t softc = find_softc();

  kread(&kaddrs.cb2, softc + 0x78, sizeof(kaddrs.cb2));

  notify("mmio: %#lx\ncb2: %#lx\ndmap: %#lx\ncr3: %#lx", kaddrs.mmio,
         kaddrs.cb2, kaddrs.dmap, kaddrs.cr3);
}

ssize_t iommu_write(uintptr_t vaddr, uint64_t value, size_t sz) {
  if (sz == 0 || sz > sizeof(uint64_t))
    return -1;

  if (vaddr == 0)
    return -1;

  uintptr_t vaddr_offset = vaddr % sizeof(uint64_t);
  uintptr_t vaddr_aligned = ALIGN_DOWN(vaddr, sizeof(uint64_t));

  if (vaddr_aligned == 0)
    return -1;

  uint64_t old_value;

  if (kread(&old_value, vaddr_aligned, sizeof(old_value)) == -1)
    return -1;

  size_t count = sizeof(uint64_t) - vaddr_offset;
  size_t n = sz < count ? sz : count;

  uint64_t mask =
      n == 8 ? UINT64_MAX : ((1ul << (n * 8)) - 1) << (vaddr_offset * 8);
  uint64_t new_value =
      (old_value & ~mask) | ((value << (vaddr_offset * 8)) & mask);

  uintptr_t paddr_aligned = get_paddr(vaddr_aligned);

  if (iommu_write_pa(paddr_aligned, new_value) == -1)
    return -1;

  if (sz > count) {
    if (kread(&old_value, vaddr_aligned + sizeof(uint64_t),
              sizeof(old_value)) == -1)
      return -1;

    n = sz - count;
    mask = n == 8 ? UINT64_MAX : (1ul << (n * 8)) - 1;
    new_value = (old_value & ~mask) | ((value >> (count * 8)) & mask);

    paddr_aligned = get_paddr(vaddr_aligned + 8);
    if (iommu_write_pa(paddr_aligned, new_value) == -1)
      return -1;
  }

  return sz;
}

ssize_t iommu_write_pa(uintptr_t paddr, uint64_t value) {
  iommu_cmd_completion_wait cmd;
  uintptr_t cur_tail;
  uintptr_t cur_head;

  if (paddr == 0 || (paddr % sizeof(uint64_t)) != 0)
    return -1;

  cmd.s = 1;
  cmd.i = 0;
  cmd.f = 0;
  cmd.address0 = paddr >> 3;
  cmd.address1 = paddr >> 32;
  cmd.rsrv = 0;
  cmd.op = IOMMU_CMD_COMPLETION_WAIT;
  cmd.data0 = value;
  cmd.data1 = value >> 32;

  if (kread(&cur_tail, kaddrs.mmio + 0xa008, sizeof(cur_tail)) == -1)
    return -1;

  uintptr_t next_tail = (cur_tail + sizeof(cmd)) % IOMMU_QUEUE_SIZE;

  if (kwrite(kaddrs.cb2 + cur_tail, &cmd, sizeof(cmd)) == -1)
    return -1;

  if (kwrite(kaddrs.mmio + 0xa008, &next_tail, sizeof(next_tail)) == -1)
    return -1;

  while (1) {
    kread(&cur_head, kaddrs.mmio + 0xa000, sizeof(cur_head));
    kread(&cur_tail, kaddrs.mmio + 0xa008, sizeof(cur_tail));

    if (cur_head == cur_tail)
      break;
  }

  return 0;
}