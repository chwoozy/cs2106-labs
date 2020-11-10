#include "zc_io.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>

// The zc_file struct is analogous to the FILE struct that you get from fopen.
struct zc_file
{
  void *addr;
  size_t size;
  int offset;
};

/**************
 * Exercise 1 *
 **************/

zc_file *zc_open(const char *path)
{
  //init variables
  struct stat s;
  zc_file *zc;
  int fd;
  void *addr;

  //open file
  if ((fd = open(path, O_CREAT | O_RDWR, 0644)) == -1)
  {
    perror("Error opening file...");
  }

  // get stat of file
  fstat(fd, &s);
  if ((addr = mmap(NULL, s.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
  {
    perror("Error mapping...make");
  }

  zc->addr = addr;
  zc->size = s.st_size;
  zc->offset = 0;
  return zc;
}

int zc_close(zc_file *file)
{
  return munmap(file->addr, file->size);
}

const char *zc_read_start(zc_file *file, size_t *size)
{
  int off = file->offset;

  if (file->size < *size)
  {
    *size = file->size;
    size = 0;
  }
  else
  {
    file->size -= *size;
  }
  file->offset += *size;

  return (char *)file->addr + off;
}

void zc_read_end(zc_file *file)
{
  // To implement
}

/**************
 * Exercise 2 *
 **************/

char *zc_write_start(zc_file *file, size_t size)
{
  // To implement
  return NULL;
}

void zc_write_end(zc_file *file)
{
  // To implement
}

/**************
 * Exercise 3 *
 **************/

off_t zc_lseek(zc_file *file, long offset, int whence)
{
  // To implement
  return -1;
}

/**************
 * Exercise 5 *
 **************/

int zc_copyfile(const char *source, const char *dest)
{
  // To implement
  return -1;
}
