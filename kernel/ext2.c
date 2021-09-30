#include "ext2.h"
#include "printk.h"
#include "log.h"

struct imginfo imginfo;

void dump_superblock(struct superblock *sb) {
  printk("superblock dump sb %p\n", sb);
  printk("sizeof *sb %d\n", sizeof(*sb));
  printk("s_inodes_count: %d\n", sb->s_inodes_count);
  printk("s_blocks_count: %d\n", sb->s_blocks_count);
  printk("s_first_data_block: %d\n", sb->s_first_data_block);
  printk("s_last_mounted: %s\n", sb->s_last_mounted);
  printk("s_log_block_size: %d\n", sb->s_log_block_size);
  printk("s_mtime: %d\n", sb->s_mtime);
  printk("s_wtime: %d\n", sb->s_wtime);
  printk("s_state: %d\n", sb->s_state);
  printk("block_size: %d\n", 1024 << sb->s_log_block_size);
  printk("s_first_ino: %d\n", sb->s_first_ino);
  printk("s_inode_size: %d\n", sb->s_inode_size);
  printk("s_block_group_nr: %d\n", sb->s_block_group_nr);
  printk("s_blocks_per_group %d\n", sb->s_blocks_per_group);
  printk("s_inodes_per_group %d\n", sb->s_inodes_per_group);
}

void dump_bg_desc(struct bg_desc *bg) {
  printk("bg_desc dump %p\n", bg);
  printk("sizeof *bg: %d\n", sizeof(*bg));
  printk("bg_block_bitmap: %d\n", bg->bg_block_bitmap);
  printk("bg_inode_bitmap: %d\n", bg->bg_inode_bitmap);
  printk("bg_inode_table: %d\n", bg->bg_inode_table);
  printk("bg_free_blocks_count: %d\n", bg->bg_free_blocks_count);
  printk("bg_free_inodes_count: %d\n", bg->bg_free_inodes_count);
  printk("bg_used_dirs_count: %d\n", bg->bg_used_dirs_count);
}

#define inode_nblock(i) ((i)->i_blocks / (2 << 0))

void dump_inode(struct inode *i) {
  printk("inode dump: %p\n", i);
  printk("sizeof *i: %d\n", sizeof(*i));
  printk("i_mode: %p\n", i->i_mode);
  printk("i_blocks: %p\n", i->i_blocks);
  for(int b = 0; b < 15; b++)
    printk("i_block[%d]: %p\n", b, i->i_block[b]);
}

void dump_ext2_dirent(struct ext2_dirent *d) {
  printk("ext2_dirent dump: %p\n", d);
  printk("sizeof *d: %d\n", sizeof(*d));
  printk("inode: %d\n", d->inode);
  printk("rec_len: %d\n", d->rec_len);
  printk("name_len: %d\n", d->name_len);
  printk("file_type: %d\n", d->file_type);
  printk("name: %s\n", d->name);
}

void dump_dirent_block(char *blk) {
  struct ext2_dirent *d = (struct ext2_dirent *)blk;
  char *blk_end = blk + imginfo.block_size;
  char *cd;

  while(d->inode != 0 && blk_end != cd) {
    dump_ext2_dirent(d);
    cd = (char *)d;
    cd += d->rec_len;
    d = (struct ext2_dirent *)cd;
  }
}

struct inode *get_inode(int inum) {
  return (struct inode *)(imginfo.inode_table + (inum - 1) * sizeof(struct inode));
}

void *get_block(int bnum) {
  return imginfo.base + (u64)bnum * imginfo.block_size;
}

void ls_inode(struct inode *ino) {
  for(int i = 0; i < inode_nblock(ino); i++) {
    char *d = get_block(ino->i_block[i]);
    dump_dirent_block(d);
  }
}

void fs_init(char *img) {
  struct superblock *sb = (struct superblock *)(img + 0x400);
  dump_superblock(sb);
  if(sb->s_magic != 0xef53)
    panic("invalid filesystem");
  u32 block_size = 1024 << sb->s_log_block_size;

  struct bg_desc *bg = (struct bg_desc *)(img + 0x800);
  dump_bg_desc(bg);

  imginfo.base = img;
  imginfo.block_size = block_size;
  imginfo.block_bitmap = get_block(bg->bg_block_bitmap);
  imginfo.inode_bitmap = get_block(bg->bg_inode_bitmap);
  imginfo.inode_table = get_block(bg->bg_inode_table);

  struct inode *i = get_inode(EXT2_ROOT_INO);
  dump_inode(i);
  ls_inode(i);
}