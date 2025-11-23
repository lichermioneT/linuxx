#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILE_NAME "log.txt"

int main()
{
  
  umask(0);

  int fd = open(FILE_NAME, O_RDONLY);
  if(fd < 0)
  {
    perror("open");
    return 1;
  }

  char buffer[1024];
  ssize_t num = read(fd, buffer, sizeof(buffer) - 1);
  if(num > 0) buffer[num] = 0;
  printf("%s \n", buffer);


  close(fd);








  // c-- W > O_WRONLY | O_CREAT | O_TRUNC, 0666
  /*
   *int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0666);
   */

  /*int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_APPEND, 0666);*/

  /*if(fd < 0)*/
  /*{*/
    /*perror("open");*/
    /*return 1;*/
  /*}*/

  /*int cnt = 5;*/
  /*char buffer[64];*/
  /*while(cnt)*/
  /*{*/
    /*sprintf(buffer, "%s:%d\n", "harallo bit", cnt--);*/
    /*// 你以斜杠零结尾是c语言规定的，和我文件有什么关系呢？*/
    /*write(fd, buffer, strlen(buffer)); // strlen计算\0之前的数据, 这里面不写要写入 \0*/
  /*}*/


  /*
   *printf("%d \n", fd);
   */
  
  /*close(fd);*/








/*
 *   FILE* fp = fopen(FILE_NAME, "a"); // r w r+(读写，不存在出错) w+(读写，不存在出错)  a(append 追加) a+
 *  
 *   if(NULL == fp)
 *   {
 *     perror("fopen"); 
 *     return 1;
 *   }
 *
 *  
 *   int cnt = 5;
 *   while(cnt)
 *   {
 *     fprintf(fp, "%s:%d\n", "hello bit", cnt--);
 *   }
 *   
 *   fclose(fp);
 */


  /*
   * FILE* fp = fopen(FILE_NAME, "r"); // r w r+(读写，不存在出错) w+(读写，不存在出错)  a(append 追加) a+
   *
   * if(NULL == fp)
   * {
   *   perror("fopen"); 
   *   return 1;
   * }
   *
   * char buffer[64];
   * while(fgets(buffer, sizeof(buffer) - 1, fp) != NULL)
   * {
   *   buffer[strlen(buffer) - 1] = 0;
   *    puts(buffer);
   * }
   *
   * fclose(fp);
   */

/*
 *  FILE* fp = fopen(FILE_NAME, "w"); // r w r+(读写，不存在出错) w+(读写，不存在出错)  a(append 追加) a+
 *  if(NULL == fp)
 *  {
 *    perror("fopen"); 
 *    return 1;
 *  }
 *  
 *  int cnt = 5;
 *  while(cnt)
 *  {
 *    fprintf(fp, "%s:%d\n", "hello bit", cnt--);
 *  }
 *
 *  fclose(fp);
 */

  return 0;
}
