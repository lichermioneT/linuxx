#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define USER "USER"
#define MY_VAL "myval"
#define MYPWD "PWD"

extern char** environ;
// int main(int argc, char* argv[], char* env[])
int main()
{
  
  for(int i = 0; environ[i]; ++i)
  {
    printf("%d:%s\n", i, environ[i]);
  }
  
  /*
   *for(int i = 0; env[i]; ++i)
   *{
   *  printf("%d:%s\n", i, env[i]);
   *}
   */


/*
 *  if(argc != 2)
 *  {
 *    printf("Usage: \n%s [-a/-b/-c/-ab/-ac/-bc/-abc]\n", argv[0]);
 *    return 1;
 *  }
 *
 *  if(strcmp("-a", argv[1]) == 0)
 *  {
 *    printf("功能a\n");
 *  }
 *  if(strcmp("-b", argv[1]) == 0)
 *  {
 *    printf("功能b\n");
 *  }
 *  
 *  if(strcmp("-c", argv[1]) == 0)
 *  {
 *    printf("功能c\n");
 *  }
 *
 *  if(strcmp("-ab", argv[1]) == 0)
 *  {
 *    printf("功能ab\n");
 *  }
 *
 *  if(strcmp("-ac", argv[1]) == 0)
 *  {
 *    printf("功能ac\n");
 *  }
 *
 *  if(strcmp("-bc", argv[1]) == 0)
 *  {
 *    printf("功能bc\n");
 *  }
 *  if(strcmp("-abc", argv[1]) == 0)
 *  {
 *    printf("功能abc\n");
 *  }
 *
 */

/*
 *
 *  for(int i = 0; i < argc; ++i)
 *  {
 *#if 0
 *     printf("%d:argv[%s]\n", argc, argv[i]);
 *#else 
 *     printf("%d:argv[%s]\n", argc, *(argv + i));
 *#endif
 *  }
 *
 */


  /*
   *printf("%s\n", getenv(MYPWD));
   */

/*
 *  char* myenv = getenv(MY_VAL);
 *  if(NULL == myenv)
 *  {
 *    printf("%s, not found\n", MY_VAL);
 *    return 1;
 *  }
 *
 *  printf("%s:%s\n", MY_VAL, myenv);
 */

  /*
   *char* user = getenv(USER);
   *if(strcmp(user, "root") == 0)
   *{
   * printf("user:%s\n", user);
   *}
   *else 
   *{
   *  printf("权限不足\n");
   *}
   */
  return 0;
}
