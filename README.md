# aarch64-os

a simple OS for aarch64(qemu virt board)

![sample](screenshot/shot.png)

## features
- single processor
- preemptive multi tasking
- process scheduler
  - round-robin
- file system(ramfs)
  - ext2
- UNIX-like syscalls
- isolation of address space by MMU
  - multiple address space
- command-line interface(sh)
- some core utils
  - ls, cat, echo etc...

## Required
- aarch64-linux-gnu-toolchain
- qemu-system-aarch64

## Build & Run

```
$ make qemu
```

## License

MIT License
