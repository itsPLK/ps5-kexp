.intel_syntax noprefix

.extern __error

.text
.global syscall
syscall:
  mov r10, rcx
  syscall
  jb fail
  ret

.global fail
fail:
  push rax
  call __error
  pop rcx
  mov [rax], ecx
  mov rax, -1
  mov rdx, -1
  ret