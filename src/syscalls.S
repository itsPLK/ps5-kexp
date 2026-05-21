.intel_syntax noprefix

.text
.global syscall
syscall:
  mov r10, rcx
  syscall
  jb fail
  ret

.global fail
fail:
  mov rax, -1
  mov rdx, -1
  ret