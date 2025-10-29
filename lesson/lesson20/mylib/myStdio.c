#include "myStdio.h"



FILE_* fopen_(const char* path_name, const char* mode)
{
    int flags = 0;
    int defaultMode=0666;
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
      // todo
    }

    int fd = 0;
    
    if(flags & O_RDONLY) fd = open(path_name, flags);
    else fd = open(path_name, flags, defaultMode);

    if(fd < 0)
    {
      const char* err = strerror(errno);
      write(2, strerror(errno), strlen(err));
      return NULL;
    }



    FILE_* fp = (FILE_*)malloc(sizeof(FILE_));
  
    assert(fp);
  
    fp->flags = SYNC_LINE;
    fp->fileno = fd;
    fp->cap = SIZE;
    fp->szie = 0;
    memset(fp->buffer, 0, SIZE);
    

     
    return fp; // 这就是为什么打开一个文件，就会返回一个FILE* 指针
}



void fwrite_(const void* ptr, int num, FILE_* fp)
{
  // 数据写入到缓冲区中
   memcpy(fp->buffer + fp->szie, ptr, num); // 不考虑缓冲区的溢出
   
   fp->szie += num;
   // 判断是否刷新
   
   if(fp->flags & SYNC_NOW)
   {
    write(fp->flags, fp->buffer, fp->szie);   
    fp->szie = 0; // 清空缓冲区
   }
   else if(fp->flags & SYNC_FULL)
   {
     if(fp->szie == fp->cap)  
     {
        write(fp->fileno, fp->buffer, fp->szie);
        fp->szie =  0;
     }
   }
   else if(fp->flags & SYNC_LINE)
   {
      if(fp->buffer[fp->szie-1] == '\n')
      {
        write(fp->fileno, fp->buffer, fp->szie);
        fp->szie =  0;
      }
   }
   else 
   {

   }
}



void fflush_(FILE_* fp)
{
  // 写到系统里面
  if(fp->szie > 0)write(fp->fileno, fp->buffer, fp->szie);

  // 真正的刷新
  fsync(fp->fileno);
  fp->szie = 0;
}

void fclose_(FILE_* fp)
{
  fflush_(fp);
  close(fp->fileno);  
}
