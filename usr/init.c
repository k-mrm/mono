#include "usys.h"
#include "ulib.h"
#include "cdev.h"
#include "fs.h"
#include "fcntl.h"

int main() {
  char *argv[] = {"loop", 0};
  char *argv2[] = {"loop", 0};
  int fd;

  if((fd = open("tty0", O_RDWR)) < 0) {
    if(mknod("tty0", S_IFCHR, CDEV_CONSOLE) < 0)
      exit(1);
    fd = open("tty0", O_RDWR);
  }

  dup(fd);    /* 1 */
  dup(fd);    /* 2 */

  puts("init");
  puts("starting sh");

  int pid = fork();
  if(pid == 0) {
    exec("loop", argv);
  } else {
    int pid2 = fork();
    if(pid2 == 0) {
      exec("loop", argv2);
    }
    else {
      wait(NULL);
      exit(0);
    }
    wait(NULL);
  }

  exit(0);
}
