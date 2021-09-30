#include "kernel.h"
#include "ramdisk.h"

extern char _binary_fs_img_start[];
extern char _binary_fs_img_end[];
extern char _binary_fs_img_size[];

struct {
  char *data;
  u64 size;
} disk;

void ramdisk_init() {
  disk.data = _binary_fs_img_start;
  disk.size = (u64)_binary_fs_img_size;
}

char *diskread(u64 off) {
  return disk.data + off;
}