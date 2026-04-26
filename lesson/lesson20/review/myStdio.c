#include "myStdio.h"

FILE_* fopen_(const char* path_name, const char* mode)
{
// 1.文件的rwa权限设置的
  int flags = 0;
  if(strcmp(mode, "r") == 0)
  {
    flags |= O_RDONLY;
  }
  else if(strcmp(mode, "w") == 0)
  {
    flags |= (O_WRONLY | O_CREAT | O_TRUNC);
  }
  else if(strcmp(mode, "a") == 0)
  {
    flags |= (O_WRONLY | O_CREAT | O_APPEND);
  }
  else 
  {
  }

// 2.文件是打开还是写和追加的
  int fd = 0;
  if(flags & O_RDONLY)
  {
    fd = open(path_name, flags);
  }
  else 
  {
    fd = open(path_name, flags, 0666);
  }
  
  if(fd < 0)
  {
    const char* str = strerror(errno);
    write(2, str, strlen(str));
    return NULL;
  }

// 3.填充c语言的结构体

  FILE_* fp = (FILE_*)malloc(sizeof(FILE_));
  
  fp->flags = SYNC_LINE;
  fp->fileno = fd;
  fp->cap = SIZE;
  fp->szie = 0;
  memset(fp->buffer, 0, SIZE);

  return fp;
}
